#include "vec.h"
#include <stdio.h>
#include <math.h>

//--------VEC2--------
vec2 vec2_add(vec2 a, vec2 b){
    return (vec2){{a.v[0]+b.v[0], a.v[1]+b.v[1]}};
}

vec2 vec2_sub(vec2 a,  vec2 b){
    return (vec2){{a.v[0]-b.v[0], a.v[1]-b.v[1]}};
}

vec2 vec2_add_scalar(vec2 a, float b){
    return (vec2){{a.v[0]+b, a.v[1]+b}};
}

vec2 vec2_sub_scalar(vec2 a, float b){
    return (vec2){{a.v[0]-b, a.v[1]-b}};
}

vec2 vec2_mul_scalar(vec2 a, float b){
    return (vec2){{a.v[0]*b, a.v[1]*b}};
}

vec2 vec2_div_scalar(vec2 a, float b){
    float divisor = 1.0f / b;
    return (vec2){{a.v[0]*divisor, a.v[1]*divisor}};
}

vec2 vec2_normalize(vec2 a){
    float divisor = 1.0f / vec2_length(a);
    return (vec2){{a.v[0]*divisor, a.v[1]*divisor}};
}

float vec2_dot(vec2 a, vec2 b){
    return (a.v[0]*b.v[0]) + (a.v[1]*b.v[1]);
}

float vec2_length(vec2 a){
    return sqrt((a.v[0]*a.v[0]) + (a.v[1]*a.v[1]));
}


//--------VEC3--------
vec3 vec3_add(vec3 a, vec3 b){
    return (vec3){{a.v[0]+b.v[0], a.v[1]+b.v[1], a.v[2]+b.v[2]}};
}

vec3 vec3_sub(vec3 a,  vec3 b){
    return (vec3){{a.v[0]-b.v[0], a.v[1]-b.v[1], a.v[2]-b.v[2]}};
}

vec3 vec3_add_scalar(vec3 a, float b){
    return (vec3){{a.v[0]+b, a.v[1]+b, a.v[2]+b}};
}

vec3 vec3_sub_scalar(vec3 a, float b){
    return (vec3){{a.v[0]-b, a.v[1]-b, a.v[2]-b}};
}

vec3 vec3_mul_scalar(vec3 a, float b){
    return (vec3){{a.v[0]*b, a.v[1]*b, a.v[2]*b}};
}

vec3 vec3_div_scalar(vec3 a, float b){
    float divisor = 1.0f / b;
    return (vec3){{a.v[0]*divisor, a.v[1]*divisor, a.v[2]*divisor}};
}

vec3 vec3_cross(vec3 a,  vec3 b){
    return (vec3){
    {a.v[1]*b.v[2] - a.v[2]*b.v[1],
     a.v[2]*b.v[0] - a.v[0]*b.v[2],
     a.v[2]*b.v[1] - a.v[1]*b.v[0]}};
}

vec3 vec3_normalize(vec3 a){
    float divisor = 1.0f / vec3_length(a);
    return (vec3){{a.v[0]*divisor, a.v[1]*divisor, a.v[2]*divisor}};
}

float vec3_dot(vec3 a, vec3 b){
    return (a.v[0]*b.v[0]) + (a.v[1]*b.v[1]) + (a.v[2]*b.v[2]);
}

float vec3_length(vec3 a){
    return sqrt((a.v[0]*a.v[0]) + (a.v[1]*a.v[1]) + (a.v[2]*a.v[2]));
}

void  vec3_print(vec3 a){
    printf("(%2.2f, %2.2f, %2.2f)\n", a.v[0], a.v[1], a.v[2]);
}


// //--------VEC4--------
vec4 vec4_add(vec4 a, vec4 b){
    return (vec4){{a.v[0]+b.v[0], a.v[1]+b.v[1], a.v[2]+b.v[2], a.v[3]+b.v[3]}};
}

vec4 vec4_sub(vec4 a,  vec4 b){
    return (vec4){{a.v[0]-b.v[0], a.v[1]-b.v[1], a.v[2]-b.v[2], a.v[3]-b.v[3]}};
}

vec4 vec4_add_scalar(vec4 a, float b){
    return (vec4){{a.v[0]+b, a.v[1]+b, a.v[2]+b, a.v[3]+b}};
}

vec4 vec4_sub_scalar(vec4 a, float b){
    return (vec4){{a.v[0]-b, a.v[1]-b, a.v[2]-b, a.v[3]-b}};
}

vec4 vec4_mul_scalar(vec4 a, float b){
    return (vec4){{a.v[0]*b, a.v[1]*b, a.v[2]*b, a.v[3]*b}};
}

vec4 vec4_div_scalar(vec4 a, float b){
    float divisor = 1.0f / b;
    return (vec4){{a.v[0]*divisor, a.v[1]*divisor, a.v[2]*divisor, a.v[3]*divisor}};
}

vec4 vec4_normalize(vec4 a){
    float divisor = 1.0f / vec4_length(a);
    return (vec4){{a.v[0]*divisor, a.v[1]*divisor, a.v[2]*divisor, a.v[3]*divisor}};
}

float vec4_dot(vec4 a, vec4 b){
    return (a.v[0]*b.v[0]) + (a.v[1]*b.v[1]) + (a.v[2]*b.v[2]) + (a.v[3]*b.v[3]);
}

float vec4_length(vec4 a){
    return sqrt((a.v[0]*a.v[0]) + (a.v[1]*a.v[1]) + (a.v[2]*a.v[2]) + (a.v[3]*a.v[3]));
}