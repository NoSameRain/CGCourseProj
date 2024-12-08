#pragma once
#include "mathLibrary.h"


class AABB
{
public:
    Vec3 max_;
    Vec3 min_;
    AABB() { reset(); }
    void reset();
    void extend(const Vec3& p);

    // if the model is scaled, collision box should also be scaled
    void transformAABB(const Vec3& scale, const Vec3& translation);
    void updateAABB(const Vec3& translation);
    bool collisionCheck(AABB& box2);
    AABB& operator=(const AABB& box) { max_ = box.max_; min_ = box.min_; return *this; }
};


class Sphere_collision
{
public:
    Vec3 centre;
    float radius;
};
