#include "vec.h"
#include <math.h>

//--------VEC2--------
vec2 vec2_add(vec2 a, vec2 b){
    return (vec2){a.x+b.x, a.y+b.y};
}

vec2 vec2_sub(vec2 a,  vec2 b){
    return (vec2){a.x-b.x, a.y-b.y};
}

vec2 vec2_add_scalar(vec2 a, float b){
    return (vec2){a.x+b, a.y+b};
}

vec2 vec2_sub_scalar(vec2 a, float b){
    return (vec2){a.x-b, a.y-b};
}

vec2 vec2_mul_scalar(vec2 a, float b){
    return (vec2){a.x*b, a.y*b};
}

vec2 vec2_div_scalar(vec2 a, float b){
    float divisor = 1.0f / b;
    return (vec2){a.x*divisor, a.y*divisor};
}

vec2 vec2_normalize(vec2 a){
    float divisor = 1.0f / vec2_length(a);
    return (vec2){a.x*divisor, a.y*divisor};
}

float vec2_dot(vec2 a, vec2 b){
    return (a.x*b.x) + (a.y*b.y);
}

float vec2_length(vec2 a){
    return sqrt((a.x*a.x) + (a.y*a.y));
}


//--------VEC3--------
vec3 vec3_add(vec3 a, vec3 b){
    return (vec3){a.x+b.x, a.y+b.y, a.z+b.z};
}

vec3 vec3_sub(vec3 a,  vec3 b){
    return (vec3){a.x-b.x, a.y-b.y, a.z-b.z};
}

vec3 vec3_add_scalar(vec3 a, float b){
    return (vec3){a.x+b, a.y+b, a.z+b};
}

vec3 vec3_sub_scalar(vec3 a, float b){
    return (vec3){a.x-b, a.y-b, a.z-b};
}

vec3 vec3_mul_scalar(vec3 a, float b){
    return (vec3){a.x*b, a.y*b, a.z*b};
}

vec3 vec3_div_scalar(vec3 a, float b){
    float divisor = 1.0f / b;
    return (vec3){a.x*divisor, a.y*divisor, a.z*divisor};
}

vec3 vec3_cross(vec3 a,  vec3 b){
    return (vec3)
    {a.y*b.z - a.z*b.y,
     a.z*b.x - a.x*b.z, 
     a.z*b.y - a.y*b.x};
}

vec3 vec3_normalize(vec3 a){
    float divisor = 1.0f / vec3_length(a);
    return (vec3){a.x*divisor, a.y*divisor, a.z*divisor};
}

float vec3_dot(vec3 a, vec3 b){
    return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
}

float vec3_length(vec3 a){
    return sqrt((a.x*a.x) + (a.y*a.y) + (a.z*a.z));
}


//--------VEC4--------
vec4 vec4_add(vec4 a, vec4 b){
    return (vec4){a.x+b.x, a.y+b.y, a.z+b.z, a.w+b.w};
}

vec4 vec4_sub(vec4 a,  vec4 b){
    return (vec4){a.x-b.x, a.y-b.y, a.z-b.z, a.w-b.w};
}

vec4 vec4_add_scalar(vec4 a, float b){
    return (vec4){a.x+b, a.y+b, a.z+b, a.w+b};
}

vec4 vec4_sub_scalar(vec4 a, float b){
    return (vec4){a.x-b, a.y-b, a.z-b, a.w-b};
}

vec4 vec4_mul_scalar(vec4 a, float b){
    return (vec4){a.x*b, a.y*b, a.z*b, a.w*b};
}

vec4 vec4_div_scalar(vec4 a, float b){
    float divisor = 1.0f / b;
    return (vec4){a.x*divisor, a.y*divisor, a.z*divisor, a.w*divisor};
}

vec4 vec4_normalize(vec4 a){
    float divisor = 1.0f / vec4_length(a);
    return (vec4){a.x*divisor, a.y*divisor, a.z*divisor, a.w*divisor};
}

float vec4_dot(vec4 a, vec4 b){
    return (a.x*b.x) + (a.y*b.y) + (a.z*b.z) + (a.w*b.w);
}

float vec4_length(vec4 a){
    return sqrt((a.x*a.x) + (a.y*a.y) + (a.z*a.z) + (a.w*a.w));
}