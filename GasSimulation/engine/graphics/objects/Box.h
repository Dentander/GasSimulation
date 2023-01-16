#pragma once
#include "Object.h"


class Box : public Object {
public:
    vec3f size = vec3f(1);
    float colorIndex = 1;

    Box(vec3f pos, vec3f size, float colorIndex);
    vec3f getNormal(vec3f point);
    vec3f getNormal2(vec3f point);
};
