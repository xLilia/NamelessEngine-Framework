#include "WindowManager.h"

_NL::Engine::WindowManager::WindowManager(const char* WindowName, int Width, int height, bool fullscreen, bool bVSync, int fpsLimit, float renderScaleRatio)
{
	window = new sf::RenderWindow(sf::VideoMode(Width, height), WindowName);
	this->renderScaleRatio = renderScaleRatio;
	//SET FULLSCREEN
	if (fullscreen) {
		window->create(sf::VideoMode::getFullscreenModes()[0], WindowName, sf::Style::Fullscreen);
	}
	window->setFramerateLimit(fpsLimit);
	window->setVerticalSyncEnabled(bVSync);
	glewInit();
}

void _NL::Engine::WindowManager::RunGameLoop()
{
	Start();
	while (window->isOpen()) {
		update();
	}
}

void _NL::Engine::WindowManager::OpenGLStart()
{
	///SCREEN QUAD
	ScreenShader.installShaders("screenQuadVshader.glsl", "screenQuadFshader.glsl");

	///SETUP FRAME BUFFER OBJECTs
	//ColorTextures
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &ColorTexture);
		glBindTexture(GL_TEXTURE_2D, ColorTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			window->getSize().x * renderScaleRatio,
			window->getSize().y * renderScaleRatio,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			NULL);
		check_gl_error_full();
	}
	
	//DepthTexture
	{
		glCreateTextures(GL_TEXTURE_2D, 1, &DepthTexture);
		glBindTexture(GL_TEXTURE_2D, DepthTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_DEPTH_COMPONENT,
			window->getSize().x * renderScaleRatio,
			window->getSize().y * renderScaleRatio,
			0,
			GL_DEPTH_COMPONENT,
			GL_UNSIGNED_BYTE,
			NULL);
	
		check_gl_error();
	}

	//RenderBuffers
	{
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
		glCreateFramebuffers(1, &FrameBuffer[0]);
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer[0]);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, ColorRenderBuffer[0]);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthRenderBuffer[0]);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D,
			ColorTexture,
			0);
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT,
			GL_TEXTURE_2D,
			DepthTexture,
			0);
		check_gl_error();
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
			check_gl_error();
		}
		//glDrawBuffer(GL_NONE);
		//glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		check_gl_error();

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
	}

	Cameras.clear();
	Lights.clear();
	//ACTION !

	//GET OBJECTS
	for each (_NL::Object::GameObject* obj in CurrentScene->GetObjectList())
	{
		if (obj->ClassName() == "_NL::Object::GameObject") {
			std::cout << "initGLObj: " << obj->name << std::endl;
			obj->getComponent(_NL::Component::MeshRenderer())->initGLObj();
		}
	}

	//GET CAMERAS
	for each (_NL::Object::CameraObj* obj in CurrentScene->GetObjectList())
	{
		if (obj->ClassName() == "_NL::Object::CameraObj") {
			Cameras.push_back(obj);
		}
	}

	//GET LIGHTS
	for each (_NL::Object::LightObject* obj in CurrentScene->GetObjectList())
	{
		if (obj->ClassName() == "_NL::Object::LightObject") {
			Lights.push_back(obj->LightProperties);
		}
	}
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

	//GL Settings
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
	OpenGLStart();
}

void _NL::Engine::WindowManager::DrawCurrentScene() {
	for each (_NL::Object::CameraObj* Cam in Cameras)
	{
		Cam->updateCameraProjectionMatrix();
		glViewport(0, 0, window->getSize().x * renderScaleRatio, window->getSize().y * renderScaleRatio);
		glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer[CurrentDrawFrameBuffer]);
		ClearCurrentBuffer();
		///DEBUG
		//if (Cam->name == "MyCam")Cam->Transform.position.z += -Time.DeltaTime.asSeconds();
		
		//Update Light ubo;
		glBindBuffer(GL_UNIFORM_BUFFER, LightsBlockUBO);
		glBufferSubData(GL_UNIFORM_BUFFER,
			0,
			sizeof(_NL::Core::LightProperties)*Lights.size(),
			&Lights[0]);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		check_gl_error();

		int LightI = 0;
		for each (_NL::Object::GameObject* obj in CurrentScene->GetObjectList())
		{
			if (obj->ClassName() == "_NL::Object::LightObject") {	
				//DEBUG
				//Lights[LightI].position.x += 1 * Time.DeltaTime.asSeconds();
				//Lights[LightI].lightColor.x -= .1 * Time.DeltaTime.asSeconds();
				//Lights[LightI].lightColor.z -= .1 * Time.DeltaTime.asSeconds();
				LightI++;
			}

			if (obj->ClassName() == "_NL::Object::GameObject") {
				///SHADER
				//COMPONENTS
				_NL::Component::MeshRenderer* ObjMR = obj->getComponent(_NL::Component::MeshRenderer());
				glUseProgram(ObjMR->Shader->getShaderProgram());
				///VERTEX
				_NL::Component::Transform* ObjT;
				_NL::Component::Transform* ObjT_P;
				if (obj->Parent != 0) {
					ObjT = obj->getComponent(_NL::Component::Transform());
					ObjT_P = obj->Parent->getComponent(_NL::Component::Transform());
				}
				else {
					ObjT = obj->getComponent(_NL::Component::Transform());
					ObjT_P = new _NL::Component::Transform();
				}
				///PARENTING
				glm::mat4 T = glm::translate(glm::mat4(), ObjT->transform.position) * glm::translate(glm::mat4(), ObjT_P->transform.position);
				glm::mat4 R = ObjT->transform.MatrixRotation * ObjT_P->transform.MatrixRotation;
				glm::mat4 S = glm::scale(glm::mat4(), ObjT->transform.scale) * glm::scale(glm::mat4(), ObjT_P->transform.scale);
				glm::mat4 Modelmat = T * R * S;
				glUniformMatrix4fv(ObjMR->ModelMatrix_atrib, 1, GL_FALSE, glm::value_ptr(Modelmat));
				///INLINE PARENTING glm::scale(glm::scale(glm::rotate(glm::rotate(glm::translate(glm::translate(Cam->projectionMatrix*Cam->getWorldToViewMatrix(), ObjT_P->transform.position), ObjT->transform.position), ObjT_P->transform.rotationAngle, ObjT_P->transform.rotationAxis), ObjT->transform.rotationAngle, ObjT->transform.rotationAxis), ObjT_P->transform.scale), ObjT->transform.scale))
				
				///DEBUG
				//std::cout << "Draw: " << obj->name << std::endl;
				if (obj->name != "Quad" && obj->name != "Skybox")ObjT->Rotate(glm::vec3(0,1,0) * Time.DeltaTime.asSeconds());
				if (obj->name == "nameless")ObjT->transform.position.y = ObjT->transform.position.y + std::sin(ObjT->transform.EulerRotation.y)*Time.DeltaTime.asSeconds();
				
				///RENDER
				//glUniformMatrix4fv(ObjMR->FullTransformMatrix_atrib, 1, GL_FALSE, glm::value_ptr(glm::scale(glm::scale(glm::rotate(glm::rotate(glm::translate(glm::translate(Cam->projectionMatrix*Cam->getWorldToViewMatrix(), ObjT_P->transform.position), ObjT->transform.position), ObjT_P->transform.rotationAngle, ObjT_P->transform.rotationAxis), ObjT->transform.rotationAngle, ObjT->transform.rotationAxis), ObjT_P->transform.scale), ObjT->transform.scale)));
				//glUniformMatrix4fv(ObjMR->ModelMatrix_atrib, 1, GL_FALSE, glm::value_ptr(glm::scale(glm::scale(ObjT->transform.MatrixRotation * ObjT_P->transform.MatrixRotation * (glm::translate(glm::translate(glm::mat4(), ObjT_P->transform.position), ObjT->transform.position)), ObjT_P->transform.scale), ObjT->transform.scale)));
				glUniformMatrix4fv(ObjMR->ViewMatrix_atrib, 1, GL_FALSE, glm::value_ptr(Cam->getWorldToViewMatrix()));
				glUniformMatrix4fv(ObjMR->ProjectionMatrix_atrib, 1, GL_FALSE, glm::value_ptr(Cam->projectionMatrix));
				glBindVertexArray(ObjMR->vao);
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
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void _NL::Engine::WindowManager::DrawScreenQuad()
{
	glViewport(0, 0, window->getSize().x, window->getSize().y);

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void _NL::Engine::WindowManager::Display() {
	
	///DISPLAY
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, ColorTexture);
	DrawScreenQuad();
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
	window->display();	
	
}

void _NL::Engine::WindowManager::update() {
	
	sf::Event event;
	while (window->pollEvent(event))
	{
		//INPUT HANDLER
		if (event.type == sf::Event::Closed)
			window->close();
		//...//
	}
	//UPDATE DISPLAY;
	check_gl_error();
	DrawCurrentScene();
	Display();
	Time.Tick();
}


_NL::Engine::WindowManager::~WindowManager()
{
}




