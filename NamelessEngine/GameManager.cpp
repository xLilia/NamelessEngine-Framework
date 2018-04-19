#include "GameManager.h"

#define Classname_CameraObj "_NL::Object::CameraObj"
#define Classname_LightObject "_NL::Object::LightObject"

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
	sf::Keyboard::setVirtualKeyboardVisible(true);
	glewInit();
	//_toggle_gl_debug();
	//---------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------
}	

void _NL::Engine::GameManager::RunCurrentScene()
{
	
	Start();

	while (window->isOpen()) {
		check_gl_error();

		while (window->pollEvent(Event))
		{
			if (Event.type == Event.Closed) {
				window->close();
			}
		}	
		
		//---------------------------------------------------------------------------------
		//UPDATE SCRIPTS 
		for each (std::vector<_NL::Core::Object*>  id in CurrentScene->ObjectList)
		{
			for each (_NL::Core::Object* obj in id)
			{
				UpdateScriptsOfObj(obj);
			}
		}

		//---------------------------------------------------------------------------------
		//END CURRENT SCENE 
		if (bEndCurrentScene) {
			bEndCurrentScene = false;
			break;
		}
		
		//---------------------------------------------------------------------------------
		//UPDATE WINDOW 
		GameTick();
	}

	CleanUpLastSceneLoadedResources();
}

//---------------------------------------------------------------------------------
//END OF CURRENT SCENE
//---------------------------------------------------------------------------------

void _NL::Engine::GameManager::EndCurrentScene()
{
	bEndCurrentScene = true;
}

void _NL::Engine::GameManager::CleanUpLastSceneLoadedResources()
{
	Cameras.clear();
	Lights.clear();
	glDeleteBuffers(1, &LightsBlockUBO);
	CurrentScene = 0;
}

//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

//---------------------------------------------------------------------------------
//SCRIPTING UPDATES
//---------------------------------------------------------------------------------

void _NL::Engine::GameManager::UpdateScriptsOfObj(_NL::Core::Object* obj) {
	for each (_NL::Component::Script<_NL::Core::CppScript>* s in obj->Components)
	{
		if (s->ClassName() == "_NL::Component::Script") {
			s->getScript()->Update();
		}
	}
}

void _NL::Engine::GameManager::StartScriptsOfObj(_NL::Core::Object* obj) {
	for each (_NL::Component::Script<_NL::Core::CppScript>* s in obj->Components)
	{
		if (s->ClassName() == "_NL::Component::Script") {
			s->getScript()->Start();
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
		check_gl_error();
		//---------------------------------------------------------------------------------
		_NL::Component::MeshRenderer* MR = obj[0]->getComponent<_NL::Component::MeshRenderer>();
		if (MR) {
			if (!MR->bIsUnpacked) {
				std::cout << "initialize MeshRenderer of Obj: " << obj[0]->name << std::endl;
				MR->initGLObj();
			}
		}
		//---------------------------------------------------------------------------------
		if (obj[0]->ClassName() == Classname_CameraObj) {
			Cameras.push_back((_NL::Object::CameraObj*)obj[0]);
		}
		//---------------------------------------------------------------------------------
		if (obj[0]->ClassName() == Classname_LightObject) {
			Lights.push_back(dynamic_cast<_NL::Object::LightObject*>(obj[0])->LightProperties);
		}
		//---------------------------------------------------------------------------------
		if (obj[0]->ClassName() == "_NL::UI::UICanvas") {
			UICanvas.push_back(dynamic_cast<_NL::Core::UI*>(obj[0]));
		}
	}

	//---------------------------------------------------------------------------------
	if (Cameras.size() == 0) {
		std::cout << "Camera not found in Current Scene..." << std::endl;
		glBindFramebuffer(GL_FRAMEBUFFER, 0);;
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
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_SCISSOR_TEST);
		//glShadeModel(GL_SMOOTH);
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

void _NL::Engine::GameManager::Start()
{
	check_gl_error();
	for each (std::vector<_NL::Core::Object*> obj in CurrentScene->ObjectList)
	{
		StartScriptsOfObj(obj[0]);
	}
	OpenGLStart();
}

void _NL::Engine::GameManager::UpdateCurrentScene() {
	int cami = 1;
	for each (_NL::Object::CameraObj* Cam in Cameras)
	{	
		//---------------------------------------------------------------------------------
		//UPDATE CAM
		check_gl_error();
 		Cam->PrepareToRenderScene();
		//---------------------------------------------------------------------------------
		//Update Light ubo;
		if (Lights.size() > 0) {
			glBindBuffer(GL_UNIFORM_BUFFER, LightsBlockUBO);
			glBufferSubData(GL_UNIFORM_BUFFER,
				0,
				sizeof(_NL::Core::LightProperties)*Lights.size(),
				&Lights[0]);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
			check_gl_error();
		}
		
		//---------------------------------------------------------------------------------
		//SKYBOX RENDERING
		if (CurrentScene->Skybox != 0) {
			CurrentScene->Skybox->SkyboxDysplayShader->Use();
			glUniformMatrix4fv(CurrentScene->Skybox->CamProjectionMatrix_uniform, 1, GL_FALSE, glm::value_ptr(Cam->getProjectionMatrix()));
			glUniformMatrix4fv(CurrentScene->Skybox->CamViewMatrix_uniform, 1, GL_FALSE, glm::value_ptr(Cam->getViewMatrix()));
			CurrentScene->Skybox->RenderSkybox();
			check_gl_error();
		}

		//---------------------------------------------------------------------------------
		//SCENE RENDERING
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

					if (objInstance->Parent != 0 && _ObjT != 0) {
						_ObjT_P = objInstance->Parent->getComponent<_NL::Component::Transform>();
					}
					else {
						_ObjT_P = new _NL::Component::Transform();
					}

					//---------------------------------------------------------------------------------
					//INSTANCE MODEL MAT
					glm::mat4 T = glm::translate(glm::mat4(), _ObjT->transform.position) * glm::translate(glm::mat4(), _ObjT_P->transform.position);
					
					glm::mat4 T_RotationMatrix = glm::toMat4(_ObjT->transform.QuaternionRotation); 
					glm::mat4 TP_RotationMatrix = glm::toMat4(_ObjT_P->transform.QuaternionRotation);
					glm::mat4 R = T_RotationMatrix * TP_RotationMatrix;

					glm::mat4 S = glm::scale(glm::mat4(), _ObjT->transform.scale) * glm::scale(glm::mat4(), _ObjT_P->transform.scale);
					glm::mat4 _Modelmat = T * R * S;
					
					delete _ObjT_P;
					Modelmat.push_back(_Modelmat);
				}

				//---------------------------------------------------------------------------------
				//CAM UNIFORMS
				//glUniformMatrix4fv(ObjMR->ModelMatrix_uniform, 1, GL_FALSE, glm::value_ptr(glm::scale(glm::scale(ObjT->transform.RotationMatrix * ObjT_P->transform.RotationMatrix * (glm::translate(glm::translate(glm::mat4(), ObjT_P->transform.position), ObjT->transform.position)), ObjT_P->transform.scale), ObjT->transform.scale)));
				//glUniformMatrix4fv(_NL::Core::ModelMatrix_uniform, 1, GL_FALSE, glm::value_ptr(*Modelmat));
				check_gl_error();
				glUniformMatrix4fv(_NL::Core::ViewMatrix_uniform, 1, GL_FALSE, glm::value_ptr(Cam->getWorldToViewMatrix()));
				check_gl_error();
				glUniformMatrix4fv(_NL::Core::ProjectionMatrix_uniform, 1, GL_FALSE, glm::value_ptr(Cam->getProjectionMatrix()));
				glUniform3f(_NL::Core::EyePos_uniform, Cam->Position.x, Cam->Position.y, Cam->Position.z);
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
						glBindTexture(GL_TEXTURE_2D, ObjMR->Material->MaterialInstanceData[i].AlbedoTexId);
						glActiveTexture(GL_TEXTURE0 + 1);
						glBindTexture(GL_TEXTURE_2D, ObjMR->Material->MaterialInstanceData[i].RoughnessTexId);
						glActiveTexture(GL_TEXTURE0 + 2);
						glBindTexture(GL_TEXTURE_2D, ObjMR->Material->MaterialInstanceData[i].MetalnessTexId);
						glActiveTexture(GL_TEXTURE0 + 3);
						glBindTexture(GL_TEXTURE_2D, ObjMR->Material->MaterialInstanceData[i].NormalTexId);
						glActiveTexture(GL_TEXTURE0 + 4);
						glBindTexture(GL_TEXTURE_2D, ObjMR->Material->MaterialInstanceData[i].AmbientOculusionTexId);
						glActiveTexture(GL_TEXTURE0 + 5);
						glBindTexture(GL_TEXTURE_CUBE_MAP, this->CurrentScene->Skybox->IrradienceMap);
						glActiveTexture(GL_TEXTURE0 + 6);
						glBindTexture(GL_TEXTURE_CUBE_MAP, this->CurrentScene->Skybox->PreFilterMap);
						glActiveTexture(GL_TEXTURE0 + 7);
						glBindTexture(GL_TEXTURE_2D, this->CurrentScene->Skybox->BRDF_2D_LUTMap);
						check_gl_error();
						//---------------------------------------------------------------------------------
						//DRAW MESH 
						glDrawArraysInstanced(GL_TRIANGLES, 0, ObjMR->Mesh->Indices.size() * 3, obj.size());
						check_gl_error();
					}
				ObjMR->Shader->UnUse();
				glDeleteBuffers(1, &InstanceABO);
				//---------------------------------------------------------------------------------
			}
		}
		//---------------------------------------------------------------------------------
		//RENDER TO SCREEN;
		if (Cam->HasPingPongShader && Cam->PingPongShader != NULL) {
			GLuint ppt = Cam->GeneratePingPongTexture();
			Cam->DisplayOnScreen(cami, &ppt);
		}
		else {
			Cam->DisplayOnScreen(cami);
		}

		check_gl_error();
		cami++;
		
	}
	
	//UI Canvas Render
	for each (_NL::UI::UICanvas* UIC in UICanvas)
	{
		check_gl_error();
		UIC->DrawElements();
		check_gl_error();
	}
	
	window->display();
}

void _NL::Engine::GameManager::GameTick() {
	
	//---------------------------------------------------------------------------------
	//UPDATE; 
	if (Cameras.size() > 0) {
		UpdateCurrentScene();
		//check_gl_error();
	}
	GameTime.Tick();
	//std::cout << Time.Clock.getElapsedTime().asSeconds() << std::endl;
}

_NL::Engine::GameManager::~GameManager()
{

}




