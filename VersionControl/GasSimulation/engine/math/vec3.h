#pragma once
#include "vec2.h"
#include <SFML/Graphics.hpp>
#define vec3f vec3<float>
#define vec3i vec3<int>


template <typename T>
class vec3 {
public:
    T x, y, z;
    explicit vec3(float _value) : x(_value), y(_value), z(_value) {};
    vec3(float _x, vec2<T> const& v) : x(_x), y(v.x), z(v.y) {};
    vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {};
    vec3() : x(0), y(0), z(0) {}

    vec3 operator+(vec3 const& other) { return vec3(x + other.x, y + other.y, z + other.z); }
    vec3 operator-(vec3 const& other) { return vec3(x - other.x, y - other.y, z - other.z); }
    vec3 operator*(vec3 const& other) { return vec3(x * other.x, y * other.y, z * other.z); }
    vec3 operator/(vec3 const& other) { return vec3(x / other.x, y / other.y, z / other.z); }
    vec3 operator-() { return vec3(-x, -y, -z); }
    vec3 operator*(float num) { return vec3(x * num, y * num, z * num); }

    bool operator==(vec3 other) const { return x == other.x and y == other.y and z == other.z; }

    vec2<T> xy() { return vec2<T>(x, y); }
    vec2<T> yx() { return vec2<T>(y, x); }
    vec2<T> xz() { return vec2<T>(x, z); }
    vec2<T> zx() { return vec2<T>(z, x); }
    vec2<T> yz() { return vec2<T>(y, z); }
    vec2<T> zy() { return vec2<T>(z, y); }
    vec3 xyz() { return  vec3(x, y, z); }
    vec3 xzy() { return  vec3(x, z, y); }
    vec3 yxz() { return  vec3(y, x, z); }
    vec3 yzx() { return  vec3(y, z, x); }
    vec3 zxy() { return  vec3(z, x, y); }
    vec3 zyx() { return  vec3(z, y, x); }
    float SquaredLength() const { return x * x + y * y + z * z; }
    sf::Vector3f toSFML() const { return sf::Vector3f(x, y, z); }
    vec3 rotate(float angle1, float angle2) {
        vec2<T> tmp1 = vec2<T>(x, z).rotate(angle1);
        y = x * std::sin(angle2);
        x = tmp1.x * cos(angle2), z = tmp1.y;
        return vec3(x, y, z);
    }
    float dot(vec3 v) {
        return x * v.x + y * v.y + z * v.z;
    }
    vec3 reflect(vec3 normal) {
        return xyz() - normal * (2.0 * dot(normal));
    }

    std::string toMap() { return "vec3(" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")"; }
};

