#include "Box.h"


Box::Box(vec3f pos, vec3f size, float colorIndex) : size(size), colorIndex(colorIndex) {
    this->pos = pos;
}

float SIGN(float num) {
    if (num > 0) { return 1; }
    return -1;
}

vec3f Box::getNormal(vec3f point) {
    vec3f normal;
    vec3f localPoint = point - pos;
    float min = std::numeric_limits<float>::max();
    float distance = std::abs(size.x - std::abs(localPoint.x));
    if (distance < min) {
        min = distance;
        normal = vec3f(1, 0, 0);
        normal = normal * SIGN(localPoint.x);
    }
    distance = std::abs(size.y - std::abs(localPoint.y));
    if (distance < min) {
        min = distance;
        normal = vec3f(0, 1, 0);
        normal = normal * SIGN(localPoint.y);
    }
    distance = std::abs(size.z - std::abs(localPoint.z));
    if (distance < min) {
        min = distance;
        normal = vec3f(0, 0, 1);
        normal = normal * SIGN(localPoint.z);
    }
    return normal;
}

vec3f Box::getNormal2(vec3f point) {
    vec3f normal = vec3f(1);
    vec3f topRight = pos + size * 0.5;
    vec3f bottomLeft = pos - size * 0.5;
    if (point.x > bottomLeft.x and point.x < topRight.x) {
        normal.x = 0;
    }
    if (point.y > bottomLeft.y and point.y < topRight.y) {
        normal.y = 0;
    }
    if (point.z > bottomLeft.z and point.z < topRight.z) {
        normal.z = 0;
    }
    if (point.x < bottomLeft.x) {
        normal.x *= -1;
    }
    if (point.y < bottomLeft.y) {
        normal.y *= -1;
    }
    if (point.z < bottomLeft.z) {
        normal.z *= -1;
    }
    return normal;
}
