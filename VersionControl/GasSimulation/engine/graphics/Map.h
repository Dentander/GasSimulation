#pragma once
#include <vector>
#include "../math/vec3.h"
#include "OutputShader.h"
#include "objects/Sphere.h"
#include "objects/Box.h"
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>


class Map {
public:
    Map();

    vec3f spheresColor = vec3f(1, 0, 0);

    std::vector<Sphere> spheres;
    std::vector<Box> boxes;

    // ======== ДОБАВЛЕНИЕ ОБЪЕКТОВ ========
    void addSphere(vec3f pos, float radius, float mass, vec3f vel); // добавить сферу
    void addSphere(Sphere& sphere); // добавить сферу

    void addBox(vec3f pos, vec3f size, float color); // добавить стену
    void addBox(Box& box); // добавить стену

    // ======== ОБНОВЛЕНИЕ КОЛЛИЗИИ ========
    void update(sf::RenderWindow& window);

    // ======== ОБНОВЛЕНИЯ 3D ШЕЙДЕРА ========
    void setUniforms(OutputShader& shader) const;
};

