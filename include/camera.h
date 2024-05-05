#ifndef CAMERA_H
#define CAMERA_H

#include "mat.h"

mat4 camera_ortho(float left, float right, float bottom, float top, float near, float far);

#endif