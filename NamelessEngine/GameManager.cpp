#include "GameManager.h"

_NL::Engine::GameManager::GameManager(const char* WindowName, int Width, int height, bool fullscreen, bool bVSync, int fpsLimit)
{
	//---------------------------------------------------------------------------------
	//WINDOW INITIALIZATION
	//---------------------------------------------------------------------------------
	window = new sf::Window(sf::VideoMode(Width, height), WindowName);
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
		check_gl_error();

		while (window->pollEvent(Event))
		{
			if (Event.type == Event.Closed) {
				window->close();
			}
		}	
		
		//---------------------------------------------------------------------------------
		//START & UPDATE SCRIPTS 
		for each (std::vector<_NL::Core::Object*> id in CurrentScene->ObjectList)
		{
			for each (_NL::Core::Object* obj in id)
			{
				StartUpdateScriptsOfObj(obj);
			}
		}

		//---------------------------------------------------------------------------------
		//CHECK FOR END OF CURRENT SCENE 
		if (bEndCurrentScene) {
			bEndCurrentScene = false;
			break;
		}
		
		//---------------------------------------------------------------------------------
		//UPDATE WINDOW 
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
	Cameras.shrink_to_fit();
	Lights.clear();
	Lights.shrink_to_fit();
	UICanvas.clear();
	UICanvas.shrink_to_fit();
	ParticleSystems.clear();
	ParticleSystems.shrink_to_fit();
	glDeleteBuffers(1, &LightsBlockUBO);
	CurrentScene = 0;
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
			s->getScript()->_this = obj; // ducktape FIX!!!
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
				Lights.push_back(dynamic_cast<_NL::Object::LightObject*>(inst)->LightProperties);
			}
			//---------------------------------------------------------------------------------
			if (inst->ClassName() == "_NL::UI::UICanvas") {
				UICanvas.push_back(dynamic_cast<_NL::Core::UI*>(inst));
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
	if (Lights.size() > 0) {
		//Create Uniform Buffer
		glGenBuffers(1, &LightsBlockUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, LightsBlockUBO);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(_NL::Core::LightProperties)*Lights.size(), &Lights[0], GL_DYNAMIC_DRAW);
		//Bind Uniform Buffer base to index in program
		glBindBufferBase(GL_UNIFORM_BUFFER, uIndexLightsBlock, LightsBlockUBO);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		
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
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CCW);
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
		RenderSceneSkybox(Cam);

		//---------------------------------------------------------------------------------
		//PARTICLE SYSTEMS
		UpdateParticleSystems();

		//---------------------------------------------------------------------------------
		//SCENE RENDERING
		RenderSceneObjects(Cam);

		//---------------------------------------------------------------------------------
		//RENDER TO SCREEN;
		RenderScreenQuad(Cam);
	}
	RenderSceneCanvas();

	window->display();
}

void _NL::Engine::GameManager::UpdateSceneLights() {
	if (Lights.size() > 0) {
		glBindBuffer(GL_UNIFORM_BUFFER, LightsBlockUBO);
		glBufferSubData(GL_UNIFORM_BUFFER,
			0,
			sizeof(_NL::Core::LightProperties)*Lights.size(),
			&Lights[0]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		check_gl_error();
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

void _NL::Engine::GameManager::RenderSceneSkybox(_NL::Object::CameraObj* Cam) {
	if (CurrentScene->Skybox != 0) {
		CurrentScene->Skybox->SkyboxDysplayShader->Use();
		glUniformMatrix4fv(CurrentScene->Skybox->CamProjectionMatrix_uniform, 1, GL_FALSE, glm::value_ptr(Cam->getProjectionMatrix()));
		glUniformMatrix4fv(CurrentScene->Skybox->CamViewMatrix_uniform, 1, GL_FALSE, glm::value_ptr(Cam->getViewMatrix()));
		CurrentScene->Skybox->RenderSkybox();
		check_gl_error();
	}
}

void _NL::Engine::GameManager::RenderSceneObjects(_NL::Object::CameraObj* Cam) {
	check_gl_error();
	for each (std::vector<_NL::Core::Object*> obj in CurrentScene->ObjectList)
	{
		//---------------------------------------------------------------------------------
		//MESH RENDERER
		_NL::Component::MeshRenderer* ObjMR = obj[0]->getComponent<_NL::Component::MeshRenderer>();

		if (ObjMR != NULL) {
			ObjMR->Shader->Use();
			glBindVertexArray(ObjMR->vao);

			//---------------------------------------------------------------------------------
			//TRANSFORMS
			std::vector<glm::mat4> Modelmat;

			for each (_NL::Core::Object* objInstance in obj)
			{
				//---------------------------------------------------------------------------------
				//PARENTS
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
			glUniformMatrix4fv(_NL::Core::ViewMatrix_uniform, 1, GL_FALSE, glm::value_ptr(Cam->getWorldToViewMatrix()));
			glUniformMatrix4fv(_NL::Core::ProjectionMatrix_uniform, 1, GL_FALSE, glm::value_ptr(Cam->getProjectionMatrix()));
			glUniform3f(_NL::Core::EyePos_uniform, Cam->TransformCam.Position.x, Cam->TransformCam.Position.y, Cam->TransformCam.Position.z);
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
					glDrawArraysInstanced(GL_TRIANGLES, 0, ObjMR->Mesh->Indices.size() * 3, obj.size());
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
		
		UIC->DrawElements(window->getSize().y);
		check_gl_error();
	}
}

void _NL::Engine::GameManager::GameTick() {
	
	//---------------------------------------------------------------------------------
	//UPDATE; 
	if (Cameras.size() > 0) {
		RenderCurrentScene();
		//check_gl_error();
	}
	GameTime.Tick();
	//std::cout << Time.Clock.getElapsedTime().asSeconds() << std::endl;
}

_NL::Engine::GameManager::~GameManager()
{

}




