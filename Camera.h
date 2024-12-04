#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "mathLibrary.h"
#include "Window.h"

class Camera {
public:
	Vec3 position;
    Quaternion orientation; 
	Vec3 rot;

    float scale;
	float mouseX_last, mouseY_last;
	bool flag;

	Camera();
	void updateTranslation(float dt, Window& win);
	void updateRotation(float dt, Window& win);
	Matrix getLookAtMat();
};

#endif