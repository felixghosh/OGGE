#include "transform.h"
#include <math.h>

mat4 transform_rotate_x(float theta){
    float rad = M_PI/180.0*theta;
    float cr = cos(rad);
    float sr = sin(rad);
    mat4 res = {{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, cr,  -sr,   0.0f},
        {0.0f, sr,   cr,   0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};
    return res;
}

mat4 transform_rotate_y(float theta){
    float rad = M_PI/180.0*theta;
    float cr = cos(rad);
    float sr = sin(rad);
    mat4 res = {{
        {cr,   0.0f, sr,   0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {-sr,  0.0f, cr,   0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};
    return res;
}

mat4 transform_rotate_z(float theta){
    float rad = M_PI/180.0*theta;
    float cr = cos(rad);
    float sr = sin(rad);
    mat4 res = {{
        {cr,  -sr,   0.0f, 0.0f},
        {sr,   cr,   0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};
    return res;
}

mat4 transform_translate(float dx, float dy, float dz){
    mat4 res = {{
        {1.0f, 0.0f, 0.0f, dx},
        {0.0f, 1.0f, 0.0f, dy},
        {0.0f, 0.0f, 1.0f, dz},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};
    return res;
}

mat4 transform_scale(float sx, float sy, float sz){
    mat4 res = {{
        {sx,   0.0f, 0.0f, 0.0f},
        {0.0f, sy,   0.0f, 0.0f},
        {0.0f, 0.0f, sz,   0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};
    return res;
}