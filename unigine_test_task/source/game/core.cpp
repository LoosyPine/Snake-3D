
#include "core.h"

/////////////////////////
/*		public         */
/////////////////////////

// getter Core static object
Core* Core::get()
{
	static Core core;
	return &core;
}
// init core
void Core::init(AppInfo* appInfo)
{
	Core::initEngine(appInfo->Resolution.width, appInfo->Resolution.height, appInfo->appName);
	delete appInfo;
	appInfo = nullptr;
}
// get Engine pointer from JointBuffer
Engine* Core::getEngine()
{
	return jointBuffer->Engine;
}
// get object pointer from JointBuffer
Object* Core::getObject(std::string nameObject)
{
	return jointBuffer->Objects.find(nameObject)->second;
}
// add sphere
Object* Core::addSphere(ObjectInfo* objInfo)
{
	if (jointBuffer->Engine == nullptr)
		throw "ERROR::JOINT_BUFFER::ENGINE::NOT_INIT\n";
	jointBuffer->Spheres.emplace(objInfo->name, new Sphere);
	jointBuffer->Spheres.find(objInfo->name)->second->init(jointBuffer->Engine, objInfo);
	return jointBuffer->Spheres.find(objInfo->name)->second->getObjectPointer();
}
// add plane
Object* Core::addPlane(ObjectInfo* objInfo)
{
	if (jointBuffer->Engine == nullptr)
		throw "ERROR::JOINT_BUFFER::ENGINE::NOT_INIT\n";
	jointBuffer->Planes.emplace(objInfo->name, new Plane);
	jointBuffer->Planes.find(objInfo->name)->second->init(jointBuffer->Engine, objInfo);
	return jointBuffer->Planes.find(objInfo->name)->second->getObjectPointer();
}
// add cube
Object* Core::addCube(ObjectInfo* objInfo)
{
	if (jointBuffer->Engine == nullptr)
		throw "ERROR::JOINT_BUFFER::ENGINE::NOT_INIT\n";
	jointBuffer->Cubes.emplace(objInfo->name, new Cube);
	jointBuffer->Cubes.find(objInfo->name)->second->init(jointBuffer->Engine, objInfo);
	return jointBuffer->Cubes.find(objInfo->name)->second->getObjectPointer();
}
void Core::initIntersectionEngine()
{
	jointBuffer->intersection = new Intersection;
	jointBuffer->GlobalAppState = true;
	jointBuffer->intersection_thread = new std::thread(&Core::circleIntersectionEngine, this);
}
// run intersection module
void Core::circleIntersectionEngine()
{
	while (jointBuffer->GlobalAppState)
	{
		updateIntersections();
	}
}
// check intesecting hash table of two objects and return TRUE if they have contact
bool Core::checkIntersectionObjects(std::string& name_object_1, std::string& name_object_2)
{
	if (checkIntersectionSphere(name_object_1, name_object_2) == true) return true;
	else if (checkIntersectionCube(name_object_1, name_object_2) == true) return true;
	else if (checkIntersectionPlane(name_object_1, name_object_2) == true) return true;
	else return false;
}
// check intesection module state
bool Core::getIntersectionState()
{
	return jointBuffer->IntersectionThreadState;
}

// DEFAULT CAMERA SETTINGS
//	YAW = -90.0f;
//	PITCH = 0.0f;
//	SPEED = 2.5f;
//	SENSITIVITY = 0.1f;
//	ZOOM = 45.0f;

// init camera
void Core::initCamera(CamInfo* camInfo)
{
	jointBuffer->Camera = &jointBuffer->Engine->getCamera();
	jointBuffer->Camera->Position = glm::vec3(camInfo->pos.x, camInfo->pos.y, camInfo->pos.z);
	jointBuffer->Camera->Pitch = camInfo->pitch;
	jointBuffer->Camera->Yaw = camInfo->yaw;
	jointBuffer->Camera->UpdateCameraVectors();
}
void Core::setupCamera(glm::vec3 position, float yaw, float pitch, float zoom)
{
		jointBuffer->Camera->Position = position;
		jointBuffer->Camera->Yaw = yaw;
		jointBuffer->Camera->Pitch = pitch;
		jointBuffer->Camera->Zoom = zoom;
}

/////////////////////////
/* private / protected */
/////////////////////////

// init engine
void Core::initEngine(unsigned short width, unsigned short height, const char* appName)
{
	jointBuffer->Engine = Engine::get();
	jointBuffer->Engine->init(width, height, appName);
}
void Core::setGlobalAppState(bool on_off)
{
	jointBuffer->GlobalAppState = on_off;
}
// intersection functions
void Core::updateIntersections()
{
	jointBuffer->intersection->update();
}
void Core::joinIntersectionThread()
{
	jointBuffer->intersection_thread->join();
}
void Core::detachIntersectionThread()
{
	jointBuffer->intersection_thread->detach();
}
// check intesection for sphere class
bool Core::checkIntersectionSphere(std::string& name_object_1, std::string& name_object_2)
{
	auto iter = jointBuffer->Spheres.find(name_object_1);
	if (iter != jointBuffer->Spheres.end())
	{
		if (iter->second->intersectingObjects.find(jointBuffer->Objects.find(name_object_2)->second) != iter->second->intersectingObjects.end())
			return true;
		else return false;
	}
	return false;
}
// check intesection for cube class
bool Core::checkIntersectionCube(std::string& name_object_1, std::string& name_object_2)
{
	auto iter = jointBuffer->Cubes.find(name_object_1);
	if (iter != jointBuffer->Cubes.end())
	{
		if (iter->second->intersectingObjects.find(jointBuffer->Objects.find(name_object_2)->second) != iter->second->intersectingObjects.end())
			return true;
		else return false;
	}
	return false;
}
// check intesection for plane class
bool Core::checkIntersectionPlane(std::string& name_object_1, std::string& name_object_2)
{
	auto iter = jointBuffer->Planes.find(name_object_1);
	if (iter != jointBuffer->Planes.end())
	{
		if (iter->second->intersectingObjects.find(jointBuffer->Objects.find(name_object_2)->second) != iter->second->intersectingObjects.end())
			return true;
		else return false;
	}
	return false;
}

/////////////////////////////////
/* ENGINE "RELOADED" FUNCTIONS */
/////////////////////////////////

// (DELETE ON OBJECT POINTER) find and delete object from 3d world and all buffers
void Core::deleteObject(Object* obj)
{
	auto object = jointBuffer->NamesObjects.find(obj);
	std::string nameObj = jointBuffer->NamesObjects.find(obj)->second;
	if (object == jointBuffer->NamesObjects.end())
		throw "ERROR::JOINT_BUFFER::NAME_OBJECTS::NOT_FOUND_OBJECT\n";
	auto sphere_iter = jointBuffer->Spheres.find(nameObj);
	if (sphere_iter != jointBuffer->Spheres.end())
	{
		delete sphere_iter->second;
		jointBuffer->Spheres.erase(sphere_iter);
	}
	else
	{
		auto plane_iter = jointBuffer->Planes.find(nameObj);
		if (plane_iter != jointBuffer->Planes.end())
		{
			delete plane_iter->second;
			jointBuffer->Planes.erase(plane_iter);
		}
		else
		{
			auto cube_iter = jointBuffer->Cubes.find(nameObj);
			if (cube_iter != jointBuffer->Cubes.end())
			{
				delete cube_iter->second;
				jointBuffer->Cubes.erase(cube_iter);
			}
		}
	}
	jointBuffer->Engine->deleteObject(obj);
}
// (DELETE ON NAME) find and delete object from 3d world and all buffers
void Core::deleteObject(std::string nameObject)
{
	auto object = jointBuffer->Objects.find(nameObject);
	if (object == jointBuffer->Objects.end())
		throw "ERROR::JOINT_BUFFER::OBJECTS::NOT_FOUND_OBJECT\n";
	jointBuffer->Engine->deleteObject(jointBuffer->Objects.find(nameObject)->second);

	if (jointBuffer->Cubes.find(nameObject) != jointBuffer->Cubes.end()) {
		jointBuffer->Cubes.erase(nameObject);
	}
	else if (jointBuffer->Spheres.find(nameObject) != jointBuffer->Spheres.end()) {
		jointBuffer->Spheres.erase(nameObject);
	}
	else if (jointBuffer->Planes.find(nameObject) != jointBuffer->Planes.end()) {
		jointBuffer->Planes.erase(nameObject);
	}
}

bool Core::isKeyPressed(int key)
{
	return jointBuffer->Engine->isKeyPressed(key);
}

bool Core::isDone()
{
	return jointBuffer->Engine->isDone();
}

void Core::update()
{
	jointBuffer->Engine->update();
	jointBuffer->Camera->UpdateCameraVectors();
}

void Core::render()
{
	jointBuffer->Engine->render();
}

void Core::swap()
{
	jointBuffer->Engine->swap();
}

void Core::shutdown()
{
	jointBuffer->Engine->shutdown();
	jointBuffer->intersection_thread->detach();
	jointBuffer->intersection_thread->~thread();
}

void Core::setCameraSpeed(float speed)
{
	jointBuffer->Engine->setCameraSpeed(speed);
}

void Core::scopeCoordsObject(Object* object)
{
	if (jointBuffer->Engine->isKeyPressed(GLFW_KEY_W))
	{
		object->setPosition(object->getPosition() + glm::vec3(0.0f, 0.0f, 0.1f));
		std::cout << object->getPosition().x << "\t" << object->getPosition().y << "\t" << object->getPosition().z << std::endl;
	}
	if (jointBuffer->Engine->isKeyPressed(GLFW_KEY_S))
	{
		object->setPosition(object->getPosition() + glm::vec3(0.0f, 0.0f, -0.1f));
		std::cout << object->getPosition().x << "\t" << object->getPosition().y << "\t" << object->getPosition().z << std::endl;
	}
	if (jointBuffer->Engine->isKeyPressed(GLFW_KEY_A))
	{
		object->setPosition(object->getPosition() + glm::vec3(-0.1f, 0.0f, 0.0f));
		std::cout << object->getPosition().x << "\t" << object->getPosition().y << "\t" << object->getPosition().z << std::endl;
	}
	if (jointBuffer->Engine->isKeyPressed(GLFW_KEY_D))
	{
		object->setPosition(object->getPosition() + glm::vec3(0.1f, 0.0f, 0.0f));
		std::cout << object->getPosition().x << "\t" << object->getPosition().y << "\t" << object->getPosition().z << std::endl;
	}
	if (jointBuffer->Engine->isKeyPressed(GLFW_KEY_Q))
	{
		object->setPosition(object->getPosition() + glm::vec3(0.0f, 0.1f, 0.0f));
		std::cout << object->getPosition().x << "\t" << object->getPosition().y << "\t" << object->getPosition().z << std::endl;
	}
	if (jointBuffer->Engine->isKeyPressed(GLFW_KEY_E))
	{
		object->setPosition(object->getPosition() + glm::vec3(0.0f, -0.1f, 0.0f));
		std::cout << object->getPosition().x << "\t" << object->getPosition().y << "\t" << object->getPosition().z << std::endl;
	}
}

float Core::getCameraSpeed()
{
	return jointBuffer->Engine->getCameraSpeed();
}

float Core::getDeltaTime()
{
	return jointBuffer->Engine->getDeltaTime();
}

float Core::getTime()
{
	return jointBuffer->Engine->getTime();
}

void Core::setEnvironmentColor(const glm::vec3& color)
{
	jointBuffer->Engine->setEnvironmentColor(color);
}

const glm::vec3& Core::getEnvironmentColor()
{
	return jointBuffer->Engine->getEnvironmentColor();
}

void Core::setLightDirection(const glm::vec3& dir)
{
	jointBuffer->Engine->setLightDirection(dir);
}

const glm::vec3& Core::getLightDirection()
{
	return jointBuffer->Engine->getLightDirection();
}

void Core::setLightColor(const glm::vec3& color)
{
	jointBuffer->Engine->setLightColor(color);
}

const glm::vec3& Core::getLightColor()
{
	return jointBuffer->Engine->getLightColor();
}

void Core::setLightAmbientColor(const glm::vec3& color)
{
	jointBuffer->Engine->setLightAmbientColor(color);
}

const glm::vec3& Core::getLightAmbientColor()
{
	return jointBuffer->Engine->getLightAmbientColor();
}

Shader& Core::getShader()
{
	return jointBuffer->Engine->getShader();
}