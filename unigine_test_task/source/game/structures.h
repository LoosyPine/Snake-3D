#pragma once
/////////////////////////////////////
struct Coords {
	float x, y, z;
};

struct Color {
	float r, g, b;
};
/////////////////////////////////////
struct AppInfo
{
	struct Resolution {
		unsigned short width, height;
	};
	Resolution Resolution;

	const char* appName;
};

struct ObjectInfo
{
	std::string name = "";
	//float scale = 0.0f;
	Coords scale = { 0.0f, 0.0f, 0.0f };
	Coords position = { 0.0f, 0.0f, 0.0f };
	Coords rotation = { 0.0f, 0.0f, 0.0f };
	Color color = { 0.0f, 0.0f, 0.0f };
};

struct CamInfo
{
	Coords pos;
	float yaw;
	float pitch;
};