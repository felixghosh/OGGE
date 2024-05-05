#ifndef CAMERA_H
#define CAMERA_H

#include "mat.h"

mat4 camera_ortho(float left, float right, float bottom, float top, float near, float far);

mat4 camera_frustum(float left, float right, float bottom, float top, float near, float far);

mat4 camera_perspective(float fovy, float aspect, float near, float far);

#endif