#include "Collision.h"
#include <cfloat>

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

    Vec3 boxCenter = (min_ + max_) * 0.5f;

    Vec3 relativeMin = min_ - boxCenter;
    Vec3 relativeMax = max_ - boxCenter;

    min_ = boxCenter + Vec3(relativeMin.x * scale.x, relativeMin.y * scale.y, relativeMin.z * scale.z) + translation;
    max_ = boxCenter + Vec3(relativeMax.x * scale.x, relativeMax.y * scale.y, relativeMax.z * scale.z) + translation;
    //min_ = boxCenter + relativeMin;
    //max_ = boxCenter + relativeMax;
}

bool AABB::collisionCheck(AABB& box2) {
    float PDx = min(max_.x, box2.max_.x) - max(min_.x, box2.min_.x);
    float PDy = min(max_.y, box2.max_.y) - max(min_.y, box2.min_.y);
    float PDz = min(max_.z, box2.max_.z) - max(min_.z, box2.min_.z);
    if (PDx > 0 && PDy > 0 && PDz > 0) {
        return true;
    }
    return false;
}

