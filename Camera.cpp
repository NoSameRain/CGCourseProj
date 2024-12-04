#include "Camera.h"
#include <string>
#include <sstream>

std::wostringstream woss;
void debugMessage(const std::wstring& message) {
	OutputDebugString(message.c_str());
}

Camera::Camera() {
	position = Vec3(0, 5, 20);
	rot = Vec3(0, 0, 0);
	orientation = Quaternion::fromEulerAngle(rot);
	scale = 0.01f;
	flag = true;
}

void Camera::updateRotation(float dt, Window& win) {
	float mousex = win.mousex;
	float mousey = win.mousey;

	if (flag) {
		mouseX_last = mousex;
		mouseY_last = mousey;
		flag = false;
	}

	// calculate mouse movement(to rotate)  
	float rotateX = (mousex - mouseX_last);
	float rotateY = -(mousey - mouseY_last);

	rot += Vec3(rotateX, rotateY, 0) * dt * 1000;
	orientation = Quaternion::fromEulerAngle(rot);

	mouseX_last = mousex;
	mouseY_last = mousey;
}


void Camera::updateTranslation(float dt, Window& win) {
	//float move = static_cast<int>(speed * dt);

	// transformDirection
	Vec3 forward = orientation * Vec3(0, 0, -1);// -1
	Vec3 right = orientation * Vec3(-1, 0, 0);
	forward.y = 0.f;
	right.y = 0.f;

	if (win.keys['W']) {
		position += forward * scale;
	}
	if (win.keys['S']) {
		position -= forward * scale;
	}
	if (win.keys['A']) {
		position -= right * scale;
	}
	if (win.keys['D']) {
		position += right * scale;
	}

}

Matrix Camera::getLookAtMat() {
	// rotate the direction of forward and up 
	Vec3 forward = orientation * Vec3(0, 0, -1);
	Vec3 up = orientation * Vec3(0, 1, 0);
	Vec3 right = orientation * Vec3(1, 0, 0);

	Matrix m;
	m = m.lookAtMat(position, position + forward, Vec3(0, 1, 0));
	/*woss << L"pos: (" << position.x << L", " << position.y << L", " << position.z <<  L")\n";
	debugMessage(woss.str());*/
	return m;
}