#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#define vec2f vec2<float>
#define vec2i vec2<int>
#include <cmath>


template <typename T>
class vec2 {
public:
    T x, y;
    vec2(float value) : x(value), y(value) {}
    vec2(float _x, float _y) : x(_x), y(_y) {}
    vec2() : x(0), y(0) {}

    vec2 operator+(vec2 const& other) { return vec2(x + other.x, y + other.y); }
    vec2 operator-(vec2 const& other) { return vec2(x - other.x, y - other.y); }
    vec2 operator*(vec2 const& other) { return vec2(x * other.x, y * other.y); }
    vec2 operator/(vec2 const& other) { return vec2(x / other.x, y / other.y); }

    bool operator==(vec2 other) const { return x == other.x and y == other.y; }

    vec2 yx() { return vec2<T>(y, x); }
    float SquaredLength() const { return x * x + y * y; }
    sf::Vector2f toSFML() { return sf::Vector2f(x, y); }

    vec2 rotate(float angle) {
        float sinA = std::sin(angle), cosA = std::cos(angle);
        float px, py;
        px = x * cosA - y * sinA;
        py = x * sinA + y * cosA;
        x = px, y = py;
        return vec2(x, y);
    }
    std::string toMap() { return "vec2(" + std::to_string(x) + ", " + std::to_string(y) + ")"; }
};