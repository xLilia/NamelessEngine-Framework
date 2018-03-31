#include "Transform.h"

_NL::Component::Transform::Transform()
{
	transform.position = glm::vec3(0, 0, 0);
	transform.EulerRotation = glm::vec3(0, 0, 0);
	transform.RotationMatrix = glm::mat4();
	transform.scale = glm::vec3(1, 1, 1);
}

std::string _NL::Component::Transform::ClassName() const
{
	return "_NL::Component::Transform";
}

void _NL::Component::Transform::EulerRotation(GLfloat x, GLfloat y, GLfloat z)
{
	EulerRotation(glm::vec3(x, y, z));
}

void _NL::Component::Transform::EulerRotation(glm::vec3 Axis)
{
	transform.EulerRotation += Axis;
	transform.RotationMatrix *= glm::yawPitchRoll(Axis.y, Axis.x , Axis.z);

	//GLfloat Tetha = NL_PI / 180.0f;
	//Tetha = angle * Tetha;
	//
	//glm::vec3 u = axis * glm::sin(Tetha / 2.0f);
	//
	////quaterinon
	//glm::vec4 q(u.x, u.y, u.z, glm::cos(Tetha / 2.0f));
	//
	////Quaternion Rotation Matrix
	//glm::vec3 rq1(1.0f - 2.0f * q.y*q.y - 2.0f * q.z*q.z, 2.0f * q.x*q.y - 2.0f * q.z*q.w, 2.0f * q.x*q.z + 2.0f * q.y*q.w);
	//glm::vec3 rq2(2.0f * q.x*q.y + 2.0f * q.z*q.w, 1.0f - 2.0f * q.x*q.x - 2.0f * q.z*q.z, 2.0f * q.y*q.z - 2.0f * q.x*q.w);
	//glm::vec3 rq3(2.0f * q.x*q.z - 2.0f * q.y*q.w, 2.0f * q.y*q.z + 2.0f * q.x*q.w, 1.0f - 2.0f * q.x*q.x - 2.0f * q.y*q.y);
	//
	//glm::mat3 Rq(rq1, rq2, rq3);
	//
	//transform.EulerRotation = Rq * transform.EulerRotation;

	//glm::vec4()
	//transform.EulerRotation += EulerRotation;
	//glm::mat4 RotX = glm::rotate(glm::mat4(), Axis.x, glm::vec3(1, 0, 0));
	//glm::mat4 RotY = glm::rotate(glm::mat4(), Axis.y, glm::vec3(0, 1, 0));
	//glm::mat4 RotZ = glm::rotate(glm::mat4(), Axis.z, glm::vec3(0, 0, 1));
	//transform.RotationMatrix *= RotZ * RotX * RotY;

}

_NL::Component::Transform::~Transform()
{
}


