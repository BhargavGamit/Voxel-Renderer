// Transform.cpp: implementation of the Transform class.

#include "Transform.h"


// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis)
{
	mat3 Identity_Matrix(1);
	mat3 A_star(0);
	vec3 normal = glm::normalize(axis);
	float x = normal.x, y = normal.y, z = normal.z;
	mat3 aaT = mat3(vec3(x*x, y*x, z*x), vec3(x*y, y*y, z*y), vec3(x*z, y*z, z*z));
	
	A_star[0][1] = -axis[2];
	A_star[0][2] = axis[1];
	A_star[1][0] = axis[2];
	A_star[1][2] = -axis[0];
	A_star[2][0] = -axis[1];
	A_star[2][1] = axis[0];
	//R(a, θ) = I3×3 cosθ + aaT(1− cosθ) + A*sinθ
	mat3 rotateMatrix = Identity_Matrix*cos(glm::radians(degrees)) + aaT * (1 - cos(glm::radians(degrees))) + sin(glm::radians(degrees)) * A_star;

	return rotateMatrix;
}

// Transforms the camera left around the object
void Transform::left(float degrees, vec3& camera, vec3& up)
{
	camera = rotate(-degrees, up) * camera;
}

// Transforms the camera up around the object
void Transform::up(float degrees, vec3& camera, vec3& up)
{
	vec3 rotMat = glm::normalize(glm::cross(up, camera));
	camera = rotate(degrees, rotMat) * camera;
	up = rotate(degrees, rotMat) * up;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
