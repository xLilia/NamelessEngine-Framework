#include "Transform.h"

_NL::Component::Transform::Transform()
{
	
}

char* _NL::Component::Transform::getTypeName() 
{
	return "_NL::Component::Transform";
}

_NL::Component::Transform::~Transform()
{
}

glm::vec3 _NL::Component::Transform::getRotatedForwardVector()
{
	return this->transform.QuaternionRotation * this->forwardVector;
}

glm::vec3 _NL::Component::Transform::getParentedVector(glm::vec3 Vector)
{
	return this->transform.QuaternionRotation * Vector;
}

glm::vec3 _NL::Component::Transform::eulerAngles()
{
	return glm::eulerAngles(transform.QuaternionRotation);
}

glm::quat _NL::Component::Transform::LookAt(glm::vec3 target, GLfloat x, GLfloat y, GLfloat z)
{
	return LookAt(target, glm::vec3(x, y, z));
}

glm::quat _NL::Component::Transform::LookAt(glm::vec3 target, glm::vec3 EyeAxis)
{

	glm::vec3 forward = glm::normalize(target - transform.position); //get new Local Z
	
	glm::vec3 worldForward = EyeAxis; //get World Z
	GLfloat dot = glm::dot(worldForward, forward); //getAngle LocalZ > WorldZ
	GLfloat rotationAngle = glm::acos(dot); //AngleIn degrees
	glm::vec3 rotationAxis = glm::normalize(glm::cross(forward, worldForward)); //localUP

	return glm::toQuat(glm::rotate(glm::mat4(), rotationAngle, -rotationAxis));
	
}

//glm::quat _NL::Component::Transform::QuaternionFromAxisAngle(glm::vec3 ax, GLfloat angle) {
//		
//	ax = glm::normalize(ax);
//	GLfloat qw = cos(angle / 2);
//	GLfloat qx = ax.x * sin(angle / 2);
//	GLfloat qy = ax.y * sin(angle / 2);
//	GLfloat qz = ax.z * sin(angle / 2);
//	return glm::normalize(glm::quat(qw, qx, qy, qz));
//}

glm::quat _NL::Component::Transform::EulerRotation(GLfloat x, GLfloat y, GLfloat z)
{
	return glm::quat(glm::vec3(x, y, z));
}


glm::quat _NL::Component::Transform::EulerRotation(glm::vec3 eulerAngles)
{
	return glm::quat(eulerAngles);
}

void _NL::Component::Transform::getModelMatrixDecompose(glm::vec3& Translation, glm::quat& Rotation, glm::vec3 Scale)
{
	glm::decompose(ModelMatrix, glm::vec3(), glm::quat(), Translation, glm::vec3(), glm::vec4());
}

glm::vec3 _NL::Component::Transform::getModelMatrixTranslation()
{
	glm::vec3 Translation;
	glm::decompose(ModelMatrix, glm::vec3(), glm::quat(), Translation, glm::vec3(), glm::vec4());
	return Translation;
}

glm::quat _NL::Component::Transform::getModelMatrixRotation()
{
	glm::quat Rotation;
	glm::decompose(ModelMatrix, glm::vec3(), Rotation, glm::vec3(), glm::vec3(), glm::vec4());
	return Rotation;
}

glm::vec3 _NL::Component::Transform::getModelMatrixScale()
{
	glm::vec3 Scale;
	glm::decompose(ModelMatrix, Scale, glm::quat(), glm::vec3(), glm::vec3(), glm::vec4());
	return Scale;
}