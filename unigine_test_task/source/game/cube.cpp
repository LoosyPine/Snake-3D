#include "cube.h"

Cube::Cube()
{
	//std::cout << "CUBE CREATE" << std::endl;
}

Cube::~Cube()
{
	//std::cout << "CUBE DELETE" << std::endl;
}

Object* Cube::getObjectPointer()
{
	return this->cube;
}

void Cube::init(Engine* objEngine, ObjectInfo* objInfo)
{
	this->cube = objEngine->createObject(&(this->cube_mesh));
	this->cube->setRotation(objInfo->rotation.x, objInfo->rotation.y, objInfo->rotation.z);
	this->cube->setPosition(objInfo->position.x, objInfo->position.y, objInfo->position.z);
	this->cube->setColor(objInfo->color.r, objInfo->color.g, objInfo->color.b);
	this->cube->setScale(objInfo->scale.x, objInfo->scale.y, objInfo->scale.z);
	jointBuffer->Objects.emplace(objInfo->name, this->cube);
	jointBuffer->NamesObjects.emplace(this->cube, objInfo->name);
}

void Cube::addIntersectingObject(Object* object)
{
	this->intersectingObjects.try_emplace(object, jointBuffer->NamesObjects.find(object)->second);
	//object->setColor(1.0f, 0.0f, 0.0f);
}

bool Cube::removeIntersectingObject(Object* object)
{
	if (this->intersectingObjects.erase(object) == true)
	{
		//object->setColor(0.0f, 1.0f, 0.0f);
		return true;
	}
	return false;
}