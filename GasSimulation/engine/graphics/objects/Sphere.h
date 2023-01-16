#pragma once
#include "Box.h"


class Sphere : public Object {
public:
    float radius = 1;
    float mass = 1;
    vec3f velocity = vec3f(1);
    vec3f color = vec3f(1, 0, 0);

    Sphere(vec3f pos, float radius, float mass, vec3f velocity, vec3f color);

    void update();
    void boxCollision(Box& box);
};
