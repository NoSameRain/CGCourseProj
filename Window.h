#pragma once
#include <Windows.h>
#include <iostream>

class Window
{
public:
	HWND hwnd;
	HINSTANCE hinstance;
	std::string name;
	float width;
	float height;
	DWORD style;

	bool keys[256];
	int mousex;
	int mousey;
	bool mouseButtons[3];

	
	void create(float window_width, float window_height, std::string window_name);
	void updateMouse(int x, int y) {
		mousex = x;
		mousey = y;
	}
	void processMessages();
};

