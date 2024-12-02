#pragma once
#include "mathLibrary.h"
#include "Window.h"

class Camera {
public:
	Vec3 position;
    //quaternion
	Vec3 forward;
	Vec3 up;
    float step;

	Camera() {
		position = Vec3(20, 5, 20);
		forward = Vec3(0, 0, -5);
		up = Vec3(0, 1, 0);
        step = 0.01f;
	}
	void updateMovement(float dt, Window& win) {
        //int move = static_cast<int>(speed * dt);
        Vec3 right_tmp = Vec3(1, 0, 0);
        Vec3 forward_tmp = Vec3(0, 0, -1);
        Vec3 up_tmp = Vec3(0, 1, 0);

        if (win.keys['W']) {
            position += forward_tmp * step;
            //position += (Vec3(0, 0, -0.01) * );
        }
        if (win.keys['S']) {
            position -= forward_tmp * step;
            //position -= Vec3(0, 0, -0.1);
        }
        if (win.keys['A']) {
            position -= right_tmp * step;
            //position -= Vec3(0.1, 0, 0);
            
        }
        if (win.keys['D']) {
            position += right_tmp * step;
        }
        //Vec3 pos = position;

	}
};