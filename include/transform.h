#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "mat.h"

mat4 transform_rotate_x(float theta);
mat4 transform_rotate_y(float theta);
mat4 transform_rotate_z(float theta);
mat4 transform_translate(float dx, float dy, float dz);
mat4 transform_scale(float sx, float sy, float sz);



#endif