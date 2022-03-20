#include "quat.h"
#include <cmath>

quat angle_axis(float angle, const vec3 &axis)
{
    vec3 norm = normalized(axis);
    float s = sinf(angle * 0.5f);

    return quat(
        norm.x * s,
        norm.y * s,
        norm.z * s,
        cosf(angle * 0.5f));
}

quat fromTo(const vec3 &from, const vec3 &to)
{
    vec3 f = normalized(from);
    vec3 t = normalized(to);
    if (f == t)
    {
        return quat();
    }
    else if (f == (t * -1.0f))
    {
        vec3 ortho = vec3(1, 0, 0);

        if (fabsf(f.y) < fabsf(f.x))
        {
            ortho = vec3(0, 1, 0);
        }
        if (fabsf(f.z) < fabsf(f.y) && fabs(f.z) < fabsf(f.x))
        {
            ortho = vec3(0, 0, 1);
        }

        vec3 axis = normalized(cross(f, ortho));
        return quat(axis.x, axis.y, axis.z, 0);
    }
    else
    {
        vec3 half = normalized(f + t);
        vec3 axis = cross(f, half);
        return quat(axis.x, axis.y, axis.z, dot(f, half));
    }
}

vec3 getAxis(const quat &quat)
{
    return normalized(vec3(quat.x, quat.y, quat.z));
}

float getAngle(const quat &quat)
{
    return 2.0f * acosf(quat.w);
}

quat operator+(const quat &a, const quat &b)
{
    return quat(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

quat operator-(const quat &a, const quat &b)
{
    return quat(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

quat operator*(const quat &a, float b)
{
    return quat(a.x * b, a.y * b, a.z * b, a.w * b);
}

quat operator*(const quat &a, const quat& b) {
    return quat(
        (b.x * a.w) + (b.y * a.z) - (b.z * a.y) + (b.w*a.x),
        (-b.x * a.z) + (b.y * a.w) + (b.z * a.x) + (b.w * a.y),
        (b.x * a.y) - (b.y * a.x) + (b.z * a.w) + (b.w * a.z),
        (-b.x * b.x) - (b.y * a.y) - (b.z * a.z) + (b.w * a.w)
    );
}

quat operator-(const quat &q)
{
    return quat(-q.x, -q.y, -q.z, -q.w);
}

bool operator==(const quat &a, const quat &b)
{
    return (fabsf(a.x - b.x) <= QUAT_EPSILON) && (fabsf(a.y - b.y) <= QUAT_EPSILON) && (fabsf(a.z - b.z) <= QUAT_EPSILON) && (fabsf(a.w - b.w) <= QUAT_EPSILON);
}

bool operator!=(const quat &a, const quat &b)
{
    return !(a == b);
}

bool sameOrientation(const quat &a, const quat &b)
{
    return ((fabsf(a.x - b.x) <= QUAT_EPSILON) && (fabsf(a.y - b.y) <= QUAT_EPSILON) && (fabsf(a.z - b.z) <= QUAT_EPSILON) && (fabsf(a.w - b.w) <= QUAT_EPSILON)) || ((fabsf(a.x + b.x) <= QUAT_EPSILON) && (fabsf(a.y + b.y) <= QUAT_EPSILON) && (fabsf(a.z + b.z) <= QUAT_EPSILON) && (fabsf(a.w + b.w) <= QUAT_EPSILON));
}

float dot(const quat &a, const quat &b)
{
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}

float lenSq(const quat &q)
{
    return (q.x * q.x) + (q.y * q.y) + (q.z * q.z) + (q.w * q.w);
}

float len(const quat &q)
{
    float lenSqr = lenSq(q);
    if (lenSqr < QUAT_EPSILON)
    {
        return 0.0f;
    }
    return sqrtf(lenSqr);
}

void normalize(quat &q)
{
    float length = len(q);
    if (length < QUAT_EPSILON)
    {
        return;
    }
    float il = 1.0f / length;

    q.x *= il;
    q.y *= il;
    q.z *= il;
    q.w *= il;
}

quat normalized(const quat &q)
{
    float length = len(q);
    if (length < QUAT_EPSILON)
    {
        return;
    }
    float il = 1.0f / length;

    return quat(
        q.x * il,
        q.y * il,
        q.z * il,
        q.w * il);
}

quat conjugate(const quat &q)
{
    return quat(-q.x, -q.y, -q.z, -q.w);
}

quat inverse(const quat &q)
{
    float length = len(q);
    if (length < QUAT_EPSILON)
    {
        return quat();
    }
    float il = 1.0f / length;
    return quat(-q.x * il, -q.y * il, -q.z * il, -q.w * il);
}
