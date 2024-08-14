#pragma once
#include <memory>
#include <unordered_map>
#include <thread>
#include <list>
#include "../framework/engine.h"
#include "../framework/utils.h"

class Engine;
class Camera;
class Sphere;
class Plane;
class Cube;
class Intersection;

struct JointBuffer
{
	bool GlobalAppState = false;
	Engine* Engine = nullptr;
	Camera* Camera = nullptr;
	std::unordered_map<std::string, Object*> Objects;
	std::unordered_map<Object*, std::string> NamesObjects;
	std::unordered_map<std::string, Sphere*> Spheres;
	std::unordered_map<std::string, Plane*> Planes;
	std::unordered_map<std::string, Cube*> Cubes;
	Intersection* intersection = nullptr;
	bool IntersectionThreadState = false;
	bool CameraDefendLock = false;
	std::thread* intersection_thread = nullptr;
};

extern JointBuffer* jointBuffer;



//void setEscort(std::string, std::list<std::string> listNames);
//void Core::setEscort(std::string nameEscort, std::list<std::string> listNames)
//{
//	jointBuffer->Escort.emplace(nameEscort, listNames);
//	auto iterHead = listNames.front();
//	for (auto iterName : listNames)
//	{
//		jointBuffer->Objects.find(iterName)->first;
//		// ÑÎÇÄÀÉ ÊËÀÑÑ ÄËß ÝÑÊÎÐÒÀ ×ÒÎÁÛ Ó ÂÑÅÕ ÎÁÚÅÊÒÎÂ ÝÑÊÎÐÒÀ ÎÄÈÍÀÊÎÂÎ ÌÅÍßËÈÑÜ ÊÎÎÐÄÈÍÀÒÛ
//	}
//}
//void getEscort(std::string name)
//{
//	jointBuffer->Escort.find(name)->second;
//}