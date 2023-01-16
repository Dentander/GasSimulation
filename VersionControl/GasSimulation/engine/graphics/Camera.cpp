#include <cmath>
#include "Camera.h"


Camera::Camera() {

}

Camera::Camera(vec3f position, vec2f rotation) {

}

void Camera::updateRotation() {
    vec2f deltaAngle = vec2f(input.deltaAngle.x * playerControls.mouseSensitivityX,
        -input.deltaAngle.y * playerControls.mouseSensitivityY);
    addRotation(deltaAngle);
}

void Camera::updatePosition() {
    // ======== MOVEMENT ========
    vec3f direction = vec3f(input.moveDirection.x, 0, input.moveDirection.y) * 0.1f;
    direction.rotate(getRotation().x, getRotation().y);
    vec3f deltaPos = direction.zyx();
    addPosition(deltaPos);
}

void Camera::update() {
    updateRotation();
    updatePosition();
    std::cout << position.x << " " << position.y << " " << position.z << std::endl;
}

vec3f Camera::getPosition() { return position; }

vec2f Camera::getRotation() { return rotation; }

void Camera::setPosition(vec3f _position) {
    position = _position;
}

void Camera::setRotation(vec2f _rotation) {
    rotation = _rotation;
}

void Camera::addPosition(vec3f _position) {
    position = position + _position;
}

void Camera::addRotation(vec2f _rotation) {
    rotation = rotation + _rotation;
}

void Camera::setUniforms(OutputShader& shader) {
    shader.setUniform("u_camera_position", getPosition());
    shader.setUniform("u_camera_rotation", getRotation());
}

