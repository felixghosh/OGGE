#ifndef VEC_H
#define VEC_H

//--------VEC2--------
typedef struct vec2 {
    float x;
    float y;
} vec2;

vec2  vec2_add       (vec2 a,  vec2 b);
vec2  vec2_sub       (vec2 a,  vec2 b);
vec2  vec2_add_scalar(vec2 a, float b);
vec2  vec2_sub_scalar(vec2 a, float b);
vec2  vec2_mul_scalar(vec2 a, float b);
vec2  vec2_div_scalar(vec2 a, float b);
vec2  vec2_normalize (vec2 a);
float vec2_dot       (vec2 a, vec2 b);
float vec2_length    (vec2 a);


//--------VEC3--------
typedef struct vec3 {
    float x;
    float y;
    float z;
} vec3;

vec3  vec3_add       (vec3 a,  vec3 b);
vec3  vec3_sub       (vec3 a,  vec3 b);
vec3  vec3_add_scalar(vec3 a, float b);
vec3  vec3_sub_scalar(vec3 a, float b);
vec3  vec3_mul_scalar(vec3 a, float b);
vec3  vec3_div_scalar(vec3 a, float b);
vec3  vec3_cross     (vec3 a,  vec3 b);
vec3  vec3_normalize (vec3 a);
float vec3_dot       (vec3 a,  vec3 b);
float vec3_length    (vec3 a);

//--------VEC4--------
typedef struct vec4 {
    float x;
    float y;
    float z;
    float w;
} vec4;

vec4  vec4_add       (vec4 a,  vec4 b);
vec4  vec4_sub       (vec4 a,  vec4 b);
vec4  vec4_add_scalar(vec4 a, float b);
vec4  vec4_sub_scalar(vec4 a, float b);
vec4  vec4_mul_scalar(vec4 a, float b);
vec4  vec4_div_scalar(vec4 a, float b);
vec4  vec4_normalize (vec4 a);
float vec4_dot       (vec4 a,  vec4 b);
float vec4_length    (vec4 a);

#endif