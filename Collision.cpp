#include "Collision.h"
#include <cfloat>
#include <string>
#include "Utils.h"
void AABB::reset()
{
    max_ = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    min_ = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
}
void AABB::extend(const Vec3& p)
{
    max_ = max_.Max(max_, p);
    min_ = min_.Min(min_, p);
}

// if the model is scaled, collision box should also be scaled
void AABB::transformAABB(const Vec3& scale, const Vec3& translation) {
    min_.x *= scale.x;
    min_.y *= scale.y;
    min_.z *= scale.z;

    max_.x *= scale.x;
    max_.y *= scale.y;
    max_.z *= scale.z;

    min_ += translation;
    max_ += translation;
    
    //debugLog("AABB pos: MAX: " + std::to_string(max_.x) + ", " + std::to_string(max_.y) + ", " + std::to_string(max_.z));
    //debugLog("AABB pos: MIN: " + std::to_string(min_.x) + ", " + std::to_string(min_.y) + ", " + std::to_string(min_.z));
}

void AABB::updateAABB(const Vec3& translation) {

    min_ += translation;
    max_ += translation;

    //debugLog("AABB pos: MAX: " + std::to_string(max_.x) + ", " + std::to_string(max_.y) + ", " + std::to_string(max_.z));
    //debugLog("AABB pos: MIN: " + std::to_string(min_.x) + ", " + std::to_string(min_.y) + ", " + std::to_string(min_.z));
}

bool AABB::collisionCheck(AABB& box2) {
    float buffer = 0.1f;
    float PDx = min(max_.x, box2.max_.x) - max(min_.x, box2.min_.x) - buffer;
    float PDy = min(max_.y, box2.max_.y) - max(min_.y, box2.min_.y) - buffer;
    float PDz = min(max_.z, box2.max_.z) - max(min_.z, box2.min_.z) - buffer;
    //debugLog("player: MAX: " + std::to_string(max_.x) + ", " + std::to_string(max_.y) + ", " + std::to_string(max_.z));
    //debugLog("player: MIN: " + std::to_string(min_.x) + ", " + std::to_string(min_.y) + ", " + std::to_string(min_.z));
    /*debugLog("NPC: MAX: " + std::to_string(box2.max_.x) + ", " + std::to_string(box2.max_.y) + ", " + std::to_string(box2.max_.z));
    debugLog("npc: MIN: " + std::to_string(box2.min_.x) + ", " + std::to_string(box2.min_.y) + ", " + std::to_string(box2.min_.z));*/
    //debugLog("----------------------");
    if (PDx > 0 && PDy > 0 && PDz > 0) {
        return true;
    }
    return false;
}

