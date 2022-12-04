#include <cstdint>
#include <algorithm>
#include <iostream>
#include <iomanip>
#include <random>
#include "Vector3d.h"

vec3_t::vec3_t(void) {
    x = y = z = 0.0f;
   
}

vec3_t::vec3_t(float _x, float _y, float _z) {
    x = _x;
    y = _y;
    z = _z;
}

vec3_t::~vec3_t(void) {};

void vec3_t::init(float _x, float _y, float _z) {
    x = _x; y = _y; z = _z;
}

void vec3_t::clamp(void) {
    x = std::clamp(x, -89.0f, 89.0f);
    y = std::clamp(std::remainder(y, 360.0f), -180.0f, 180.0f);
    z = std::clamp(z, -50.0f, 50.0f);
}

vec3_t vec3_t::clamped() {
    vec3_t clamped = *this;
    clamped.clamp();
    return clamped;
}
vec3_t vec3_t::calculate_angle(vec3_t& in, vec3_t& out) {
    vec3_t delta = in - out;
    double hyp = sqrt(delta.x * delta.x + delta.y * delta.y);

    vec3_t ret;
    ret.x = (float)(atanf(delta.z / hyp) * 57.295779513082f);
    ret.y = (float)(atanf(delta.y / delta.x) * 57.295779513082f);
    ret.z = 0.0f;

    if (delta.x >= 0.0) ret.y += 180.0f;

    return ret;
}
vec3_t vec3_t::calculate_relative_angle(vec3_t& src, vec3_t& dst, vec3_t& viewangles) {
    vec3_t angles = calculate_angle(src, dst);

    return (angles - viewangles).normalized();
}
void vec3_t2::transform_vector(vec3_t& a, matrix_t& b, vec3_t& out) {
    out.x = a.dot(b.mat_val[0]) + b.mat_val[0][3];
    out.y = a.dot(b.mat_val[1]) + b.mat_val[1][3];
    out.z = a.dot(b.mat_val[2]) + b.mat_val[2][3];
}
float vec3_t::distance_to(const vec3_t& other) {
    vec3_t delta;
    delta.x = x - other.x;
    delta.y = y - other.y;
    delta.z = z - other.z;

    return delta.length();
}


void vec3_t::normalize() {
    x = std::isfinite(x) ? std::remainderf(x, 360.0f) : 0.0f;
    y = std::isfinite(y) ? std::remainderf(y, 360.0f) : 0.0f;
    z = 0.0f;
}

vec3_t vec3_t::normalized(void) {
    vec3_t vec(*this);
    vec.normalize();

    return vec;
}

float vec3_t::length(void) {
    return std::sqrt(x * x + y * y + z * z);
}

float vec3_t::length_sqr(void) {
    auto sqr = [](float n) {
        return static_cast<float>(n * n);
    };

    return (sqr(x) + sqr(y) + sqr(z));
}

float vec3_t::length_2d_sqr(void) const {
    return (x * x + y * y);
}

float vec3_t::dot(const vec3_t other) {
    return (x * other.x + y * other.y + z * other.z);
}

float vec3_t::dot(const float* other) {
    const vec3_t& a = *this;

    return(a.x * other[0] + a.y * other[1] + a.z * other[2]);
}
