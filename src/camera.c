#include "camera.h"

mat4 camera_ortho(float left, float right, float bottom, float top, float near, float far){
    mat4 N = {{
        {2/(right-left), 0.0f, 0.0f, -(right+left)/(right-left)},
        {0.0f, 2/(top-bottom), 0.0f, -(top+bottom)/(top-bottom)},
        {0.0f, 0.0f, -2/(far-near), -(far+near)/(far-near)},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};
    return N;
}