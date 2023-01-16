#pragma once
#include "Input.h"
#include "OutputShader.h"


class Camera {
private:
    Input& input = Input::instance();
    PlayerControls& playerControls = PlayerControls::instance();

    vec3f position = vec3f(0, 0, -3);
    vec2f rotation = vec2f(0);

    void updateRotation();
    void updatePosition();

public:
    Camera();
    explicit Camera(vec3f position, vec2f rotation);

    void update();

    void setUniforms(OutputShader& shader);
    void setPosition(vec3f position);
    void setRotation(vec2f rotation);
    void addPosition(vec3f position);
    void addRotation(vec2f rotation);
    vec3f getPosition();
    vec2f getRotation();
};
