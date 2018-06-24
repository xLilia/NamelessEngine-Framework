#pragma once
#include "GameObject.h"

class BOIDObject : public _NL::Object::GameObject
{
public:
	BOIDObject();
	~BOIDObject();

	glm::vec3 velocity;

	_NL::Component::Transform* T = new _NL::Component::Transform();
	_NL::Component::MeshRenderer* Mr = new _NL::Component::MeshRenderer();
	_NL::Element::ShaderInstance* Shader;
	_NL::Element::MaterialInstance* Material;
	_NL::Element::MeshInstance* Mesh;
	void UpdateMeshRenderer();
	void UpdateTransform();
	char* getTypeName() override;

};

