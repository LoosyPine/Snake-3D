#pragma once

#include <chrono>
#include <thread>
#include <forward_list>
#include <ctime>
#include <cstdlib>
#include "../framework/engine.h"
#include "../framework/utils.h"
#include "structures.h"
#include "joint_buffer.h"
#include "cube.h"
#include "sphere.h"
#include "plane.h"
#include "intersection.h"




class Core
{
public:
	static Core* get();
	void init(AppInfo* appInfo);
	Engine* getEngine();
	Object* getObject(std::string nameObject);
	void deleteObject(std::string nameObject);
	Object* addSphere(ObjectInfo* objInfo);
	Object* addPlane(ObjectInfo* objInfo);
	Object* addCube(ObjectInfo* objInfo);

	void deleteObject(Object* obj);
	bool isKeyPressed(int key);
	bool isDone();
	void update();
	void render();
	void swap();
	void shutdown();
	
	void setEnvironmentColor(const glm::vec3& color);
	const glm::vec3& getEnvironmentColor();

	void setLightDirection(const glm::vec3& dir);
	const glm::vec3& getLightDirection();

	void setLightColor(const glm::vec3& color);
	const glm::vec3& getLightColor();

	void setLightAmbientColor(const glm::vec3& color);
	const glm::vec3& getLightAmbientColor();
	
	void setCameraSpeed(float speed);
	float getCameraSpeed();

	float getDeltaTime();
	float getTime();
	Shader& getShader();

	void scopeCoordsObject(Object* object);

	// camera functions
	void initCamera(CamInfo* camInfo);
	void setupCamera(glm::vec3 position, float yaw, float pitch, float zoom);
	void setCameraPosition(glm::vec3 position) { jointBuffer->Camera->Position = position; }
	void setCameraYaw(float yaw) { jointBuffer->Camera->Yaw = yaw; }
	void setCameraPitch(float pitch) { jointBuffer->Camera->Pitch = pitch; }
	void setCameraZoom(float zoom) { jointBuffer->Camera->Zoom = zoom; }

	//intesection functions
	void initIntersectionEngine();
	bool getIntersectionState();
	bool checkIntersectionObjects(std::string& name_object_1, std::string& name_object_2);
	void circleIntersectionEngine();
	void joinIntersectionThread();
	void detachIntersectionThread();
	void setGlobalAppState(bool on_off);
private:
	void initEngine(unsigned short width, unsigned short height, const char* appName);
	//intesection functions
	void updateIntersections();
	bool checkIntersectionSphere(std::string& name_object_1, std::string& name_object_2);
	bool checkIntersectionCube(std::string& name_object_1, std::string& name_object_2);
	bool checkIntersectionPlane(std::string& name_object_1, std::string& name_object_2);
	bool spawnAnimationCamera_State = false;
};