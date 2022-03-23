#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vec3.h"
#include "quat.h"
#include <cmath>

struct Transform {
    vec3 position;
    quat rotation;
    vec3 scale;

    Transform(const vec3& p, const quat& r, const vec3& s): position(p), rotation(r), scale(s) {}
    Transform(): position(vec3(0, 0, 0)), rotation(quat(0, 0, 0, 1)), scale(vec3(1, 1, 1)) {}
};

Transform combine(const Transform& a, const Transform& b);
Transform inverse(const Transform& t);
Transform mix(const Transform& a, const Transform &b, float dt);
mat4 transformToMat4(const Transform& t);
Transform mat4ToTransform(const mat4& m);
vec3 transform_point(const Transform& a, const vec3& b);
vec3 transform_vector(const Transform& a, const vec3& b);

#endif