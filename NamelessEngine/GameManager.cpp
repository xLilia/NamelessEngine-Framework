#include "GameManager.h"

_NL::Engine::GameManager::GameManager(const char* WindowName, int Width, int height, bool fullscreen, bool bVSync, int fpsLimit)
{
	//---------------------------------------------------------------------------------
	//WINDOW INITIALIZATION
	//---------------------------------------------------------------------------------
	window = new sf::RenderWindow(sf::VideoMode(Width, height), WindowName);
	if (fullscreen) {
		window->create(sf::VideoMode::getFullscreenModes()[0], WindowName, sf::Style::Fullscreen);
	}
	window->setFramerateLimit(fpsLimit);
	window->setVerticalSyncEnabled(bVSync);
	window->setKeyRepeatEnabled(false);
	glewInit();
	//_toggle_gl_debug();
	//---------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------
}	

void _NL::Engine::GameManager::RunScene(_NL::Engine::WorldSpace* set_current_scene)
{
	this->CurrentScene = set_current_scene;

	OpenGLStart();

	while (window->isOpen()) {

		while (window->pollEvent(Event));
		{
			//PROCESS INPUT STACK
		}	
		
		//---------------------------------------------------------------------------------
		//START & UPDATE SCRIPTS 
		for(int obj = 0; obj < CurrentScene->ObjectList.size(); obj++)
		//for each (std::vector<_NL::Core::Object*> id in CurrentScene->ObjectList)
		{
			for(int inst = 0; inst < CurrentScene->ObjectList[obj].size(); inst++)
			//for each (_NL::Core::Object* obj in id)
			{
				StartUpdateScriptsOfObj(CurrentScene->ObjectList[obj][inst]);
			}
		}

		//---------------------------------------------------------------------------------
		//PARTICLE SYSTEMS
		UpdateParticleSystems();

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

//---------------------------------------------------------------------------------
//END OF CURRENT SCENE
//---------------------------------------------------------------------------------

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
	for each (std::vector<_NL::Core::Object*> obj in CurrentScene->ObjectList)
	{
		for each (_NL::Core::Object* inst in obj)
		{
			EndScriptsOfObj(inst);
		}
	}
	Cameras.clear();
	LightsProperties.clear();
	Lights.clear();
	UICanvas.clear();
	ParticleSystems.clear();
	glDeleteBuffers(1, &LightsBlockUBO);
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//SCRIPTING UPDATES
//---------------------------------------------------------------------------------

void _NL::Engine::GameManager::StartUpdateScriptsOfObj(_NL::Core::Object* obj) {
	for each (_NL::Component::CppScript<_NL::Core::Script>* s in obj->Components)
	{
		if (s->ClassName() == "_NL::Component::CppScript") {
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
		if (s->ClassName() == "_NL::Component::CppScript") {
			s->getScript()->End();
		}

	}
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

void _NL::Engine::GameManager::OpenGLStart()
{
	check_gl_error();
	
	//GET OBJECTS
	for each (std::vector<_NL::Core::Object*> obj in CurrentScene->ObjectList)
	{
		//---------------------------------------------------------------------------------
		for each (_NL::Core::Object* inst in obj)
		{
			_NL::Component::MeshRenderer* MR = inst->getComponent<_NL::Component::MeshRenderer>();
			if (MR != nullptr) {
				if (!MR->bIsUnpacked) {
					std::cout << "initialize MeshRenderer of Obj: " << inst->name << std::endl;
					MR->initGLObj();
				}
			}
			//---------------------------------------------------------------------------------
			if (inst->ClassName() == "_NL::Object::CameraObj") {
				Cameras.push_back((_NL::Object::CameraObj*)inst);
			}
			//---------------------------------------------------------------------------------
			if (inst->ClassName() == "_NL::Object::LightObject") {
				_NL::Object::LightObject* L = dynamic_cast<_NL::Object::LightObject*>(inst);
				L->GenerateFramebuffer(SHADOW_WIDTH, SHADOW_HEIGHT);
				LightsProperties.push_back(L->LightProperties);
				Lights.push_back(L);
			}
			//---------------------------------------------------------------------------------
			if (inst->ClassName() == "_NL::UI::UICanvas") {
				UICanvas.push_back(dynamic_cast<_NL::UI::UICanvas*>(inst));
			}
			//---------------------------------------------------------------------------------
			if (inst->ClassName() == "_NL::Object::ParticleSystem") {
				ParticleSystems.push_back(dynamic_cast<_NL::Object::ParticleSystem*>(inst));
			}
		}
	}

	//---------------------------------------------------------------------------------
	if (Cameras.size() == 0) {
		std::cout << "Camera not found in Current Scene..." << std::endl;
	}
	else {
		for each (_NL::Object::CameraObj* Cam in Cameras)
		{
			Cam->GenerateFrameBuffers();
		}
	}

	//---------------------------------------------------------------------------------
	//INITIALIZE LIGHT UBO
	if (LightsProperties.size() > 0) {

		glGenBuffers(1, &LightsBlockUBO);
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, LightsBlockUBO);
		glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(_NL::Core::LightProperties)*LightsProperties.size(), &LightsProperties[0], GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, uIndexLightsBlock, LightsBlockUBO);
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, uIndexLightsBlock, LightsBlockUBO, 0, sizeof(_NL::Core::LightProperties)*LightsProperties.size());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
		check_gl_error();

		//Generate Light's Framebuffers

		check_gl_error();
	}

	//---------------------------------------------------------------------------------
	//GL SETTINGS
	{
		///GL OPTIONS
		
		glShadeModel(GL_SMOOTH);
		//glDepthMask(GL_TRUE);
		//glDepthFunc(GL_LEQUAL);
		//glDepthRange(0.0f, 1.0f);
		///FACE CULLIG
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		//glFrontFace(GL_CCW);
		///RENDER
		glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

void _NL::Engine::GameManager::RenderCurrentScene() {
	int camID = 0;
	camID++;
	//---------------------------------------------------------------------------------
	//Update Lights
	UpdateSceneLights();
	for each (_NL::Object::CameraObj* Cam in Cameras)
	{	
		//---------------------------------------------------------------------------------
		//UPDATE CAM
 		Cam->PrepareToRenderScene();

		//---------------------------------------------------------------------------------
		//SKYBOX RENDERING
		RenderSceneSkybox(Cam->getViewMatrix(), Cam->getProjectionMatrix());

		//---------------------------------------------------------------------------------
		//SCENE RENDERING
		RenderSceneObjects(Cam->TransformCam.Position, Cam->getWorldToViewMatrix(), Cam->getProjectionMatrix());

		//---------------------------------------------------------------------------------
		//RENDER TO SCREEN;
		RenderScreenQuad(Cam);
	}

	RenderSceneCanvas();
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

	if (LightsProperties.size() > 0) {
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, LightsBlockUBO);
		glBufferSubData(GL_SHADER_STORAGE_BUFFER,
			0,
			sizeof(_NL::Core::LightProperties)*LightsProperties.size(),
			&LightsProperties[0]);
		glBindBufferRange(GL_SHADER_STORAGE_BUFFER, uIndexLightsBlock, LightsBlockUBO, 0, sizeof(_NL::Core::LightProperties)*LightsProperties.size());
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	}
}

void _NL::Engine::GameManager::UpdateParticleSystems() {
	if (ParticleSystems.size() > 0) {
		for each (_NL::Object::ParticleSystem* PS in ParticleSystems)
		{
			PS->CurrentScene = this->CurrentScene;
			PS->TickSystem();
		}
	}
}

void _NL::Engine::GameManager::RenderSceneSkybox(glm::mat4 ViewMatrix, glm::mat4 ProjectionMatrix) {
	if (CurrentScene->Skybox != 0) {
		CurrentScene->Skybox->SkyboxDysplayShader->Use();
		glUniformMatrix4fv(CurrentScene->Skybox->CamProjectionMatrix_uniform, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
		glUniformMatrix4fv(CurrentScene->Skybox->CamViewMatrix_uniform, 1, GL_FALSE, glm::value_ptr(ViewMatrix));
		CurrentScene->Skybox->RenderSkybox();
		check_gl_error();
	}
}

void _NL::Engine::GameManager::RenderSceneObjects(glm::vec3 EyePos, glm::mat4 WorldToViewMatrix, glm::mat4 ProjectionMatrix, GLuint UseOverrideShaderProgram) {
	check_gl_error();
	for(int objN = 0; objN < CurrentScene->ObjectList.size(); objN++)
	//for each (std::vector<_NL::Core::Object*> obj in CurrentScene->ObjectList)
	{
		std::vector<_NL::Core::Object*>& obj = CurrentScene->ObjectList[objN];
		if (obj.size() > 0) {
			//MESH RENDERER
			_NL::Component::MeshRenderer* ObjMR = obj[0]->getComponent<_NL::Component::MeshRenderer>();

			if (ObjMR != NULL) {
				if (UseOverrideShaderProgram == NULL) {
					ObjMR->Shader->Use();
				}
				else {
					glUseProgram(UseOverrideShaderProgram);
				}
				
				glBindVertexArray(ObjMR->vao);

				//---------------------------------------------------------------------------------
				//TRANSFORMS
				std::vector<glm::mat4> Modelmat;

				for (int inst = 0; inst < obj.size(); inst++)
					//for each (_NL::Core::Object* objInstance in obj)
				{
					//---------------------------------------------------------------------------------
					//PARENTS
					_NL::Core::Object* objInstance = obj[inst];
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
				glUniformMatrix4fv(_NL::Core::ViewMatrix_uniform, 1, GL_FALSE, glm::value_ptr(WorldToViewMatrix));
				glUniformMatrix4fv(_NL::Core::ProjectionMatrix_uniform, 1, GL_FALSE, glm::value_ptr(ProjectionMatrix));
				glUniform3f(_NL::Core::EyePos_uniform, EyePos.x, EyePos.y, EyePos.z);
				check_gl_error();
			
				//---------------------------------------------------------------------------------
				//LIGHT UNIFORMS
				glUniform1i(_NL::Core::NumberOfLights_uniform, LightsProperties.size());
				check_gl_error();

				//---------------------------------------------------------------------------------
				//BIND INSTANCE ARRAY
				GLuint InstanceABO;
				glGenBuffers(1, &InstanceABO);
				glBindBuffer(GL_ARRAY_BUFFER, InstanceABO);
				glBufferData(GL_ARRAY_BUFFER, obj.size() * sizeof(glm::mat4), glm::value_ptr(Modelmat[0]), GL_STATIC_DRAW);

				//---------------------------------------------------------------------------------
				//SETUP INSTANCE MODEL MAT ATRIB

				GLsizei vec4Size = sizeof(glm::vec4);
				glEnableVertexAttribArray(_NL::Core::InstModelMatrix0_atrib + 0);
				glVertexAttribPointer(_NL::Core::InstModelMatrix0_atrib + 0, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(0 * vec4Size));
				glEnableVertexAttribArray(_NL::Core::InstModelMatrix0_atrib + 1);
				glVertexAttribPointer(_NL::Core::InstModelMatrix0_atrib + 1, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(1 * vec4Size));
				glEnableVertexAttribArray(_NL::Core::InstModelMatrix0_atrib + 2);
				glVertexAttribPointer(_NL::Core::InstModelMatrix0_atrib + 2, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
				glEnableVertexAttribArray(_NL::Core::InstModelMatrix0_atrib + 3);
				glVertexAttribPointer(_NL::Core::InstModelMatrix0_atrib + 3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));

				glVertexAttribDivisor(_NL::Core::InstModelMatrix0_atrib + 0, 1);
				glVertexAttribDivisor(_NL::Core::InstModelMatrix0_atrib + 1, 1);
				glVertexAttribDivisor(_NL::Core::InstModelMatrix0_atrib + 2, 1);
				glVertexAttribDivisor(_NL::Core::InstModelMatrix0_atrib + 3, 1);

				check_gl_error();
				if (ObjMR->Material->MaterialInstanceData.size()>0)
					for (int i = 0; i < ObjMR->Material->MaterialInstanceData.size(); i++)
					{
						check_gl_error();
						//---------------------------------------------------------------------------------
						//SETUP MATERIAL UNIFORMS
						glUniform1i(_NL::Core::ALbedoTexture_uniform, 0);
						glUniform1i(_NL::Core::RoughnessTexture_uniform, 1);
						glUniform1i(_NL::Core::MetalnessTexture_uniform, 2);
						glUniform1i(_NL::Core::NormalTexture_uniform, 3);
						glUniform1i(_NL::Core::AmbientOculusionTexture_uniform, 4);
						glUniform1i(_NL::Core::AmbientIrradianceTexture_uniform, 5);
						glUniform1i(_NL::Core::PreFilterTexture_uniform, 6);
						glUniform1i(_NL::Core::BRDF2DLUTTexture_uniform, 7);
						check_gl_error();
						//---------------------------------------------------------------------------------
						//SEND MATERIAL DATA
						glActiveTexture(GL_TEXTURE0 + 0);
						if (ObjMR->Material->MaterialInstanceData[i].AlbedoTexId)
							glBindTexture(GL_TEXTURE_2D, ObjMR->Material->MaterialInstanceData[i].AlbedoTexId);

						glActiveTexture(GL_TEXTURE0 + 1);
						if (ObjMR->Material->MaterialInstanceData[i].RoughnessTexId)
							glBindTexture(GL_TEXTURE_2D, ObjMR->Material->MaterialInstanceData[i].RoughnessTexId);

						glActiveTexture(GL_TEXTURE0 + 2);
						if (ObjMR->Material->MaterialInstanceData[i].MetalnessTexId)
							glBindTexture(GL_TEXTURE_2D, ObjMR->Material->MaterialInstanceData[i].MetalnessTexId);

						glActiveTexture(GL_TEXTURE0 + 3);
						if (ObjMR->Material->MaterialInstanceData[i].NormalTexId)
							glBindTexture(GL_TEXTURE_2D, ObjMR->Material->MaterialInstanceData[i].NormalTexId);

						glActiveTexture(GL_TEXTURE0 + 4);
						if (ObjMR->Material->MaterialInstanceData[i].AmbientOculusionTexId)
							glBindTexture(GL_TEXTURE_2D, ObjMR->Material->MaterialInstanceData[i].AmbientOculusionTexId);

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

						glDrawArraysInstanced(ObjMR->GL_RenderMode, 0, ObjMR->Mesh->Indices.size() * 3, obj.size());
						check_gl_error();

						//---------------------------------------------------------------------------------
						//CLEAR MATERIAL DATA
						glActiveTexture(GL_TEXTURE0 + 0);
						if (ObjMR->Material->MaterialInstanceData[i].AlbedoTexId)
							glBindTexture(GL_TEXTURE_2D, 0);

						glActiveTexture(GL_TEXTURE0 + 1);
						if (ObjMR->Material->MaterialInstanceData[i].RoughnessTexId)
							glBindTexture(GL_TEXTURE_2D, 0);

						glActiveTexture(GL_TEXTURE0 + 2);
						if (ObjMR->Material->MaterialInstanceData[i].MetalnessTexId)
							glBindTexture(GL_TEXTURE_2D, 0);

						glActiveTexture(GL_TEXTURE0 + 3);
						if (ObjMR->Material->MaterialInstanceData[i].NormalTexId)
							glBindTexture(GL_TEXTURE_2D, 0);

						glActiveTexture(GL_TEXTURE0 + 4);
						if (ObjMR->Material->MaterialInstanceData[i].AmbientOculusionTexId)
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

		}else {
			CurrentScene->ObjectList.erase(CurrentScene->ObjectList.begin() + objN);
		}
	}
}

void _NL::Engine::GameManager::RenderScreenQuad(_NL::Object::CameraObj* Cam) {
	check_gl_error();
	if (Cam->HasPingPongShader && Cam->PingPongShader != NULL) {
		GLuint ppt = Cam->GeneratePingPongTexture();
		Cam->DisplayOnScreen(&ppt);
	}
	else {
		Cam->DisplayOnScreen();
	}

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




