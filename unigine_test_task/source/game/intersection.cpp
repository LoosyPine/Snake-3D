#include "intersection.h"

void Intersection::update()
{
	for (auto first_object : jointBuffer->Objects)
	{
		a = first_object.second->getPosition();
		firstObjectScale = first_object.second->getScale();
		a_diagonal_point = { a.x + abs(firstObjectScale.x) / 2.0f, a.y + abs(firstObjectScale.y) / 2.0f, a.z + abs(firstObjectScale.z) / 2.0f };
		a_diagonal_lenght = getDistance(a, a_diagonal_point);
		for (auto second_object : jointBuffer->Objects)
		{
			if (first_object.second == second_object.second)
				continue;
			b = second_object.second->getPosition();
			secondObjectScale = second_object.second->getScale();
			b_diagonal_point = { b.x + abs(secondObjectScale.x) / 2.0f, b.y + abs(secondObjectScale.y) / 2.0f, b.z + abs(secondObjectScale.z) / 2.0f };
			b_diagonal_lenght = getDistance(b, b_diagonal_point);
			result = getDistance(a, b);
			if (result < (a_diagonal_lenght + b_diagonal_lenght))
			{
				if (tryEmplaceSphere(first_object.first, second_object.second)) {}
				else if (tryEmplacePlane(first_object.first, second_object.second)) {}
				else if (tryEmplaceCube(first_object.first, second_object.second)) {}

				if (tryEmplaceSphere(second_object.first, first_object.second)) {}
				else if (tryEmplacePlane(second_object.first, first_object.second)) {}
				else if (tryEmplaceCube(second_object.first, first_object.second)) {}
			}
			else
			{
				if (tryRemoveSphere(first_object.first, second_object.second)) {}
				else if (tryRemoveCube(first_object.first, second_object.second)) {}
				else if (tryRemovePlane(first_object.first, second_object.second)) {}

				if (tryRemoveSphere(second_object.first, first_object.second)) {}
				else if (tryRemoveCube(second_object.first, first_object.second)) {}
				else if (tryRemovePlane(second_object.first, first_object.second)) {}
			}
		}
	}
	jointBuffer->IntersectionThreadState = true;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

float Intersection::getDistance(const glm::vec3& a, const glm::vec3& b)
{
	return sqrt(glm::pow((a.x - b.x), 2) + glm::pow((a.y - b.y), 2) + glm::pow((a.z - b.z), 2));
}

float Intersection::getDistance(const glm::vec2& a, const glm::vec2& b)
{
	return sqrt(glm::pow((a.x - b.x), 2) + glm::pow((a.y - b.y), 2));
}

bool Intersection::checkInstersection(const glm::vec3& objectVec3)
{
	if (result <= (abs(objectVec3.x) + abs(objectVec3.z) + abs(objectVec3.y)))
	{
		return true;
	}
	return false;
}

bool Intersection::checkInstersection(float result, float object_diagonal)
{
	if (result <= object_diagonal)
	{
		return true;
	}
	return false;
}

bool Intersection::tryEmplaceSphere(const std::string& name_object, Object* offer_object)
{
	auto iter = jointBuffer->Spheres.find(name_object);
	if (iter != jointBuffer->Spheres.end())
	{
		iter->second->addIntersectingObject(offer_object);
		return true;
	}
	return false;
}

bool Intersection::tryEmplacePlane(const std::string& name_object, Object* offer_object)
{
	auto iter = jointBuffer->Planes.find(name_object);
	if (iter != jointBuffer->Planes.end())
	{
		iter->second->addIntersectingObject(offer_object);
		return true;
	}
	return false;
}

bool Intersection::tryEmplaceCube(const std::string& name_object, Object* offer_object)
{
	auto iter = jointBuffer->Cubes.find(name_object);
	if (iter != jointBuffer->Cubes.end())
	{
		iter->second->addIntersectingObject(offer_object);
		return true;
	}
	return false;
}

bool Intersection::tryRemoveSphere(const std::string& name_object, Object* offer_object)
{
	auto iter = jointBuffer->Spheres.find(name_object);
	if (iter != jointBuffer->Spheres.end())
	{
		iter->second->removeIntersectingObject(offer_object);
		return true;
	}
	return false;
}

bool Intersection::tryRemovePlane(const std::string& name_object, Object* offer_object)
{
	auto iter = jointBuffer->Planes.find(name_object);
	if (iter != jointBuffer->Planes.end())
	{
		iter->second->removeIntersectingObject(offer_object);
		return true;
	}
	return false;
}

bool Intersection::tryRemoveCube(const std::string& name_object, Object* offer_object)
{
	auto iter = jointBuffer->Cubes.find(name_object);
	if (iter != jointBuffer->Cubes.end())
	{
		iter->second->removeIntersectingObject(offer_object);
		return true;
	}
	return false;
}