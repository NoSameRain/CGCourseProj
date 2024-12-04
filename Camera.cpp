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
    
    // calculate mouse movement  
    float moveX = (mousex - mouseX_last) * M_PI / 180.f * 0.1;
    float moveY = (mousey - mouseY_last) * M_PI / 180.f * 0.1;

    //ignore tiny rotation
    float threshold = 0.01f; 
    if (abs(moveX) < threshold) moveX = 0;
    if (abs(moveY) < threshold) moveY = 0;
    woss << L"mouse: (" << win.mousex << L", " << mouseX_last <<  L")\n";
    debugMessage(woss.str());

    float pitchLimit = M_PI / 2.0f - 0.1f; // restrict rotation to -90 ~ 90
    Vec3 eulerAngle = orientation.toEulerAngle();
    float newPitch = clamp(eulerAngle.x + moveY, -pitchLimit, pitchLimit);
    moveY = newPitch - eulerAngle.x;


    // quaternion for yaw rotation: around Y-axis
    Quaternion yaw = yaw.fromAxisAngle(Vec3(0, 1, 0), moveX);
    // quaternion for pitch rotation: around X-axis
    Quaternion pitch = pitch.fromAxisAngle(Vec3(1, 0, 0), moveY);

    orientation = pitch *(yaw * orientation);
    orientation.normalized();
    
    rotation = orientation.toMatrix();

    mouseX_last = mousex;
    mouseY_last = mousey;

    std::wostringstream woss;
    //woss << L"Orientation: " << orientation.a << L", " << orientation.b << L", " << orientation.c << L", " << orientation.d << L"\n";
   // debugMessage(woss.str());
}

//void Camera::updateRotation(float dt, Window& win) {
//
//    // 定义每次旋转的角度步进值（敏感度）
//    float rotationSpeed = M_PI / 180.0f * 45.0f; // 每秒旋转45度（可调整）
//
//    // 初始化旋转角度
//    float moveX = 0.0f;
//    float moveY = 0.0f;
//
//    // 根据按键更新旋转角度
//    if (win.keys[VK_UP]) {         // 上键控制向上旋转（Pitch增加）
//        moveY += rotationSpeed ;
//    }
//    if (win.keys[VK_DOWN]) {       // 下键控制向下旋转（Pitch减少）
//        moveY -= rotationSpeed ;
//    }
//    if (win.keys[VK_LEFT]) {       // 左键控制向左旋转（Yaw减少）
//        moveX -= rotationSpeed ;
//    }
//    if (win.keys[VK_RIGHT]) {      // 右键控制向右旋转（Yaw增加）
//        moveX += rotationSpeed ;
//    }
//
//    // 忽略微小的旋转
//    float threshold = 0.001f;
//    if (abs(moveX) < threshold) moveX = 0;
//    if (abs(moveY) < threshold) moveY = 0;
//
//    // 限制 Pitch 范围到 ±90°
//    float pitchLimit = M_PI / 2.0f - 0.1f; // 限制到接近 ±90°
//    Vec3 eulerAngle = orientation.toEulerAngle();
//    float newPitch = clamp(eulerAngle.x + moveY, -pitchLimit, pitchLimit);
//    moveY = newPitch - eulerAngle.x; // 调整有效旋转量
//
//    // 创建四元数旋转
//    Quaternion yaw = yaw.fromAxisAngle(Vec3(0, 1, 0), -moveX);  // 绕Y轴（Yaw）
//    Quaternion pitch = pitch.fromAxisAngle(Vec3(1, 0, 0), moveY); // 绕X轴（Pitch）
//
//    // 更新相机的方向
//    orientation = pitch * (yaw * orientation);
//    orientation.normalized();
//
//    // 将四元数转换为矩阵
//    rotation = orientation.toMatrix();
//}


void Camera::updateTranslation(float dt, Window& win) {
    //float move = static_cast<int>(speed * dt);

    // transformDirection
    Vec3 forward = rotation.mulVec(Vec3(0, 0, -1));
    Vec3 right = rotation.mulVec(Vec3(1, 0, 0));  

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
    //woss << L"pos: (" << position.x << L", " << position.y << L", " << position.z <<  L")\n";
    //debugMessage(woss.str());
}

Matrix Camera::getLookAtMat() {
    // rotate the direction of forward and up 
    Vec3 forward = rotation.mulVec(Vec3(0, 0, -1));
    Vec3 up = rotation.mulVec(Vec3(0, 1, 0));
    
    Matrix m;
    m = m.lookAtMat(position, position + forward, Vec3(0, 1, 0));
    return m;
}