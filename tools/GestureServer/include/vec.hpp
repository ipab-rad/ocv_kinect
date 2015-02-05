#ifndef _VEC3_H
#define _VEC3_H

#include <XnCppWrapper.h>


struct vec3 {

    double x;
    double y;
    double z;

    vec3& set(double x0, double y0, double z0);
    vec3& set(XnVector3D v);

    vec3& operator+=(const vec3& other);
    vec3& operator-=(const vec3& other);
    vec3& operator*=(const vec3& other);
    vec3& operator/=(const vec3& other);
    vec3& operator*=(double c);
    vec3& operator/=(double c);

    friend vec3 operator+(vec3 first, const vec3& second);
    friend vec3 operator-(vec3 first, const vec3& second);
    friend vec3 operator*(vec3 first, const vec3& second);
    friend vec3 operator/(vec3 first, const vec3& second);
    friend vec3 operator*(vec3 first, double c);
    friend vec3 operator/(vec3 first, double c);

    double dot(const vec3& other);
    double mag();
    double magSq();

};

#endif