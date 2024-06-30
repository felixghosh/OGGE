#include <math.h>

#include "camera.h"
#include "transform.h"

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
    float bottom = near * tan(fovy);
    float top = -bottom;
    float right = top * aspect;
    //float left = -right;
    mat4 N = {{
        {near/right, 0.0f, 0.0f, 0.0f},
        {0.0f, near/top, 0.0f, 0.0f},
        {0.0f, 0.0f, -(far+near)/(far-near), -(2*far*near)/(far-near)},
        {0.0f, 0.0f, -1.0f, 0.0f}
    }};
    return N;
}

void camera_pitch(camera *c, float deg){
    if(c->theta_x + deg < -90.0f)
        c->theta_x = -90.0f;
    else if(c->theta_x + deg > 90.0f)
        c->theta_x = 90.0f;
    else
        c->theta_x += deg;
}

void camera_yaw(camera *c, float deg){
    c->theta_y += deg;
}

mat4 camera_view_mat(camera *c, object o){
    vec3 object_pos = o.pos;
    mat4 view = mat4_identity();

    //translation
    view = mat4_mul(view, transform_translate(-c->x, -c->y, -c->z));

    //rotation
    view = mat4_mul(view, transform_translate(c->x-object_pos.v[0], c->y-object_pos.v[1], c->z-object_pos.v[2]));
    view = mat4_mul(view, transform_rotate_x(-c->theta_x));
    view = mat4_mul(view, transform_rotate_y(-c->theta_y));
    view = mat4_mul(view, transform_translate(object_pos.v[0]-c->x, object_pos.v[1]-c->y, object_pos.v[2]-c->z));

    return view;
}

float deg_to_rad(float deg) {
    float rad = deg * M_PI / 180.0f;
    return rad;
}