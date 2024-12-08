#include "Camera.h"
#include <string>
#include <sstream>

//std::wostringstream woss;
//void debugMessage(const std::wstring& message) {
//    OutputDebugString(message.c_str());
//}

Camera::Camera() {
    position = Vec3(0, 12, 25);
    euler = Vec3(0, 0, 0);
    orientation = Quaternion::fromEulerAngle(euler);

    speed = 0.03f;
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
    float rotateX = (mousex - mouseX_last) ;
    float rotateY = (mousey - mouseY_last);

    //ignore tiny rotation
    float threshold = 0.01f; 
    if (abs(rotateX) < threshold) rotateX = 0;
    if (abs(rotateY) < threshold) rotateY = 0;

    euler += Vec3(-rotateX, 0, 0) * dt * 60.0;
    euler += Vec3(0, -rotateY, 0) * dt * 60.0;
    orientation = Quaternion::fromEulerAngle(euler).normalized();
    
    mouseX_last = mousex;
    mouseY_last = mousey;
}


void Camera::updateTranslation(float dt, Window& win, bool ifCollided) {
    //float move = static_cast<int>(speed * dt);

    // transformDirection
    Vec3 forward = orientation * Vec3(0, 0, -1);
    Vec3 right = orientation * Vec3(1, 0, 0);
    Vec3 pos_new = position;
    forward.y = 0.f;
    right.y = 0.f;

    if (win.keys['W']) {
        pos_new += forward * speed;
    }
    if (win.keys['S']) {
        pos_new -= forward * speed;
    }
    if (win.keys['A']) {
        pos_new -= right * speed;
    }
    if (win.keys['D']) {
        pos_new += right * speed;
    }
    if (!ifCollided) {
        position = pos_new;
    }
}

Matrix Camera::getLookAtMat() {
    // rotate the direction of forward and up 
    Vec3 forward = orientation * Vec3(0, 0, -1);
    Vec3 up = orientation * Vec3(0, 1, 0);
    Vec3 right = orientation * Vec3(1, 0, 0);

    Matrix m;
    m = m.lookAtMat(position, position + forward, Vec3(0, 1,0));
    /*woss << L"pos: (" << position.x << L", " << position.y << L", " << position.z <<  L")\n";
    debugMessage(woss.str());*/
    return m;
}