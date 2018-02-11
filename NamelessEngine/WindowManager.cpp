#include "WindowManager.h"

_NL::Engine::WindowManager::WindowManager(const char* WindowName, int Width, int height, bool fullscreen)
{
	window = new sf::RenderWindow(sf::VideoMode(Width, height), WindowName);
	//SET FULLSCREEN
	if (fullscreen) {
		window->create(sf::VideoMode::getFullscreenModes()[0], WindowName, sf::Style::Fullscreen);
	}
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
	Cameras.clear();
	//INIT OBJ DATA from WORLD
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

}

void _NL::Engine::WindowManager::Start()
{
	OpenGLStart();
}

void _NL::Engine::WindowManager::DrawCurrentScene() {

	for each (_NL::Object::CameraObj* Cam in Cameras)
	{
		int i = 0;
		Cam->updateCameraSettings();
		for each (_NL::Object::GameObject* obj in CurrentScene->GetObjectList())
		{
			if (obj->ClassName() == "_NL::Object::GameObject") {
				///DEBUG
				//std::cout << "Draw: " << obj->name << std::endl;

				_NL::Component::MeshRenderer* ObjMR = obj->getComponent(_NL::Component::MeshRenderer());
				_NL::Component::Transform* ObjT = obj->getComponent(_NL::Component::Transform());
				(i != 0) ? ObjT->transform.rotationAngle += .0001 : i;
				glBindVertexArray(ObjMR->vao);
				glUseProgram(ObjMR->Shader->getShaderProgram());
				glm::mat4 FullTransform = glm::scale(glm::rotate(glm::translate(Cam->projectionMatrix*Cam->getWorldToViewMatrix(), ObjT->transform.position), ObjT->transform.rotationAngle, ObjT->transform.rotationAxis), ObjT->transform.scale);
				glUniformMatrix4fv(ObjMR->FullTransformMatrix_atrib, 1, GL_FALSE, glm::value_ptr(FullTransform));
				glDrawElements(
					GL_TRIANGLES,
					ObjMR->Mesh->Indices.size() * 3,
					GL_UNSIGNED_INT,
					0
				);
				glUseProgram(0);
				glBindVertexArray(0);
				i++;
			}
		}
	}
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	DrawCurrentScene();
	window->display();
	//...//
}


_NL::Engine::WindowManager::~WindowManager()
{
}




