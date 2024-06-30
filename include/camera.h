#ifndef CAMERA_H
#define CAMERA_H

#include "mat.h"
#include "object.h"

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

void camera_pitch(camera *c, float deg);

void camera_yaw(camera *c, float deg);

mat4 camera_view_mat(camera *c, object o);

float deg_to_rad(float deg);

#endif