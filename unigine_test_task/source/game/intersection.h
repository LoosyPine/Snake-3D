#pragma once

#include "core.h"

class Intersection
{
public:
	static Intersection* get();
	//Intersection();
	void update();
private:
	float getDistance(const glm::vec3& a, const glm::vec3& b);
	float getDistance(const glm::vec2& a, const glm::vec2& b);
	bool checkInstersection(const glm::vec3& objectVec3);
	bool checkInstersection(float result, float object_diagonal);
	bool tryEmplaceSphere(const std::string& name_object, Object* offer_object);
	bool tryEmplacePlane(const std::string& name_object, Object* offer_object);
	bool tryEmplaceCube(const std::string& name_object, Object* offer_object);
	bool tryRemoveSphere(const std::string& name_object, Object* offer_object);
	bool tryRemovePlane(const std::string& name_object, Object* offer_object);
	bool tryRemoveCube(const std::string& name_object, Object* offer_object);
	
	float result;
	glm::vec3 a = { 0.0f, 0.0f, 0.0f };
	glm::vec3 a_diagonal_point = { 0.0f, 0.0f, 0.0f };
	float a_diagonal_lenght = 0.0f;
	glm::vec3 firstObjectScale = { 0.0f, 0.0f, 0.0f };
	glm::vec3 b = { 0.0f, 0.0f, 0.0f };
	glm::vec3 b_diagonal_point = { 0.0f, 0.0f, 0.0f };
	float b_diagonal_lenght = 0.0f;
	glm::vec3 secondObjectScale = { 0.0f, 0.0f, 0.0f };
};