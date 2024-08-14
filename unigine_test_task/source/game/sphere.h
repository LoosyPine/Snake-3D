#pragma once

#include "../framework/engine.h"
#include "../framework/utils.h"
#include "structures.h"
#include "joint_buffer.h"

class Sphere
{
public:
	Sphere();
	~Sphere();
	void init(Engine* objEngine, ObjectInfo* objInfo);
	Object* getObjectPointer();
	void addIntersectingObject(Object* object);
	bool removeIntersectingObject(Object* object);
	std::unordered_map <Object*, std::string> intersectingObjects;
private:
	Object* sphere = nullptr;
	Mesh sphere_mesh = createSphere();
};

