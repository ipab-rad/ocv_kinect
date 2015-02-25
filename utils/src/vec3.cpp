#include <algorithm>
#include <cmath>
#include <vec3.hpp>


vec3 vec3::zero() {
    return {0, 0, 0};
}

vec3& vec3::set(double x0, double y0, double z0) {
    this->x = x0;
    this->y = y0;
    this->z = z0;
    return *this;
}

vec3& vec3::set(XnVector3D v) {
    this->x = v.X;
    this->y = v.Y;
    this->z = v.Z;
    return *this;
}

vec3& vec3::operator+=(const vec3& other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}

vec3& vec3::operator-=(const vec3& other) {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}

vec3& vec3::operator*=(const vec3& other) {
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;
    return *this;
}

vec3& vec3::operator/=(const vec3& other) {
    this->x /= other.x;
    this->y /= other.y;
    this->z /= other.z;
    return *this;
}

vec3& vec3::operator*=(double c) {
    this->x *= c;
    this->y *= c;
    this->z *= c;
    return *this;
}

vec3& vec3::operator/=(double c) {
    this->x /= c;
    this->y /= c;
    this->z /= c;
    return *this;
}

double vec3::dot(const vec3& other) {
    vec3 mulled = (*this) * other;
    return mulled.x + mulled.y + mulled.z;
}

double vec3::magSq() {
    return (this->dot(*this));
}

double vec3::mag() {
    return sqrt(this->magSq());
}

vec3 vec3::normalized() {
    return (*this) / (this->mag());
}

vec3& vec3::squash(double minimum) {
    this->x = (-minimum < this->x && this->x < minimum) ? 0 : this->x;
    this->y = (-minimum < this->y && this->y < minimum) ? 0 : this->y;
    this->z = (-minimum < this->z && this->z < minimum) ? 0 : this->z;
    return *this;
}

double sign(double val) {
    return (val >= 0.0) ? 1.0 : -1.0; 
}

vec3& vec3::shrink(double amount) {
    double sx = sign(this->x);
    double sy = sign(this->y);
    double sz = sign(this->z);
    this->x = std::max(0.0, std::abs(this->x) - amount) * sx;
    this->y = std::max(0.0, std::abs(this->y) - amount) * sy;
    this->z = std::max(0.0, std::abs(this->z) - amount) * sz;
    return *this;
}

vec3& vec3::clampcomponents(double minimum, double maximum) {
    this->x = std::min(std::max(minimum, this->x), maximum);
    this->y = std::min(std::max(minimum, this->y), maximum);
    this->z = std::min(std::max(minimum, this->z), maximum);
    return *this;
}