#include "WindowManager.h"

#define Classname_CameraObj "_NL::Object::CameraObj"
#define Classname_LightObject "_NL::Object::LightObject"

_NL::Engine::WindowManager::WindowManager(const char* WindowName, int Width, int height, bool fullscreen, bool bVSync, int fpsLimit)
{
	//INITIALZE WINDOW
	window = new sf::RenderWindow(sf::VideoMode(Width, height), WindowName);
	if (fullscreen) {
		window->create(sf::VideoMode::getFullscreenModes()[0], WindowName, sf::Style::Fullscreen);
	}
	window->setFramerateLimit(fpsLimit);
	window->setVerticalSyncEnabled(bVSync);
	window->setKeyRepeatEnabled(false);
	glewInit();
}	

void _NL::Engine::WindowManager::RunProgramLoop()
{
	Start();
	
	while (window->isOpen()) {
		while (window->pollEvent(Event))
		{
			//======================
			if (Event.type == Event.Closed) {
				window->close();
			}
		}	
		//======================
		//UPDATE SCRIPTS 
		for each (_NL::Core::Object* obj in CurrentScene->GetObjectList())
		{
			UpdateScriptsOfObj(obj);
		}
		//======================
		//UPDATE WINDOW 
		updateWindow();
	}
}

void _NL::Engine::WindowManager::UpdateScriptsOfObj(_NL::Core::Object* obj) {
	for each (_NL::Component::Script<_NL::Core::CppScript>* s in obj->Components)
	{
		if (s->ClassName() == "_NL::Component::Script") {
			s->getScript()->Update();
		}
	}
}

void _NL::Engine::WindowManager::StartScriptsOfObj(_NL::Core::Object* obj) {
	for each (_NL::Component::Script<_NL::Core::CppScript>* s in obj->Components)
	{
		if (s->ClassName() == "_NL::Component::Script") {
			s->getScript()->Start();
		}
	}
}

void _NL::Engine::WindowManager::GenerateCamFramebuffers(std::vector<_NL::Object::CameraObj*> Cams)
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
				GL_RGBA,
				Cam->Settings.RenderWindowSize.x * Cam->Settings.RenderScaleRatio,
				Cam->Settings.RenderWindowSize.y *  Cam->Settings.RenderScaleRatio,
				0,
				GL_RGBA,
				GL_UNSIGNED_BYTE,
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

void _NL::Engine::WindowManager::OpenGLStart()
{
	//======================
	//SCREEN QUAD
	ScreenShader.installShaders("screenQuadVshader.glsl", "screenQuadFshader.glsl");

	//======================
	Cameras.clear();
	Lights.clear();
	//ACTION !

	//======================
	//GET OBJECTS
	for each (_NL::Core::Object* obj in CurrentScene->GetObjectList())
	{
		//======================
		_NL::Component::MeshRenderer* MR = obj->getComponent<_NL::Component::MeshRenderer>();
		if (MR) {
			std::cout << "initialize MeshRenderer of Obj: " << obj->name << std::endl;
			MR->initGLObj();
		}
		//======================
		if (obj->ClassName() == Classname_CameraObj) {
			Cameras.push_back((_NL::Object::CameraObj*)obj);
		}
		//======================
		if (obj->ClassName() == Classname_LightObject) {
			Lights.push_back(dynamic_cast<_NL::Object::LightObject*>(obj)->LightProperties);
		}
	}

	GenerateCamFramebuffers(Cameras);

	//for each (_NL::Object::GameObject* obj in CurrentScene->GetObjectList())
	//{
	//		std::cout << "initGLObj: " << obj->name << std::endl;
	//		obj->getComponent(_NL::Component::MeshRenderer())->initGLObj();
	//		obj->StartScriptComponents(); 
	//	}
	//	
	//}
	//
	////GET CAMERAS
	//for each (_NL::Object::CameraObj* obj in CurrentScene->GetObjectList())
	//{
	//	if (obj->ClassName() == "_NL::Object::CameraObj") {
	//		Cameras.push_back(obj);
	//	}
	//}
	//
	//
	//
	////GET LIGHTS
	//for each (_NL::Object::LightObject* obj in CurrentScene->GetObjectList())
	//{
	//	if (obj->ClassName() == "_NL::Object::LightObject") {
	//		Lights.push_back(obj->LightProperties);
	//	}
	//}

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

void _NL::Engine::WindowManager::Start()
{
	for each (_NL::Core::Object* obj in CurrentScene->GetObjectList())
	{
		StartScriptsOfObj(obj);
	}
	OpenGLStart();
}

void _NL::Engine::WindowManager::UpdateCurrentScene() {
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

		glBindBuffer(GL_UNIFORM_BUFFER, LightsBlockUBO);
		glBufferSubData(GL_UNIFORM_BUFFER,
			0,
			sizeof(_NL::Core::LightProperties)*Lights.size(),
			&Lights[0]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		check_gl_error();

		//======================
		//SKYBOX RENDERING
		if (CurrentScene->Skybox != 0) {
			glUseProgram(CurrentScene->Skybox->SkyboxShader->getShaderProgram());
			glUniformMatrix4fv(CurrentScene->Skybox->CamProjection_matrix, 1, GL_FALSE, glm::value_ptr(Cam->projectionMatrix));
			glUniformMatrix4fv(CurrentScene->Skybox->CamView_matrix, 1, GL_FALSE, glm::value_ptr(Cam->getViewMatrix()));
			CurrentScene->Skybox->RenderSkybox();
			check_gl_error_full();
		}

		for each (_NL::Core::Object* obj in CurrentScene->GetObjectList())
		{
			//======================
			//COMPONENTS

			_NL::Component::MeshRenderer* ObjMR = obj->getComponent<_NL::Component::MeshRenderer>();

			//======================
			//PARENTING

			_NL::Component::Transform* ObjT = obj->getComponent<_NL::Component::Transform>();
			_NL::Component::Transform* ObjT_P;

			if (obj->Parent != 0 && ObjT) {
				ObjT_P = obj->Parent->getComponent<_NL::Component::Transform>();
			}
			else {
				ObjT_P = new _NL::Component::Transform();
			}

			//======================
			//DEBUG
			//======================
		
			//OBJ RENDERING
			if (ObjMR != NULL && ObjT != NULL && ObjT_P != NULL) {
				
				glUseProgram(ObjMR->Shader->getShaderProgram());
				
				///INLINE PARENTING glm::scale(glm::scale(glm::rotate(glm::rotate(glm::translate(glm::translate(Cam->projectionMatrix*Cam->getWorldToViewMatrix(), ObjT_P->transform.position), ObjT->transform.position), ObjT_P->transform.rotationAngle, ObjT_P->transform.rotationAxis), ObjT->transform.rotationAngle, ObjT->transform.rotationAxis), ObjT_P->transform.scale), ObjT->transform.scale))

				glm::mat4 T = glm::translate(glm::mat4(), ObjT->transform.position) * glm::translate(glm::mat4(), ObjT_P->transform.position);
				glm::mat4 R = ObjT->transform.MatrixRotation * ObjT_P->transform.MatrixRotation;
				glm::mat4 S = glm::scale(glm::mat4(), ObjT->transform.scale) * glm::scale(glm::mat4(), ObjT_P->transform.scale);
				glm::mat4 Modelmat = T * R * S;
				
				//glUniformMatrix4fv(ObjMR->ModelMatrix_atrib, 1, GL_FALSE, glm::value_ptr(glm::scale(glm::scale(ObjT->transform.MatrixRotation * ObjT_P->transform.MatrixRotation * (glm::translate(glm::translate(glm::mat4(), ObjT_P->transform.position), ObjT->transform.position)), ObjT_P->transform.scale), ObjT->transform.scale)));
				glUniformMatrix4fv(ObjMR->ModelMatrix_atrib, 1, GL_FALSE, glm::value_ptr(Modelmat));
				glUniformMatrix4fv(ObjMR->ViewMatrix_atrib, 1, GL_FALSE, glm::value_ptr(Cam->getWorldToViewMatrix()));
				glUniformMatrix4fv(ObjMR->ProjectionMatrix_atrib, 1, GL_FALSE, glm::value_ptr(Cam->projectionMatrix));
				glBindVertexArray(ObjMR->vao);
				///RENDER
				//glDrawElements(
				//	GL_LINE_LOOP,
				//	ObjMR->Mesh->Indices.size() * 3,
				//	GL_UNSIGNED_INT,
				//	0
				//);
				glBindTexture(GL_TEXTURE_2D, ObjMR->Material->MTLTexIds[0]);
				glDrawArrays(GL_TRIANGLES, 0, ObjMR->Mesh->Indices.size() * 3);
				glUseProgram(0);
				glBindVertexArray(0);
			}
		}
		//======================
		//DISPLAY && CLEAR;

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		ClearCurrentBuffer();
		Display(CamID);
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

void _NL::Engine::WindowManager::DrawScreenQuad(GLuint CamID)
{
	
	glViewport(Cameras[CamID]->Settings.RenderWindowPos.x, Cameras[CamID]->Settings.RenderWindowPos.y, Cameras[CamID]->Settings.RenderWindowSize.x, Cameras[CamID]->Settings.RenderWindowSize.y);
	//glViewport(0, 0, window->getSize().x, window->getSize().y);
	
	ClearCurrentBuffer();

	_NL::Core::ScreenQuad q;
	glUseProgram(ScreenShader.InstlledProgramIDs[0]);

	GLuint uScreenQuadTexture = glGetUniformLocation(ScreenShader.InstlledProgramIDs[0], "fbo_texture");
	GLuint aScreenQuadTexCoords = glGetAttribLocation(ScreenShader.InstlledProgramIDs[0], "texCoords");
	glUniform1i(uScreenQuadTexture, 0);
	check_gl_error();

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	glEnableVertexAttribArray(aScreenQuadTexCoords);
	glVertexAttribPointer(aScreenQuadTexCoords, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)q.fullquad_t);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (GLvoid*)q.fullquad_i);
	glDisableVertexAttribArray(aScreenQuadTexCoords);

	glUseProgram(0);
	
}

void _NL::Engine::WindowManager::ClearCurrentBuffer()
{
	//======================
	//CLEAR BUFFER_BIT
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void _NL::Engine::WindowManager::Display(GLuint CamID) {
	
	//======================
	//DISPLAY
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, ColorTexture[CamID]);
	DrawScreenQuad(CamID);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
	window->display();	

}

void _NL::Engine::WindowManager::updateWindow() {
	
	//======================
	//UPDATE; 
	check_gl_error();
	UpdateCurrentScene();	Time.Tick();
}

_NL::Engine::WindowManager::~WindowManager()
{
}




