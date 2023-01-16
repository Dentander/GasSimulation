#include "Map.h"


Map::Map() = default;

void Map::addSphere(vec3f pos, float radius, float mass, vec3f vel) {
    spheres.emplace_back(pos, radius, mass, vel, spheresColor);
}

void Map::addSphere(Sphere & sphere) {
    spheres.push_back(sphere);
}


void Map::addBox(vec3f pos, vec3f size, float color) {
    boxes.emplace_back(pos, size, color);
}

void Map::addBox(Box & box) {
    boxes.push_back(box);
}


void Map::update(sf::RenderWindow & window) {
    std::for_each(spheres.begin(), spheres.end(), [](Sphere& sphere) { sphere.update(); });
}

void Map::setUniforms(OutputShader& shader) const {

    std::vector<sf::Glsl::Vec3> spheresPos;
    std::vector<float> spheresRadius;
    for (auto& item : spheres) {
        spheresPos.push_back(item.pos.toSFML());
        spheresRadius.push_back(item.radius);
    }

    std::vector<sf::Glsl::Vec3> boxPos;
    std::vector<sf::Glsl::Vec3> boxSize;
    std::vector<float> boxColor;
    for (auto& item : boxes) {
        boxPos.push_back(item.pos.toSFML());
        boxSize.push_back(item.size.toSFML());
        boxColor.push_back(item.colorIndex);
    }


    shader.setUniform("u_sphere_count", int(spheres.size()));
    shader.setUniform("u_sphere_pos", spheresPos);
    shader.setUniform("u_sphere_radius", spheresRadius);
    shader.setUniform("u_sphere_color", spheresColor);

    shader.setUniform("u_box_count", int(boxes.size()));
    shader.setUniform("u_box_pos", boxPos);
    shader.setUniform("u_box_size", boxSize);
    shader.setUniform("u_box_color", boxColor);
}
