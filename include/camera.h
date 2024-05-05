#ifndef CAMERA_H
#define CAMERA_H

#include "mat.h"

typedef struct camera_t {
    float x;
    float y;
    float z;
    float theta_x;
    float theta_y;
} camera;

mat4 camera_ortho(float left, float right, float bottom, float top, float near, float far);

mat4 camera_frustum(float left, float right, float bottom, float top, float near, float far);

mat4 camera_perspective(float fovy, float aspect, float near, float far);

#endif