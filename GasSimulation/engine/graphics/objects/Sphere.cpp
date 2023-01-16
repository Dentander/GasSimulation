#include "Sphere.h"


Sphere::Sphere(vec3f pos, float radius, float mass, vec3f velocity, vec3f color) : radius(radius), mass(mass), velocity(velocity), color(color) {
    this->pos = pos;
}

void Sphere::update() {
    pos = pos + velocity;
}

void Sphere::boxCollision(Box& box) {
    //std::cout << box.getNormal(pos).x << ' ' << box.getNormal(pos).y << ' ' << box.getNormal(pos).z << "\n";
    velocity = velocity * -1;
    velocity = velocity.reflect(box.getNormal2(pos));
}
