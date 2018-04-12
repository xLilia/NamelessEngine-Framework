#include "GameManager.h"

#define Classname_CameraObj "_NL::Object::CameraObj"
#define Classname_LightObject "_NL::Object::LightObject"

_NL::Engine::GameManager::GameManager(const char* WindowName, int Width, int height, bool fullscreen, bool bVSync, int fpsLimit)
{
	//INITIALZE WINDOW
	window = new sf::RenderWindow(sf::VideoMode(Width, height), WindowName);
	if (fullscreen) {
		window->create(sf::VideoMode::getFullscreenModes()[0], WindowName, sf::Style::Fullscreen);
	}
	window->setFramerateLimit(fpsLimit);
	window->setVerticalSyncEnabled(bVSync);
	window->setKeyRepeatEnabled(false);
	sf::Keyboard::setVirtualKeyboardVisible(true);
	glewInit();
	_toggle_gl_debug();
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
		
		//======================
		//UPDATE SCRIPTS 
		for each (std::vector<_NL::Core::Object*>  id in CurrentScene->ObjectList)
		{
			for each (_NL::Core::Object* obj in id)
			{
				UpdateScriptsOfObj(obj);
			}
		}

		//======================
		//END CURRENT SCENE 
		if (bEndCurrentScene) {
			bEndCurrentScene = false;
			break;
		}
		
		//======================
		//UPDATE WINDOW 
		updateWindow();
	}

	CleanUpLastSceneLoadedResources();
}

void _NL::Engine::GameManager::EndCurrentScene()
{
	bEndCurrentScene = true;
}

void _NL::Engine::GameManager::CleanUpLastSceneLoadedResources()
{
	Cameras.clear();
	Lights.clear();
	glDeleteFramebuffers(1, FrameBuffer);
	glDeleteBuffers(1, &LightsBlockUBO);
	glDeleteTextures(1, ColorTexture);
	glDeleteTextures(1, DepthTexture);
	CurrentScene = 0;
}

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

void _NL::Engine::GameManager::GenerateCamFramebuffers(std::vector<_NL::Object::CameraObj*> Cams)
{
	//======================
	//INITIALIZE FRAMEBUFFER AND TEXTURES
	FrameBuffer = new GLuint[Cams.size()]{};
	ColorTexture = new GLuint[Cams.size()]{};
	DepthTexture = new GLuint[Cams.size()]{};

	int iCam = 0;
	for each (_NL::Object::CameraObj* Cam in Cams)
	{
		//======================
		//SETUP FRAME BUFFER OBJECTs
		//======================
		//======================
		//ColorTextures
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &ColorTexture[iCam]);
			glBindTexture(GL_TEXTURE_2D, ColorTexture[iCam]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA16F, //high dynamic range (HDR) 
				Cam->Settings.RenderWindowSize.x * Cam->Settings.RenderScaleRatio,
				Cam->Settings.RenderWindowSize.y *  Cam->Settings.RenderScaleRatio,
				0,
				GL_RGBA,
				GL_FLOAT,
				NULL);
			check_gl_error_full();
		}
		//======================
		//DepthTexture
		{
			glCreateTextures(GL_TEXTURE_2D, 1, &DepthTexture[iCam]);
			glBindTexture(GL_TEXTURE_2D, DepthTexture[iCam]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_DEPTH_COMPONENT,
				Cam->Settings.RenderWindowSize.x * Cam->Settings.RenderScaleRatio,
				Cam->Settings.RenderWindowSize.y *  Cam->Settings.RenderScaleRatio,
				0,
				GL_DEPTH_COMPONENT,
				GL_UNSIGNED_BYTE,
				NULL);

			check_gl_error();
		}

		//======================
		//RenderBuffers
		{
			//glCreateRenderbuffers(1, &ColorRenderBuffer[1]);
			//glBindRenderbuffer(GL_RENDERBUFFER, ColorRenderBuffer[1]);
			//glRenderbufferStorageMultisample(GL_RENDERBUFFER, 0, GL_RGBA32F, window->getSize().x, window->getSize().y);
			//glBindRenderbuffer(GL_RENDERBUFFER, 0);
			//check_gl_error();
			//
			//glCreateRenderbuffers(1, &DepthRenderBuffer[1]);
			//glBindRenderbuffer(GL_RENDERBUFFER, DepthRenderBuffer[1]);
			//glRenderbufferStorageMultisample(GL_RENDERBUFFER, 0, GL_DEPTH_COMPONENT, window->getSize().x, window->getSize().y);
			//glBindRenderbuffer(GL_RENDERBUFFER, 0);
			//check_gl_error();

			//glCreateFramebuffers(1, &FrameBuffer[1]);
			//glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer[1]);
			//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, ColorRenderBuffer[1]);
			//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthRenderBuffer[1]);
			//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			//	check_gl_error();
			//}
			//glBindFramebuffer(GL_FRAMEBUFFER, 0);

			//glCreateRenderbuffers(1, &ColorRenderBuffer[0]);
			//glBindRenderbuffer(GL_RENDERBUFFER, ColorRenderBuffer[0]);
			//glRenderbufferStorageMultisample(
			//	GL_RENDERBUFFER,
			//	0, GL_RGBA,
			//	window->getSize().x,
			//	window->getSize().y);
			//
			//glBindRenderbuffer(GL_RENDERBUFFER, 0);
			//check_gl_error();
			//
			//glCreateRenderbuffers(1, &DepthRenderBuffer[0]);
			//glBindRenderbuffer(GL_RENDERBUFFER, DepthRenderBuffer[0]);
			//glRenderbufferStorageMultisample(
			//	GL_RENDERBUFFER,
			//	0,
			//	GL_DEPTH_COMPONENT,
			//	window->getSize().x,
			//	window->getSize().y);
			//
			//glBindRenderbuffer(GL_RENDERBUFFER, 0);
			//check_gl_error();
		}


		//FrameBuffers
		{
			glCreateFramebuffers(1, &FrameBuffer[iCam]);
			glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer[iCam]);
			//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, ColorRenderBuffer[0]);
			//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthRenderBuffer[0]);
			glFramebufferTexture2D(
				GL_FRAMEBUFFER,
				GL_COLOR_ATTACHMENT0,
				GL_TEXTURE_2D,
				ColorTexture[iCam],
				0);
			glFramebufferTexture2D(
				GL_FRAMEBUFFER,
				GL_DEPTH_ATTACHMENT,
				GL_TEXTURE_2D,
				DepthTexture[iCam],
				0);
			check_gl_error();
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
				check_gl_error();
			}
			//glDrawBuffer(GL_NONE);
			//glReadBuffer(GL_NONE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			check_gl_error();
			iCam++;
		}
	}
}

void _NL::Engine::GameManager::OpenGLStart()
{
	check_gl_error_full();

	//======================
	//SCREEN QUAD
	if(ScreenShader.InstlledProgramIDs.size() == 0) //If not installed Install DEFAULT OR CUSTOM
		ScreenShader.installShaders("screenQuadVshader.glsl", "screenQuadFshader.glsl");

	//======================
	//GET OBJECTS
	for each (std::vector<_NL::Core::Object*> obj in CurrentScene->ObjectList)
	{
		//======================
		_NL::Component::MeshRenderer* MR = obj[0]->getComponent<_NL::Component::MeshRenderer>();
		if (MR) {
			if (!MR->bIsUnpacked) {
				std::cout << "initialize MeshRenderer of Obj: " << obj[0]->name << std::endl;
				MR->initGLObj();
			}
		}
		//======================
		if (obj[0]->ClassName() == Classname_CameraObj) {
			Cameras.push_back((_NL::Object::CameraObj*)obj[0]);
		}
		//======================
		if (obj[0]->ClassName() == Classname_LightObject) {
			Lights.push_back(dynamic_cast<_NL::Object::LightObject*>(obj[0])->LightProperties);
		}
	}

	//======================
	if (Cameras.size() == 0) {
		std::cout << "Camera not found in Current Scene..." << std::endl;
		ClearCurrentBuffer();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		ClearCurrentBuffer();
	}
	else {
		GenerateCamFramebuffers(Cameras);
	}

	//======================
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

	//======================
	//GL SETTINGS
	{
		///DEPTH TEST
		glEnable(GL_DEPTH_TEST);
		glShadeModel(GL_SMOOTH);
		//glDepthMask(GL_TRUE);
		//glDepthFunc(GL_LEQUAL);
		//glDepthRange(0.0f, 1.0f);
		///FACE CULLIG
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_BACK);
		//glFrontFace(GL_CCW);
		///BLENDING
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

void _NL::Engine::GameManager::Start()
{
	check_gl_error_full();
	for each (std::vector<_NL::Core::Object*> obj in CurrentScene->ObjectList)
	{
		StartScriptsOfObj(obj[0]);
	}
	OpenGLStart();
}

void _NL::Engine::GameManager::UpdateCurrentScene() {
	int CamID = 0;
	for each (_NL::Object::CameraObj* Cam in Cameras)
	{	
		//======================
		//SETUP FRAMEBUFFERS
		Cam->updateCameraProjectionMatrix();
		glViewport(Cam->Settings.RenderWindowPos.x, Cam->Settings.RenderWindowPos.y, Cam->Settings.RenderWindowSize.x * Cam->Settings.RenderScaleRatio, Cam->Settings.RenderWindowSize.y * Cam->Settings.RenderScaleRatio);
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer[CamID]);
		ClearCurrentBuffer();

		//======================
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
		
		//======================
		//SKYBOX RENDERING
		if (CurrentScene->Skybox != 0) {
			CurrentScene->Skybox->SkyboxShader->Use();
			glUniformMatrix4fv(CurrentScene->Skybox->CamProjectionMatrix_uniform, 1, GL_FALSE, glm::value_ptr(Cam->projectionMatrix));
			glUniformMatrix4fv(CurrentScene->Skybox->CamViewMatrix_uniform, 1, GL_FALSE, glm::value_ptr(Cam->getViewMatrix()));
			CurrentScene->Skybox->RenderSkybox();
			check_gl_error_full();
		}

		//======================
		//OBJ RENDERING
		check_gl_error();
		for each (std::vector<_NL::Core::Object*> obj in CurrentScene->ObjectList)
		{

			//======================
			//MESH RENDERER
			_NL::Component::MeshRenderer* ObjMR = obj[0]->getComponent<_NL::Component::MeshRenderer>();
			if (ObjMR != NULL) {
				ObjMR->Shader->Use();
				glBindVertexArray(ObjMR->vao);

				//======================
				//TRANSFORMS
				std::vector<glm::mat4> Modelmat;
				

				for each (_NL::Core::Object* objInstance in obj)
				{
					//======================
					//PARENTS
					_NL::Component::Transform* _ObjT_P;
					_NL::Component::Transform* _ObjT = objInstance->getComponent<_NL::Component::Transform>();

					if (objInstance->Parent != 0 && _ObjT != 0) {
						_ObjT_P = objInstance->Parent->getComponent<_NL::Component::Transform>();
					}
					else {
						_ObjT_P = new _NL::Component::Transform();
					}

					//======================
					//INSTANCE MODEL MAT
					glm::mat4 T = glm::translate(glm::mat4(), _ObjT->transform.position) * glm::translate(glm::mat4(), _ObjT_P->transform.position);
					glm::mat4 R = _ObjT->transform.RotationMatrix * _ObjT_P->transform.RotationMatrix;
					glm::mat4 S = glm::scale(glm::mat4(), _ObjT->transform.scale) * glm::scale(glm::mat4(), _ObjT_P->transform.scale);
					glm::mat4 _Modelmat = T * R * S;
					
					delete _ObjT_P;
					Modelmat.push_back(_Modelmat);
				}

				

				//======================
				//CAM UNIFORMS
				//glUniformMatrix4fv(ObjMR->ModelMatrix_uniform, 1, GL_FALSE, glm::value_ptr(glm::scale(glm::scale(ObjT->transform.RotationMatrix * ObjT_P->transform.RotationMatrix * (glm::translate(glm::translate(glm::mat4(), ObjT_P->transform.position), ObjT->transform.position)), ObjT_P->transform.scale), ObjT->transform.scale)));
				//glUniformMatrix4fv(_NL::Core::ModelMatrix_uniform, 1, GL_FALSE, glm::value_ptr(*Modelmat));
				check_gl_error();
				glUniformMatrix4fv(_NL::Core::ViewMatrix_uniform, 1, GL_FALSE, glm::value_ptr(Cam->getWorldToViewMatrix()));
				check_gl_error();
				glUniformMatrix4fv(_NL::Core::ProjectionMatrix_uniform, 1, GL_FALSE, glm::value_ptr(Cam->projectionMatrix));
				glUniform3f(_NL::Core::EyePos_uniform, Cam->Transform.position.x, Cam->Transform.position.y, Cam->Transform.position.z);
				check_gl_error();

				//======================
				//BIND INSTANCE ARRAY
				GLuint InstanceABO;
				glGenBuffers(1, &InstanceABO);
				glBindBuffer(GL_ARRAY_BUFFER, InstanceABO);
				glBufferData(GL_ARRAY_BUFFER, obj.size() * sizeof(glm::mat4), glm::value_ptr(Modelmat[0]), GL_STATIC_DRAW);

				//======================
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
						//======================
						//SETUP MATERIAL UNIFORMS
						glUniform1i(_NL::Core::ALbedoTexture_uniform, 0);
						glUniform1i(_NL::Core::RoughnessTexture_uniform, 1);
						glUniform1i(_NL::Core::MetalnessTexture_uniform, 2);
						glUniform1i(_NL::Core::NormalTexture_uniform, 3);
						glUniform1i(_NL::Core::AmbientOculusionTexture_uniform, 4);
						glUniform1i(_NL::Core::AmbientIrradianceTexture_uniform, 5);
						check_gl_error();
						//======================
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
						check_gl_error();
						//======================
						//DRAW MESH 
						glDrawArraysInstanced(GL_TRIANGLES, 0, ObjMR->Mesh->Indices.size() * 3, obj.size());
						check_gl_error();
					}
				glDeleteBuffers(1, &InstanceABO);
				glUseProgram(0);
				glBindVertexArray(0);
			}
		}
		//======================
		//DISPLAY && CLEAR;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		ClearCurrentBuffer();
		RenderToScreen(CamID);
		CamID++;

	}
	//======================
	////AFTER DRAWING CAM VIEW for both buffers
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//ClearCurrentBuffer();
	//CamID = 0;
	//for each (_NL::Object::CameraObj* Cam in Cameras)
	//{
	//	Display(CamID);
	//	CamID++;
	//}
}

void _NL::Engine::GameManager::DrawScreenQuad(GLuint CamID)
{
	glViewport(Cameras[CamID]->Settings.RenderWindowPos.x, Cameras[CamID]->Settings.RenderWindowPos.y, Cameras[CamID]->Settings.RenderWindowSize.x, Cameras[CamID]->Settings.RenderWindowSize.y);
	//glViewport(0, 0, window->getSize().x, window->getSize().y);
	
	ClearCurrentBuffer();

	_NL::Core::ScreenQuad q;
	glUseProgram(ScreenShader.InstlledProgramIDs[0]);

	GLuint uScreenQuadTexture = /* this->QuadTexture_uniform;//*/glGetUniformLocation(ScreenShader.InstlledProgramIDs[0], "fbo_texture");
	glUniform1i(uScreenQuadTexture, 0);
	GLuint aScreenQuadTexCoords =/* this->QuadTexCoords_atrib;//*/glGetAttribLocation(ScreenShader.InstlledProgramIDs[0], "texCoords");

	check_gl_error();

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glEnableVertexAttribArray(aScreenQuadTexCoords);
	glVertexAttribPointer(aScreenQuadTexCoords, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)q.fullquad_t);
	
	///Uniforms
	glUniform1f(uRenderExposure, RenderExposure);
	glUniform1f(uRenderGamma,RenderGamma);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (GLvoid*)q.fullquad_i);
	glDisableVertexAttribArray(aScreenQuadTexCoords);

	glUseProgram(0);
	check_gl_error();
}

void _NL::Engine::GameManager::ClearCurrentBuffer()
{
	//======================
	//CLEAR BUFFER_BIT
	glClearColor(ClearColor.x, ClearColor.y, ClearColor.z, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void _NL::Engine::GameManager::RenderToScreen(GLuint CamID) {
	
	//======================
	//DISPLAY
	
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ColorTexture[CamID]);
	DrawScreenQuad(CamID); //add custom shader func
	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(0);
	glEnable(GL_DEPTH_TEST);
	window->display();	

}

void _NL::Engine::GameManager::updateWindow() {
	
	//======================
	//UPDATE; 
	if (Cameras.size() > 0) {
		UpdateCurrentScene();
		//check_gl_error();
	}
	Time.Tick();
}

_NL::Engine::GameManager::~GameManager()
{
}




