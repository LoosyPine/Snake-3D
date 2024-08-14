#include "plane.h"

Plane::Plane()
{
	//std::cout << "PLANE CREATE" << std::endl;
}

Plane::~Plane()
{
	//std::cout << "PLANE DELETE" << std::endl;
}

Object* Plane::getObjectPointer()
{
	return this->plane;
}

void Plane::init(Engine* objEngine, ObjectInfo* objInfo)
{
		this->plane = objEngine->createObject(&(this->plane_mesh));
		this->plane->setRotation(objInfo->rotation.x, objInfo->rotation.y, objInfo->rotation.z);
		this->plane->setPosition(objInfo->position.x, objInfo->position.y, objInfo->position.z);
		this->plane->setColor(objInfo->color.r, objInfo->color.g, objInfo->color.b);
		this->plane->setScale(objInfo->scale.x, objInfo->scale.y, objInfo->scale.z);
		jointBuffer->Objects.emplace(objInfo->name, this->plane);
		jointBuffer->NamesObjects.emplace(this->plane, objInfo->name);
}

void Plane::addIntersectingObject(Object* object)
{
	this->intersectingObjects.try_emplace(object, jointBuffer->NamesObjects.find(object)->second);
	//object->setColor(1.0f, 0.0f, 0.0f);
}

bool Plane::removeIntersectingObject(Object* object)
{
	if (this->intersectingObjects.erase(object) == true)
	{
		//object->setColor(0.0f, 1.0f, 0.0f);
		return true;
	}
	return false;
}
