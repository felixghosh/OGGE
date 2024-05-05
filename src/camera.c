#include "camera.h"
#include <math.h>

mat4 camera_ortho(float left, float right, float bottom, float top, float near, float far){
    mat4 N = {{
        {2/(right-left), 0.0f, 0.0f, -(right+left)/(right-left)},
        {0.0f, 2/(top-bottom), 0.0f, -(top+bottom)/(top-bottom)},
        {0.0f, 0.0f, -2/(far-near), -(far+near)/(far-near)},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};
    return N;
}

mat4 camera_frustum(float left, float right, float bottom, float top, float near, float far){
    mat4 N = {{
        {(2*near)/(right-left), 0.0f, (right+left)/(right-left), 0.0f},
        {0.0f, (2*near)/(top-bottom), (top+bottom)/(top-bottom), 0.0f},
        {0.0f, 0.0f, -(far+near)/(far-near), -(2*far*near)/(far-near)},
        {0.0f, 0.0f, -1.0f, 0.0f}
    }};
    return N;
}

mat4 camera_perspective(float fovy, float aspect, float near, float far){
    float top = near * tan(fovy);
    float bottom = -top;
    float right = top * aspect;
    float left = -right;
    mat4 N = {{
        {near/right, 0.0f, 0.0f, 0.0f},
        {0.0f, near/top, 0.0f, 0.0f},
        {0.0f, 0.0f, -(far+near)/(far-near), -(2*far*near)/(far-near)},
        {0.0f, 0.0f, -1.0f, 0.0f}
    }};
    return N;
}