#pragma once
#include <SFML/Graphics.hpp>
#include "../math/vec2.h"
#include "../math/vec3.h"


class OutputShader {
private:
    sf::RectangleShape outputRectangle;
    sf::Shader outputShader;

    std::string frameShader;

public:
    OutputShader() = default;
    explicit OutputShader(vec2f size);

    void build(vec2f size);
    void draw(sf::RenderWindow& window);

    void setUniform(const std::string& name, float uniform);
    void setUniform(const std::string& name, int uniform);
    void setUniform(const std::string& name, vec2f uniform);
    void setUniform(const std::string& name, vec3f uniform);
    void setUniform(const std::string& name, const std::vector<float>& arr);
    void setUniform(const std::string& name, const std::vector<sf::Glsl::Vec3>& arr);
};
