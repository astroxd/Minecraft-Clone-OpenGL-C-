#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaderClass.h"


struct Plane {
	glm::vec3 normal = { 0.f, 1.f, 0.f }; // unit vector
	float     distance = 0.f;        // Distance with origin

	Plane() = default;

	Plane(const glm::vec3& p1, const glm::vec3& norm)
		: normal(glm::normalize(norm)),
		distance(glm::dot(normal, p1))
	{}

	float getSignedDistanceToPlan(const glm::vec3& point) const
	{
		return glm::dot(normal, point) - distance;
	}
};

struct Frustum {
	Plane topFace;
	Plane bottomFace;

	Plane rightFace;
	Plane leftFace;

	Plane farFace;
	Plane nearFace;
};

struct BoundingVolume
{
	virtual bool isOnFrustum(const Frustum& camFrustum, const glm::mat4& transform) const = 0;

	virtual bool isOnOrForwardPlan(const Plane& plan) const = 0;

	bool isOnFrustum(const Frustum& camFrustum) const
	{
		return (isOnOrForwardPlan(camFrustum.leftFace) &&
			isOnOrForwardPlan(camFrustum.rightFace) &&
			isOnOrForwardPlan(camFrustum.topFace) &&
			isOnOrForwardPlan(camFrustum.bottomFace) &&
			isOnOrForwardPlan(camFrustum.nearFace) &&
			isOnOrForwardPlan(camFrustum.farFace));
	};
};


struct Sphere : public BoundingVolume
{
	glm::vec3 center{ 0.f, 0.f, 0.f };
	float radius{ 0.f };

	Sphere(const glm::vec3& inCenter, float inRadius)
		: BoundingVolume{}, center{ inCenter }, radius{ inRadius }
	{}

	bool isOnOrForwardPlan(const Plane& plan) const final
	{
		return plan.getSignedDistanceToPlan(center) > -radius;
	}

	bool isOnFrustum(const Frustum& camFrustum, const glm::mat4& transform) const final
	{
		//Get global scale thanks to our transform
		const glm::vec3 globalScale = glm::vec3{ glm::length(transform[0]), glm::length(transform[1]) ,glm::length(transform[2]) };

		//Get our global center with process it with the global model matrix of our transform
		//CHUNK MODEL METRIX
		const glm::vec3 globalCenter{ transform * glm::vec4(center, 1.f) };

		//To wrap correctly our shape, we need the maximum scale scalar.
		const float maxScale = std::max(std::max(globalScale.x, globalScale.y), globalScale.z);

		//Max scale is assuming for the diameter. So, we need the half to apply it to our radius
		Sphere globalSphere(globalCenter, radius * (maxScale * 0.5f));

		//Check Firstly the result that have the most chance to faillure to avoid to call all functions.
		return (globalSphere.isOnOrForwardPlan(camFrustum.leftFace) &&
			globalSphere.isOnOrForwardPlan(camFrustum.rightFace) &&
			globalSphere.isOnOrForwardPlan(camFrustum.farFace) &&
			globalSphere.isOnOrForwardPlan(camFrustum.nearFace) &&
			globalSphere.isOnOrForwardPlan(camFrustum.topFace) &&
			globalSphere.isOnOrForwardPlan(camFrustum.bottomFace));
	};
};


class Camera {
public:
	// Stores the main vectors of the camera
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	// Prevents the camera from jumping around when first clicking left click
	bool firstClick = true;

	// Stores the width and height of the window
	int width;
	int height;

	// Adjust the speed of the camera and it's sensitivity when looking around
	float speed = 1.0f;
	float sensitivity = 50.0f;


	// Camera constructor to set up initial values
	Camera(int width, int height, glm::vec3 position);
	
	void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader& shader, const char* uniform);
	
	glm::mat4 getProj();
	// Handles camera inputs
	void Inputs(GLFWwindow* window, float deltaTime);


	Frustum createFrustumFromCamera(float aspect, float fovDeg, float nearPlane, float farPlane);

	float half_y = glm::radians(45.0) * 0.5;
	float factor_y = 1.0f / cos(half_y);
	float tan_y = tan(half_y);
	
	float half_x = glm::radians(45.0) * 0.5;
	float factor_x = 1.0f / cos(half_x);
	float tan_x = tan(half_x);

	bool isOnFrustum(glm::vec3 chunkCenter);

	std::string getCameraPosition();
	std::string getCameraOrientation();

};




#endif;
