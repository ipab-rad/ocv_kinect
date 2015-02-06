#include <math.h>
#include <vec.hpp>


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
    this->x += other->x;
    this->y += other->y;
    this->z += other->z;
    return *this;
}

vec3& vec3::operator-=(const vec3& other) {
    this->x -= other->x;
    this->y -= other->y;
    this->z -= other->z;
    return *this;
}

vec3& vec3::operator*=(const vec3& other) {
    this->x *= other->x;
    this->y *= other->y;
    this->z *= other->z;
    return *this;
}

vec3& vec3::operator/=(const vec3& other) {
    this->x /= other->x;
    this->y /= other->y;
    this->z /= other->z;
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

friend vec3 vec3::operator+(vec3 first, const vec3& second) {
    return (first += second);
}

friend vec3 vec3::operator-(vec3 first, const vec3& second) {
    return (first -= second);
}

friend vec3 vec3::operator*(vec3 first, const vec3& second) {
    return (first *= second);
}

friend vec3 vec3::operator/(vec3 first, const vec3& second) {
    return (first /= second);
}

friend vec3 vec3::operator*(vec3 first, double c) {
    return (first *= c);
}

friend vec3 vec3::operator/(vec3 first, double c) {
    return (first /= c);
}

double vec3::dot(const vec3& other) {
    vec3 mulled = (this * other);
    return mulled.x + mulled.y + mulled.z;
}

double vec3::magSq() {
    return (this->dot(this));
}

double vec3::mag() {
    return sqrt(this->magSq());
}

vec3 vec3::normalized() {
    return this / this->mag();
}