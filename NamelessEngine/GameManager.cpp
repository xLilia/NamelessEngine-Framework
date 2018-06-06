#include "GameManager.h"

_NL::Engine::GameManager::GameManager(std::string WindowName, int Width, int Height, bool hideMouse, bool fullscreen, bool borderless, bool bVSync, int fpsLimit)
{
	//---------------------------------------------------------------------------------
	//WINDOW INITIALIZATION
	//---------------------------------------------------------------------------------
	this->WindowName = WindowName;
	window = new sf::RenderWindow();
	if (borderless) {
		GoBorderless(Width,Height);
	}
	else if(fullscreen){
		GoFullscreen();
	}
	else {
		GoWindowed(Width,Height);
	}
	window->setMouseCursorVisible(!hideMouse);
	window->setFramerateLimit(fpsLimit);
	window->setVerticalSyncEnabled(bVSync);
	window->setKeyRepeatEnabled(false);
	glewInit();
	//_toggle_gl_debug();
}

void _NL::Engine::GameManager::GoFullscreen()
{
	window->create(sf::VideoMode::getFullscreenModes()[0], this->WindowName, sf::Style::Fullscreen);
}

void _NL::Engine::GameManager::GoWindowed(GLfloat Width, GLfloat Height)
{
	window->create(sf::VideoMode(Width, Height), this->WindowName, sf::Style::Default);
}

void _NL::Engine::GameManager::GoBorderless(GLfloat Width, GLfloat Height)
{
	window->create(sf::VideoMode(Width, Height), this->WindowName, sf::Style::None);
}


void _NL::Engine::GameManager::UpdateObjectLists() {
	this->Cameras = CurrentScene->getAllObjsOfType<_NL::Object::CameraObj>();
	if (Cameras.size() == 0) std::cout << "Camera not found in Current Scene..." << std::endl;
	this->Lights = CurrentScene->getAllObjsOfType<_NL::Object::LightObject>();
	this->ParticleSystems = CurrentScene->getAllObjsOfType<_NL::Object::ParticleSystem>();
	this->UICanvas = CurrentScene->getAllObjsOfType<_NL::UI::UICanvas>();
}

void _NL::Engine::GameManager::SafeKillObj(_NL::Core::Object * Target)
{
	if (Target != nullptr) {
		CurrentScene->KillObjectInstance(Target);
	}
}

void _NL::Engine::GameManager::RunScene(_NL::Engine::WorldSpace* set_current_scene)
{

	//GL SETTINGS
	{
		///GL OPTIONS
		glShadeModel(GL_SMOOTH);
		///RENDER
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}

	this->CurrentScene = set_current_scene;

	UpdateObjectLists();

	while (window->isOpen()) {

		//glFinish();

		while (window->pollEvent(Event));
		{
			//PROCESS INPUT STACK
		}	

		//---------------------------------------------------------------------------------
		//START & UPDATE SCRIPTS 
		for (_NL::Core::ObjTypeList OTL : CurrentScene->ObjectLists) {
			for (_NL::Core::ObjInstanceList OIL : OTL) {
				for (_NL::Core::Object* Instance : OIL) {
					StartUpdateScriptsOfObj(Instance);
				}
			}
		}

		//---------------------------------------------------------------------------------
		//PARTICLE SYSTEMS
		UpdateParticleSystems();

		UpdateObjectLists();

		//---------------------------------------------------------------------------------
		//CHECK FOR END OF CURRENT SCENE 
		if (bEndCurrentScene) {
			bEndCurrentScene = false;
			break;
		}
		
		//---------------------------------------------------------------------------------
		//UPDATE WINDOW
		if (Cameras.size() > 0) {
			window->setActive(true);
			RenderCurrentScene();
			check_gl_error();
			window->setActive(false);
		}
		//---------------------------------------------------------------------------------
		//TICK TIME  
		GameTick();
	}

}

void _NL::Engine::GameManager::EndCurrentScene(_NL::Engine::WorldSpace* load_next_scene)
{
	//STOP SCENE UPDATE
	bEndCurrentScene = true;
	//CLEAN AND PREPARE FOR NEXT SCENE
	CleanUpCurrentSceneLoadedResources();
	//LOAD NEXT SCENE IF !NULL
	if (load_next_scene) {
		bEndCurrentScene = false;
		RunScene(load_next_scene);
	}
}

void _NL::Engine::GameManager::CleanUpCurrentSceneLoadedResources()
{
	for (_NL::Core::ObjTypeList OTL : CurrentScene->ObjectLists) {
		for (_NL::Core::ObjInstanceList OIL : OTL) {
			for (_NL::Core::Object* Instance : OIL) {
				EndScriptsOfObj(Instance);
			}
		}
	}
	Cameras.clear();
	Lights.clear();
	UICanvas.clear();
	ParticleSystems.clear();
	glDeleteBuffers(1, &LightsSSBO);
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//SCRIPTING UPDATES
//---------------------------------------------------------------------------------

void _NL::Engine::GameManager::StartUpdateScriptsOfObj(_NL::Core::Object* obj) {

	for each (_NL::Component::CppScript<_NL::Core::Script>* s in obj->Components)
	{
		if (s->getTypeName() == "_NL::Component::CppScript") {
			s->getScript()->_this = obj; // ducktape FIX!!! Reference doesn't get passed when obj added to vector
			if (!s->getScript()->awake) {
				s->getScript()->awake = true;
				s->getScript()->Start();
			}
			s->getScript()->Update();
		}
	}
}

void _NL::Engine::GameManager::EndScriptsOfObj(_NL::Core::Object* obj) {
	for each (_NL::Component::CppScript<_NL::Core::Script>* s in obj->Components)
	{
		if (s->getTypeName() == "_NL::Component::CppScript") {
			s->getScript()->End();
		}

	}
}

void _NL::Engine::GameManager::RenderCurrentScene() {

	//window->resetGLStates();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	//---------------------------------------------------------------------------------
	//Update Lights
	UpdateSceneLights();
	for each (_NL::Object::CameraObj* Cam in Cameras)
	{	

		//---------------------------------------------------------------------------------
		//UPDATE CAM

		glBindFramebuffer(GL_FRAMEBUFFER, Cam->PostProcessingReadyFramebuffer);
		Cam->ClearCurrentRenderTarget();
		Cam->SetThisCamViewPort();

		glDisable(GL_DEPTH_TEST);
		RenderSceneSkybox(Cam);
		glEnable(GL_DEPTH_TEST);

		Cam->SetCamAsRenderTarget();
		Cam->ClearCurrentRenderTarget();

		glEnable(GL_DEPTH_TEST);
		RenderSceneObjects(Cam);
		glDisable(GL_DEPTH_TEST);
		
		Cam->RenderToPostProcessingFramebuffer();

		//POSTPROCESSING STACK _.-Lwip`-._

		Cam->blitPostProcessedImageToScreen();
		
	}

	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDisable(GL_DEPTH_TEST);
	//RenderSceneCanvas();

	window->display();
	


}

void _NL::Engine::GameManager::UpdateSceneLights() {
	
	//for (GLuint i = 0; i < Lights.size(); i++) {
	//	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	//	glBindFramebuffer(GL_FRAMEBUFFER, Lights[i]->Framebuffer);
	//	glClear(GL_DEPTH_BUFFER_BIT);
	//
	//	float near_plane = 1.0f, far_plane = 7.5f;
	//	glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	//
	//	glm::mat4 lightView = glm::lookAt(
	//		Lights[i]->LightProperties.lightPosition,
	//		Lights[i]->LightProperties.lightDirection,
	//		glm::vec3(0.0f, 1.0f, 0.0f)
	//	);
	//
	//	RenderSceneObjects(Lights[i]->LightProperties.lightPosition, lightView, lightProjection, this->DepthPassShader->getShaderProgram());
	//
	//}
	//INITIALIZE LIGHT SSBO

	std::vector<_NL::Core::LightProperties> LightsProperties;

	if (Lights.size() > 0) {

		for each(_NL::Object::LightObject* Lp in Lights) {
			LightsProperties.push_back(Lp->LightProperties);
		}
		
		glDeleteBuffers(1, &LightsSSBO);

		glGenBuffers(1, &LightsSSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, LightsSSBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(_NL::Core::LightProperties)*LightsProperties.size(), &LightsProperties[0], GL_DYNAMIC_DRAW);
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, LightsSSBOBindingPoint, LightsSSBO, 0, sizeof(_NL::Core::LightProperties)*LightsProperties.size());
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, LightsSSBOBindingPoint, LightsSSBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		check_gl_error();

		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, LightsSSBO);
		//glBufferSubData(GL_SHADER_STORAGE_BUFFER,
		//	0,
		//	sizeof(_NL::Core::LightProperties)*LightsProperties.size(),
		//	&LightsProperties[0]);
		//glBindBufferRange(GL_SHADER_STORAGE_BUFFER, LightsSSBOBindingPoint, LightsSSBO, 0, sizeof(_NL::Core::LightProperties)*LightsProperties.size());
		//glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}

void _NL::Engine::GameManager::UpdateParticleSystems() {
	if (ParticleSystems.size() > 0) {
		for each (_NL::Object::ParticleSystem* PS in ParticleSystems)
		{
			if(PS->CurrentScene != this->CurrentScene) PS->CurrentScene = this->CurrentScene;
			PS->TickSystem();
		}
	}
}

void _NL::Engine::GameManager::RenderSceneSkybox(_NL::Object::CameraObj* Cam) {
	
	if (CurrentScene->Skybox != 0) {
		CurrentScene->Skybox->SkyboxDysplayShader->Use();
		glUniformMatrix4fv(CurrentScene->Skybox->CamProjectionMatrix_uniform, 1, GL_FALSE, glm::value_ptr(Cam->getProjectionMatrix()));
		glUniformMatrix4fv(CurrentScene->Skybox->CamViewMatrix_uniform, 1, GL_FALSE, glm::value_ptr(Cam->getViewMatrix()));
		CurrentScene->Skybox->RenderSkybox();
	}
}

void _NL::Engine::GameManager::RenderSceneObjects(_NL::Object::CameraObj* Cam, GLuint UseOverrideShaderProgram) {
	check_gl_error();

	for (_NL::Core::ObjTypeList OTL : CurrentScene->ObjectLists) 
	for (_NL::Core::ObjInstanceList OIL : OTL) {
	//for(int objN = 0; objN < CurrentScene->ObjectList.size(); objN++)
	//for each (std::vector<_NL::Core::Object*> obj in CurrentScene->ObjectList)
		//std::vector<_NL::Core::Object*>& obj = CurrentScene->ObjectList[objN];
		if (OIL.size() > 0) {
			//MESH RENDERER
			_NL::Component::MeshRenderer* ObjMR = OIL[0]->getComponent<_NL::Component::MeshRenderer>();

			if (ObjMR != NULL) {
				if (UseOverrideShaderProgram == NULL) {
					ObjMR->Shader->Use();
					check_gl_error();
				}
				else {
					glUseProgram(UseOverrideShaderProgram);
					check_gl_error();
				}
				
				glBindVertexArray(ObjMR->vao);
				check_gl_error();
				//---------------------------------------------------------------------------------
				//TRANSFORMS
				std::vector<glm::mat4> Modelmat;

				for (int inst = 0; inst < OIL.size(); inst++)
					//for each (_NL::Core::Object* objInstance in obj)
				{
					//---------------------------------------------------------------------------------
					//PARENTS
					_NL::Core::Object* objInstance = OIL[inst];
					_NL::Component::Transform* _ObjT_P;
					_NL::Component::Transform* _ObjT = objInstance->getComponent<_NL::Component::Transform>();
					bool HasParent;

					if (objInstance->Parent && _ObjT) {
						HasParent = true;
						_ObjT_P = objInstance->Parent->getComponent<_NL::Component::Transform>();
					}
					else {
						HasParent = false;
					}

					//---------------------------------------------------------------------------------
					//INSTANCE MODEL MAT
					glm::mat4 T = glm::translate(glm::mat4(), _ObjT->transform.position);
					glm::mat4 R = glm::toMat4(_ObjT->transform.QuaternionRotation);
					glm::mat4 S = glm::scale(glm::mat4(), _ObjT->transform.scale);

					if (HasParent) {
						T *= glm::translate(glm::mat4(), _ObjT_P->transform.position);
						R *= glm::toMat4(_ObjT_P->transform.QuaternionRotation);
						S *= glm::scale(glm::mat4(), _ObjT_P->transform.scale);
					}

					glm::mat4 _Modelmat = T * R * S;
					Modelmat.push_back(_Modelmat);
				}
				//---------------------------------------------------------------------------------
				//CAM UNIFORMS
				check_gl_error();
				glUniformMatrix4fv(_NL::Core::GLSL_AU::ViewMatrix_uniform, 1, GL_FALSE, glm::value_ptr(Cam->getWorldToViewMatrix()));
				glUniformMatrix4fv(_NL::Core::GLSL_AU::ProjectionMatrix_uniform, 1, GL_FALSE, glm::value_ptr(Cam->getProjectionMatrix()));
				glm::vec3 *EyePos = &Cam->transformCam.Position;
				glUniform3f(_NL::Core::GLSL_AU::EyePos_uniform, EyePos->x, EyePos->y, EyePos->z);
				check_gl_error();
				
				//---------------------------------------------------------------------------------
				//LIGHT UNIFORMS
				//glUniform1i(_NL::Core::GLSL_AU::NumberOfLights_uniform, LightsProperties.size());
				//check_gl_error();

				//---------------------------------------------------------------------------------
				//BIND INSTANCE ARRAY
				GLuint InstanceABO;
				glGenBuffers(1, &InstanceABO);
				glBindBuffer(GL_ARRAY_BUFFER, InstanceABO);
				glBufferData(GL_ARRAY_BUFFER, OIL.size() * sizeof(glm::mat4), glm::value_ptr(Modelmat[0]), GL_STATIC_DRAW);
				check_gl_error();
				//---------------------------------------------------------------------------------
				//SETUP INSTANCE MODEL MAT ATRIB

				GLsizei vec4Size = sizeof(glm::vec4);
				glEnableVertexAttribArray(_NL::Core::GLSL_AU::InstModelMatrix0_atrib + 0);
				glVertexAttribPointer(_NL::Core::GLSL_AU::InstModelMatrix0_atrib + 0, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(0 * vec4Size));
				glEnableVertexAttribArray(_NL::Core::GLSL_AU::InstModelMatrix0_atrib + 1);
				glVertexAttribPointer(_NL::Core::GLSL_AU::InstModelMatrix0_atrib + 1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
				glEnableVertexAttribArray(_NL::Core::GLSL_AU::InstModelMatrix0_atrib + 2);
				glVertexAttribPointer(_NL::Core::GLSL_AU::InstModelMatrix0_atrib + 2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
				glEnableVertexAttribArray(_NL::Core::GLSL_AU::InstModelMatrix0_atrib + 3);
				glVertexAttribPointer(_NL::Core::GLSL_AU::InstModelMatrix0_atrib + 3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

				glVertexAttribDivisor(_NL::Core::GLSL_AU::InstModelMatrix0_atrib + 0, 1);
				glVertexAttribDivisor(_NL::Core::GLSL_AU::InstModelMatrix0_atrib + 1, 1);
				glVertexAttribDivisor(_NL::Core::GLSL_AU::InstModelMatrix0_atrib + 2, 1);
				glVertexAttribDivisor(_NL::Core::GLSL_AU::InstModelMatrix0_atrib + 3, 1);

				check_gl_error();

				if (ObjMR->Material) {
					//---------------------------------------------------------------------------------
					//SETUP MATERIAL UNIFORMS
					glUniform1i(_NL::Core::GLSL_AU::ALbedoTexture_uniform, 0);
					glUniform1i(_NL::Core::GLSL_AU::RoughnessTexture_uniform, 1);
					glUniform1i(_NL::Core::GLSL_AU::MetalnessTexture_uniform, 2);
					glUniform1i(_NL::Core::GLSL_AU::NormalTexture_uniform, 3);
					glUniform1i(_NL::Core::GLSL_AU::AmbientOculusionTexture_uniform, 4);
					glUniform1i(_NL::Core::GLSL_AU::AmbientIrradianceTexture_uniform, 5);
					glUniform1i(_NL::Core::GLSL_AU::PreFilterTexture_uniform, 6);
					glUniform1i(_NL::Core::GLSL_AU::BRDF2DLUTTexture_uniform, 7);
					check_gl_error();

					//---------------------------------------------------------------------------------
					//SEND MATERIAL DATA
					glActiveTexture(GL_TEXTURE0 + 0);
					if (ObjMR->Material->MaterialInstanceData.AlbedoTexId)
						glBindTexture(GL_TEXTURE_2D, ObjMR->Material->MaterialInstanceData.AlbedoTexId);

					glActiveTexture(GL_TEXTURE0 + 1);
					if (ObjMR->Material->MaterialInstanceData.RoughnessTexId)
						glBindTexture(GL_TEXTURE_2D, ObjMR->Material->MaterialInstanceData.RoughnessTexId);

					glActiveTexture(GL_TEXTURE0 + 2);
					if (ObjMR->Material->MaterialInstanceData.MetalnessTexId)
						glBindTexture(GL_TEXTURE_2D, ObjMR->Material->MaterialInstanceData.MetalnessTexId);
				
					glActiveTexture(GL_TEXTURE0 + 3);
					if (ObjMR->Material->MaterialInstanceData.NormalTexId)
						glBindTexture(GL_TEXTURE_2D, ObjMR->Material->MaterialInstanceData.NormalTexId);
				
					glActiveTexture(GL_TEXTURE0 + 4);
					if (ObjMR->Material->MaterialInstanceData.AmbientOculusionTexId)
						glBindTexture(GL_TEXTURE_2D, ObjMR->Material->MaterialInstanceData.AmbientOculusionTexId);
				
					if (this->CurrentScene->Skybox) {
						glActiveTexture(GL_TEXTURE0 + 5);
						if (this->CurrentScene->Skybox->IrradienceMap)
							glBindTexture(GL_TEXTURE_CUBE_MAP, this->CurrentScene->Skybox->IrradienceMap);

						glActiveTexture(GL_TEXTURE0 + 6);
						if (this->CurrentScene->Skybox->PreFilterMap)
							glBindTexture(GL_TEXTURE_CUBE_MAP, this->CurrentScene->Skybox->PreFilterMap);

						glActiveTexture(GL_TEXTURE0 + 7);
						if (this->CurrentScene->Skybox->BRDF_2D_LUTMap)
							glBindTexture(GL_TEXTURE_2D, this->CurrentScene->Skybox->BRDF_2D_LUTMap);
					}

					check_gl_error();
					//---------------------------------------------------------------------------------
					//DRAW MESH 

					ObjMR->UpdateGLSettings();
					//glDrawElementsInstanced(ObjMR->GL_RenderMode, ObjMR->IndicesBuf.size(), GL_UNSIGNED_INT, nullptr, obj.size());
					glDisable(GL_BLEND);
					glDrawArraysInstanced(ObjMR->GL_RenderMode, 0, ObjMR->VertsBuf.size(), OIL.size());
					glEnable(GL_BLEND);

					check_gl_error();

					//---------------------------------------------------------------------------------
					//CLEAR MATERIAL DATA
					glActiveTexture(GL_TEXTURE0 + 0);
					if (ObjMR->Material->MaterialInstanceData.AlbedoTexId)
						glBindTexture(GL_TEXTURE_2D, 0);

					glActiveTexture(GL_TEXTURE0 + 1);
					if (ObjMR->Material->MaterialInstanceData.RoughnessTexId)
						glBindTexture(GL_TEXTURE_2D, 0);

					glActiveTexture(GL_TEXTURE0 + 2);
					if (ObjMR->Material->MaterialInstanceData.MetalnessTexId)
						glBindTexture(GL_TEXTURE_2D, 0);

					glActiveTexture(GL_TEXTURE0 + 3);
					if (ObjMR->Material->MaterialInstanceData.NormalTexId)
						glBindTexture(GL_TEXTURE_2D, 0);

					glActiveTexture(GL_TEXTURE0 + 4);
					if (ObjMR->Material->MaterialInstanceData.AmbientOculusionTexId)
						glBindTexture(GL_TEXTURE_2D, 0);

					if (this->CurrentScene->Skybox) {
						glActiveTexture(GL_TEXTURE0 + 5);
						if (this->CurrentScene->Skybox->IrradienceMap)
							glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

						glActiveTexture(GL_TEXTURE0 + 6);
						if (this->CurrentScene->Skybox->PreFilterMap)
							glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

						glActiveTexture(GL_TEXTURE0 + 7);
						if (this->CurrentScene->Skybox->BRDF_2D_LUTMap)
							glBindTexture(GL_TEXTURE_2D, 0);
					}
				}
				ObjMR->Shader->UnUse();
				glDeleteBuffers(1, &InstanceABO);
				//---------------------------------------------------------------------------------
			}

		}
	}
}

void _NL::Engine::GameManager::RenderScreenQuad(_NL::Object::CameraObj* Cam) {
	check_gl_error();
	//if (Cam->HasPingPongShader && Cam->PingPongShader != NULL) {
	//	GLuint ppt = Cam->GeneratePingPongTexture();
	//	Cam->RenderToPostProcessingImage();
	//}
	//else {
	//	Cam->RenderToPostProcessingImage();
	//}
	Cam->RenderToPostProcessingFramebuffer();
}

void _NL::Engine::GameManager::RenderSceneCanvas() {
	for each (_NL::UI::UICanvas* UIC in UICanvas)
	{
		UIC->DrawElements();
		check_gl_error();
	}
}

void _NL::Engine::GameManager::GameTick() {
	
	GameTime.Tick();
}

_NL::Engine::GameManager::~GameManager()
{

}




