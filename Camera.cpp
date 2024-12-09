#include "Camera.h"
#include <string>
#include <sstream>

Camera::Camera() {
    position = Vec3(0, 12, 25);
    euler = Vec3(0, 0, 0);
    orientation = Quaternion::fromEulerAngle(euler);

    speed = 8.f;
    flag = true;
    
}

void Camera::updateRotation(float dt, Window& win) {
    float mousex = win.mousex;
    float mousey = win.mousey;
    
    // initialize mouse position for first time
    if (flag) {
        mouseX_last = mousex;
        mouseY_last = mousey;
        flag = false;
    }
    
    // calculate mouse movement(to determine rotate)  
    float rotateX = (mousex - mouseX_last) ;
    float rotateY = (mousey - mouseY_last);

    //ignore small movement
    float threshold = 0.01f; 
    if (abs(rotateX) < threshold) rotateX = 0;
    if (abs(rotateY) < threshold) rotateY = 0;

    euler += Vec3(-rotateX, 0, 0) * dt * 60.0; // yaw: rotate around axis-x
    euler += Vec3(0, -rotateY, 0) * dt * 60.0; // pitch： rotate around axis-y
    orientation = Quaternion::fromEulerAngle(euler).normalized();
    
    // save mouse pos for next frame
    mouseX_last = mousex;
    mouseY_last = mousey;
}


void Camera::updateTranslation(float dt, Window& win, bool ifCollided) {
    float move = speed * dt;
    // transform froward, right direction from orientation
    Vec3 forward = orientation * Vec3(0, 0, -1);
    Vec3 right = orientation * Vec3(1, 0, 0);
    Vec3 pos_new = position;

    // constrain movement to horizontal plane
    forward.y = 0.f;
    right.y = 0.f;

    if (win.keys['W']) {
        pos_new += forward * move;
    }
    if (win.keys['S']) {
        pos_new -= forward * move;
    }
    if (win.keys['A']) {
        pos_new -= right * move;
    }
    if (win.keys['D']) {
        pos_new += right * move;
    }
    // if player collided with object, then move away from it
    if(!ifCollided) {
        position = pos_new;
    }
    else {
        if (win.keys['W']) {
            position -= forward * move * 5;
        }
        else if (win.keys['S']) {
            position += forward * move * 5;
        }
        else if (win.keys['A']) {
            position += right * move * 5;
        }
        else if (win.keys['D']) {
            position -= right * move * 5;
        }
    }

}

// calculate lookAtMatrix for rendering 
Matrix Camera::getLookAtMat() {
    // rotate the direction of forward and up 
    Vec3 forward = orientation * Vec3(0, 0, -1);
    Vec3 up = orientation * Vec3(0, 1, 0);
    Vec3 right = orientation * Vec3(1, 0, 0);

    Matrix m;
    m = m.lookAtMat(position, position + forward, Vec3(0, 1,0));
    return m;
}