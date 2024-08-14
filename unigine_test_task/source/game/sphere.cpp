#include "sphere.h"

Sphere::Sphere()
{
	//std::cout << "SPHERE CREATE" << std::endl;
}

Sphere::~Sphere()
{
	//std::cout << "SPHERE DELETE" << std::endl;
}

Object* Sphere::getObjectPointer()
{
	return this->sphere;
}

void Sphere::init(Engine* objEngine, ObjectInfo* objInfo)
{
	this->sphere = objEngine->createObject(&(this->sphere_mesh));
	this->sphere->setRotation(objInfo->rotation.x, objInfo->rotation.y, objInfo->rotation.z);
	this->sphere->setPosition(objInfo->position.x, objInfo->position.y, objInfo->position.z);
	this->sphere->setColor(objInfo->color.r, objInfo->color.g, objInfo->color.b);
	this->sphere->setScale(objInfo->scale.x, objInfo->scale.y, objInfo->scale.z);
	jointBuffer->Objects.emplace(objInfo->name, this->sphere);
	jointBuffer->NamesObjects.emplace(this->sphere, objInfo->name);
}

void Sphere::addIntersectingObject(Object* object)
{
	this->intersectingObjects.try_emplace(object, jointBuffer->NamesObjects.find(object)->second);
	//object->setColor(1.0f, 0.0f, 0.0f);
}

bool Sphere::removeIntersectingObject(Object* object)
{
	if (this->intersectingObjects.erase(object) == true)
	{
		//object->setColor(0.0f, 1.0f, 0.0f);
		return true;
	}
	return false;
}





