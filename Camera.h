#pragma once
#ifndef CAMERA_H
#define CAMERA_H
#include "mathLibrary.h"
#include "Window.h"

class Camera {
public:
	Vec3 position;
    Quaternion orientation; 
	Matrix rotation;
	Vec3 euler;
    float speed;
	float mouseX_last, mouseY_last;
	bool flag;

	Camera();
	void updateTranslation(float dt, Window& win, bool ifCollided);
	void updateRotation(float dt, Window& win);
	Matrix getLookAtMat();
};

#endif