#pragma once

#include "game/core.h"

// Доброго времени суток, буду краток.
// Моя работа не иедальна. Есть куча косяков и слабых мест.
// Из глобальных это не правильный "движок" колизий.
// Была большая спешка, а желаний было очень много(в итоге приоритет был поставлен на работоспособность).
// Но выводы я сделал, буду рад дальнейшее критике.

/*
* Coordinate system:
* x - right
* y - up
* z - backward
*/

// Ещё одно предисловие.
// Т.к. движку колизий плохо и стенки выставлять не удобно(т.к. по факту у всех фигур в игре груглые хитбоксы)
// Я решил сыграть от обратного и поставил огромный куб с которым постоянно контакрируте змейка.
// Если она выйдет за его рамки и контакт прервёся, то на змейку дествует "сила притяжения" и она падает.
// В итоге теперь змейка ползает на листочке(что-то по типу листка кувшинки), который окружён водой.
// Если сползает с листа, то закономерно тонет. Вот такая вот концепция)))
// Не нужно удивляться, что на WASD двигается часть листка есть специальная функция scopeCoordsObject(), она сделана чисто для удобства(при движении выводит координаты в консоль, можно хорошо подогнать фигуры в общей экпозиции).
// Основное управление на стрелки!
int main()
{
	// init core
	AppInfo* appInfo = new AppInfo;
	appInfo->appName = "Snake Game";
	appInfo->Resolution.width = 1280;
	appInfo->Resolution.height = 720;
	Core* core = Core::get();
	core->init(appInfo);
	Intersection intersection;

	// init camera
	CamInfo* camInfo = new CamInfo;
	camInfo->pos = { 0.0f, 0.0f, 0.0f };
	camInfo->yaw = 0.0f;
	camInfo->pitch = -70.0f;
	core->initCamera(camInfo);

	// create snake
	ObjectInfo* snakeInfo = new ObjectInfo;
	snakeInfo->name = "part_snake_";
	snakeInfo->scale = { 1.0f, 1.0f, 1.0f };
	snakeInfo->position = { 0.0f, -1.0f, 0.0f };
	snakeInfo->rotation = { 0.0f, 0.0f, 0.0f };
	snakeInfo->color = { 0.0f, 0.75f, 0.0f };
	short snake_length = 3;
	std::unordered_map<std::string, Object*> snake;
	std::vector<glm::vec3> snakeTailCoords;
	for (short i = 0; i < snake_length; i++)
	{
		snakeInfo->name += std::to_string(i);
		snake.emplace(snakeInfo->name, core->addCube(snakeInfo));
		snake.find(snakeInfo->name)->second->setPosition(snakeInfo->position.x + i, snakeInfo->position.y, snakeInfo->position.z);
		snakeTailCoords.push_back(glm::vec3(0.0f, 0.5f + i, 0.0f));
		snakeInfo->name = "part_snake_";
	}

	
	// leafs
	ObjectInfo* groundInfo = new ObjectInfo;
	groundInfo->name = "ground";
	groundInfo->position = { 0.0f, -0.25f, 0.0f };
	groundInfo->rotation = { 0.0f, 0.0f, 0.0f };
	groundInfo->scale = { 19.3f, 0.15f, 20.0f };
	groundInfo->color = { 0.5f, 1.0f, 0.3f };
	Object* ground = core->addSphere(groundInfo);
	groundInfo->name = "ground1";
	groundInfo->position = { 0.0f, -0.25f, 0.0f };
	groundInfo->rotation = { 0.0f, 0.0f, 0.0f };
	groundInfo->scale = { 25.5f, 0.15f, 20.0f };
	groundInfo->color = { 0.5f, 1.0f, 0.3f };
	Object* ground1 = core->addSphere(groundInfo);
	groundInfo->name = "ground2";
	groundInfo->position = { 0.0f, -0.25f, 0.0f };
	groundInfo->rotation = { 0.0f, 0.0f, 0.0f };
	groundInfo->scale = { 27.5f, 0.15f, 14.0f };
	groundInfo->color = { 0.5f, 1.0f, 0.3f };
	Object* ground2 = core->addSphere(groundInfo);
	groundInfo->name = "ground3";
	groundInfo->position = { 0.0f, -0.25f, -5.0f };
	groundInfo->rotation = { 0.0f, 0.0f, 0.0f };
	groundInfo->scale = { 25.5f, 0.15f, 20.0f };
	groundInfo->color = { 0.5f, 1.0f, 0.3f };
	Object* ground3 = core->addSphere(groundInfo);
	groundInfo->name = "ground4";
	groundInfo->position = { 0.0f, -0.15f, -5.0f };
	groundInfo->rotation = { 0.0f, 0.0f, 0.0f };
	groundInfo->scale = { 25.5f, 0.15f, 20.0f };
	groundInfo->color = { 0.5f, 1.0f, 0.3f };
	Object* ground4 = core->addSphere(groundInfo);

	// waves
	ObjectInfo* waveInfo = new ObjectInfo;
	waveInfo->name = "wave";
	waveInfo->position = { -10.0f, -0.50f, 0.0f };
	waveInfo->rotation = { 0.0f, 0.0f, 0.0f };
	waveInfo->scale = { 25.5f, 0.15f, 20.0f };
	waveInfo->color = { 0.0f, 0.3f, 1.0f };
	Object* wave = core->addSphere(waveInfo);
	waveInfo->name = "wave1";
	waveInfo->position = { 10.0f, -0.50f, 0.0f };
	waveInfo->rotation = { 0.0f, 0.0f, 0.0f };
	waveInfo->scale = { 25.5f, 0.15f, 20.0f };
	waveInfo->color = { 0.0f, 0.3f, 1.0f };
	Object* wave1 = core->addSphere(waveInfo);
	waveInfo->name = "wave2";
	waveInfo->position = { 0.0f, -0.50f, -10.0f };
	waveInfo->rotation = { 0.0f, 0.0f, 0.0f };
	waveInfo->scale = { 25.5f, 0.15f, 20.0f };
	waveInfo->color = { 0.0f, 0.3f, 1.0f };
	Object* wave2 = core->addSphere(waveInfo);
	waveInfo->name = "wave3";
	waveInfo->position = { 0.0f, -0.50f, 10.0f };
	waveInfo->rotation = { 0.0f, 0.0f, 0.0f };
	waveInfo->scale = { 25.5f, 0.15f, 20.0f };
	waveInfo->color = { 0.0f, 0.3f, 1.0f };
	Object* wave3 = core->addSphere(waveInfo);
	waveInfo->name = "wave4_background";
	waveInfo->position = { 0.0f, -0.50f, 10.0f };
	waveInfo->rotation = { 0.0f, 0.0f, 0.0f };
	waveInfo->scale = { 100.5f, 0.15f, 100.0f };
	waveInfo->color = { 0.0f, 0.3f, 1.0f };
	Object* wave4_background = core->addSphere(waveInfo);



	// create apple
	ObjectInfo* appleInfo = new ObjectInfo;
	appleInfo->name = "apple_";
	appleInfo->scale = { 1.0f, 1.0f, 1.0f };
	appleInfo->position = { 0.0f, 6.0f, 0.0f };
	appleInfo->rotation = { 0.0f, 0.0f, 0.0f };
	appleInfo->color = { 0.8f, 0.0f, 0.0f };
	short apple_max_count = 5;
	std::unordered_map<std::string, Object*> apple;
	srand(static_cast<unsigned int>(time(0)));
	for (short i = 0; i < apple_max_count; i++)
	{
		appleInfo->name += std::to_string(i);
		apple.emplace(appleInfo->name, core->addSphere(appleInfo));
		float rand_x = rand() % (int(ground->getScale().x + 1) / 2);
		float rand_z = rand() % (int(ground->getScale().z + 1) / 2);
		apple.find(appleInfo->name)->second->setPosition(rand_x, 0.0, rand_z);
		appleInfo->name = "apple_";
	}

	//core->setLightAmbientColor(glm::vec3(0.55f, 0.75f, 0.95f));

	ObjectInfo* bottomInfo = new ObjectInfo;
	bottomInfo->name = "invis_bottom";
	bottomInfo->position = { 0.0f, -13.7f, 0.0f }; //-17.2 // -14.1
	bottomInfo->rotation = { 0.0f, 0.0f, 0.0f };
	bottomInfo->scale = { 20.2f, 20.0f, 20.2f }; // 20.3f, 20.0f, 20.3f
	bottomInfo->color = { 0.0f, 0.0f, 1.0f };
	Object* invis_bottom = core->addCube(bottomInfo);


	

	glm::vec3 move = { 1.0f, 0.85f, 0.5f };
	glm::vec3 prevCoords;
	glm::vec3 prevCoords2;
	bool move_up_down_state = false;
	bool move_left_right_state = false;
	float move_z = 0.0f, move_x = 0.0f;
	float g_power = 0.0f;
	float move_speed = 0.1;
	core->setGlobalAppState(true);
	core->initIntersectionEngine();
	core->setCameraZoom(0);
	float zoom = 0;
	float cos = glm::cos(core->getTime());
	float sin = glm::sin(core->getTime());
	std::string snakeHead = "";
	std::string snakePart = "";
	std::string appleName = "";
	glm::vec3 partVec3 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 snakeVec3 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 appleVec3 = glm::vec3(0.0f, 0.0f, 0.0f);
	float rand_x = 0.0f;
	float rand_z = 0.0f;
	while (!core->isDone())
	{
		core->update();
		core->render();
		// sin and cos update
		cos = glm::cos(core->getTime());
		sin = glm::cos(core->getTime());

		//Camera setup
		core->setCameraPosition(glm::vec3(0.0f, 22.0f, -25.0f));
		core->setCameraYaw(90.0f);
		core->setCameraPitch(-45.0f);

		// animation of the camera appearance
		if (zoom < 36.99)
		{
			zoom = abs(glm::sin(core->getTime()) * 37);
		}
		core->setCameraZoom(zoom); // ~35,0f

		// animation leafs
		ground->setRotation(cos * 1.0f, 0.0f, sin * 1.0f);
		ground1->setRotation(cos * 1.0f, 0.0f, sin * 1.0f);
		ground2->setRotation(cos * 1.0f, 0.0f, sin * 1.0f);
		ground3->setRotation(sin * 1.0f, 0.0f, sin * 1.0f);
		ground4->setRotation(cos * 1.0f, 0.0f, cos * 1.0f);
		
		// animation waves
		wave->setRotation(cos * 0.7f, sin * 0.7f, cos * 0.7f);
		wave1->setRotation(cos * 0.7f, sin * 0.7f, sin * 0.7f);
		wave2->setRotation(cos * 0.7f, sin * 0.7f, sin * 0.7f);
		wave3->setRotation(cos * 0.7f, sin * 0.7f, sin * 0.7f);

		// snake(player) control
		if (core->isKeyPressed(GLFW_KEY_UP) && move_up_down_state == false) // UP
		{
			move_z = move_speed;
			move_x = 0.0f;
			move_up_down_state = true;
			move_left_right_state = false;
		}
		if (core->isKeyPressed(GLFW_KEY_DOWN) && move_up_down_state == false) // DOWN
		{
			move_z = -move_speed;
			move_x = 0.0f;
			move_up_down_state = true;
			move_left_right_state = false;
		}
		if (core->isKeyPressed(GLFW_KEY_LEFT) && move_left_right_state == false) // LEFT
		{
			move_x = move_speed;
			move_z = 0.0f;
			move_up_down_state = false;
			move_left_right_state = true;
		}
		if (core->isKeyPressed(GLFW_KEY_RIGHT) && move_left_right_state == false) // RIGHT
		{
			move_x = -move_speed;
			move_z = 0.0f;
			move_up_down_state = false;
			move_left_right_state = true;
		}
		core->scopeCoordsObject(ground);
		core->scopeCoordsObject(ground1);
		core->scopeCoordsObject(ground2);
		
		snakeHead = snakeInfo->name + std::to_string(0);
		if (core->checkIntersectionObjects(bottomInfo->name, snakeHead) != true && core->getIntersectionState() == true)
		{
			move_up_down_state = true;
			move_left_right_state = true;
			g_power = 0.8f;
			move_speed = 0.5f;
			if (move_x != 0.0f) move_x *= move_speed;
			else if (move_z != 0.0f) move_x *= move_speed;
		}
		for (short j = 0; j < apple_max_count; j++)
		{
			appleName = appleInfo->name + std::to_string(j);
			snakeHead = snakeInfo->name + std::to_string(0);
			appleVec3 = apple.find(appleName)->second->getPosition();
			snakeVec3 = snake.find(snakeHead)->second->getPosition();
			if ((appleVec3.x <= snakeVec3.x + apple.find(appleName)->second->getScale().x / 2.0f)
				&& (appleVec3.x >= snakeVec3.x - apple.find(appleName)->second->getScale().x / 2.0f)
				&& (appleVec3.z >= snakeVec3.z - apple.find(appleName)->second->getScale().z / 2.0f)
				&& (appleVec3.z <= snakeVec3.z + apple.find(appleName)->second->getScale().z / 2.0f))
			{
				//std::cout << "+LENGHT" << std::endl;
				++snake_length;
				snakeInfo->name = "part_snake_" + std::to_string(snake_length - 1);
				snake.emplace(snakeInfo->name, core->addCube(snakeInfo));
				snakeTailCoords.push_back(snakeTailCoords[snake_length - 2]);
				snakeInfo->name = "part_snake_";
				rand_x = (rand() % (int(invis_bottom->getScale().x + 1) / 2)) - rand() % (int(invis_bottom->getScale().x + 1) / 2);
				rand_z = (rand() % (int(invis_bottom->getScale().z + 1) / 2)) - rand() % (int(invis_bottom->getScale().z + 1) / 2);
				apple.find(appleName)->second->setPosition(rand_x, 0.0f, rand_z);
				std::cout << "SCORE:\t" << snake_length << std::endl;
			}
		}

		// chekc on contact "snake to snake"
		snakeInfo->name = "part_snake_";
		for (short j = 1; j < snake_length; j++)
		{
			snakeHead = snakeInfo->name + std::to_string(0);
			snakePart = snakeInfo->name + std::to_string(j);
			partVec3 = snake.find(snakePart)->second->getPosition();
			snakeVec3 = snake.find(snakeHead)->second->getPosition();
			if ((snakeVec3.x <= partVec3.x + 0.0 * snake.find(snakePart)->second->getScale().x / 8.0f)
				&& (snakeVec3.x >= partVec3.x - 0.0 * snake.find(snakePart)->second->getScale().x / 8.0f)
				&& (snakeVec3.z >= partVec3.z - 0.0 * snake.find(snakePart)->second->getScale().z / 8.0f)
				&& (snakeVec3.z <= partVec3.z + 0.0 * snake.find(snakePart)->second->getScale().z / 8.0f))
			{
				if (snake_length > 3)
				{
					//std::cout << "CONTACT!!!\t" << snakePart << std::endl;
					core->detachIntersectionThread();
					return 0;
				}
			}
		}

		//snake calcule
		move = glm::vec3(0.0f + move_x, 0.0f - g_power, 0.0f + 0 + move_z);
		prevCoords = snakeTailCoords[0];
		snakeTailCoords[0] += move;
		for (short i = 1; i < snakeTailCoords.size(); i++)
		{
			prevCoords2 = snakeTailCoords[i];
			snakeTailCoords[i] = prevCoords;
			prevCoords = prevCoords2;
		}
		//snake move
		short i = 0;
		for (auto iter : snake)
		{
			iter.second->setPosition(snakeTailCoords[i]);
			++i;
		}
		core->swap();
	}
	core->shutdown();
	return 0;
}
