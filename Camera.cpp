#include "Camera.h"
#include <string>
#include <sstream>

std::wostringstream woss;
void debugMessage(const std::wstring& message) {
    OutputDebugString(message.c_str());
}

Camera::Camera() {
    position = Vec3(0, 5, 20);
    orientation = Quaternion(1, 0, 0, 0);
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
    float rotateX = (mousex - mouseX_last) * M_PI / 180.f * 0.1;
    float rotateY = (mousey - mouseY_last) * M_PI / 180.f * 0.1;

    //ignore tiny rotation
    float threshold = 0.01f; 
    if (abs(rotateX) < threshold) rotateX = 0;
    if (abs(rotateY) < threshold) rotateY = 0;
    //woss << L"mouse: (" << win.mousex << L", " << mouseX_last <<  L")\n";
    //debugMessage(woss.str());

    //if new rotateY add orientation is over  -90 ~ 90 camera may up side down
    float pitchLimit = M_PI / 2.0f - 0.1f; // restrict rotation to -90 ~ 90
    Vec3 eulerAngle = orientation.toEulerAngle();
    float newPitch = clamp(eulerAngle.x + rotateY, -pitchLimit, pitchLimit);
    rotateY = newPitch - eulerAngle.x;

    //woss << L"Mouse movement: (" << moveX << L", " << moveY << L")\n";
    //debugMessage(woss.str());

    // quaternion for yaw rotation: around Y-axis
    Quaternion yaw = yaw.fromAxisAngle(Vec3(0, 1, 0), rotateX);
    // quaternion for pitch rotation: around X-axis
    Quaternion pitch = pitch.fromAxisAngle(Vec3(1, 0, 0), rotateY);

    orientation = pitch *(yaw * orientation);
    orientation.normalized();
    
    rotation = orientation.toMatrix();

    mouseX_last = mousex;
    mouseY_last = mousey;

    
    //woss << L"Orientation: " << orientation.a << L", " << orientation.b << L", " << orientation.c << L", " << orientation.d << L"\n";
    //debugMessage(woss.str());
}


void Camera::updateTranslation(float dt, Window& win) {
    //float move = static_cast<int>(speed * dt);

    // transformDirection
    Vec3 forward = rotation.mulVec(Vec3(0, 0, 1));// -1
    Vec3 right = rotation.mulVec(Vec3(1, 0, 0));  
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
    Vec3 forward = rotation.mulVec(Vec3(0, 0, 1));
    Vec3 up = rotation.mulVec(Vec3(0, -1, 0));
    
    Matrix m;
    m = m.lookAtMat(position, position + forward, Vec3(0, 1, 0));
    /*woss << L"pos: (" << position.x << L", " << position.y << L", " << position.z <<  L")\n";
    debugMessage(woss.str());*/
    return m;
}