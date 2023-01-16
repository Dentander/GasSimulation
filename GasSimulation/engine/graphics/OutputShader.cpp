#include "OutputShader.h"
#include <iostream>


OutputShader::OutputShader(vec2f size) {
    build(size);
}

void OutputShader::build(vec2f size) {
    outputShader.loadFromFile("glsl/Fragment.glsl", sf::Shader::Fragment);
    setUniform("u_resolution", size);
    outputRectangle.setSize(size.toSFML());
}

void OutputShader::draw(sf::RenderWindow& window) {
    setUniform("u_resolution", vec2f(window.getSize().x, window.getSize().y));
    window.draw(outputRectangle, &outputShader);
}

void OutputShader::setUniform(const std::string& name, float uniform) {
    outputShader.setUniform(name, uniform);
}

void OutputShader::setUniform(const std::string& name, int uniform) {
    outputShader.setUniform(name, uniform);
}

void OutputShader::setUniform(const std::string& name, vec2f uniform) {
    outputShader.setUniform(name, uniform.toSFML());
}

void OutputShader::setUniform(const std::string& name, vec3f uniform) {
    outputShader.setUniform(name, uniform.toSFML());
}

void OutputShader::setUniform(const std::string& name, const std::vector<float>& arr) {
    
    outputShader.setUniformArray(name, arr.data(), arr.size());
}

void OutputShader::setUniform(const std::string& name, const std::vector<sf::Glsl::Vec3>& arr) {
    outputShader.setUniformArray(name, arr.data(), arr.size());
}





