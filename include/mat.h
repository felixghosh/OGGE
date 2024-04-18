#ifndef MAT_H
#define MAT_H
#include "vec.h"

//All matrices are row-major, meaning they are index as m[row][col]

//--------MAT2--------
typedef struct mat2{
    float m[2][2];
} mat2;

mat2 mat2_add       (mat2 a,  mat2 b);
mat2 mat2_sub       (mat2 a,  mat2 b);
mat2 mat2_mul       (mat2 a,  mat2 b);
mat2 mat2_add_scalar(mat2 a, float b);
mat2 mat2_sub_scalar(mat2 a, float b);
mat2 mat2_mul_scalar(mat2 a, float b);
mat2 mat2_div_scalar(mat2 a, float b);
vec2 mat2_mul_vec   (mat2 a,  vec2 b);


//--------MAT2x3--------
typedef struct mat2x3{
    float m[3][2];
} mat2x3;

mat2x3 mat2x3_add       (mat2x3 a, mat2x3 b);
mat2x3 mat2x3_sub       (mat2x3 a, mat2x3 b);
mat2x3 mat2x3_mul       (mat2x3 a, mat2x3 b);
mat2x3 mat2x3_add_scalar(mat2x3 a,  float b);
mat2x3 mat2x3_sub_scalar(mat2x3 a,  float b);
mat2x3 mat2x3_mul_scalar(mat2x3 a,  float b);
mat2x3 mat2x3_div_scalar(mat2x3 a,  float b);
vec3   mat2x3_mul_vec   (mat2x3 a,   vec3 b);


//--------MAT2x4--------
typedef struct mat2x4{
    float m[4][2];
} mat2x4;

mat2x4 mat2x4_add       (mat2x4 a, mat2x4 b);
mat2x4 mat2x4_sub       (mat2x4 a, mat2x4 b);
mat2x4 mat2x4_mul       (mat2x4 a, mat2x4 b);
mat2x4 mat2x4_add_scalar(mat2x4 a,  float b);
mat2x4 mat2x4_sub_scalar(mat2x4 a,  float b);
mat2x4 mat2x4_mul_scalar(mat2x4 a,  float b);
mat2x4 mat2x4_div_scalar(mat2x4 a,  float b);
vec4   mat2x4_mul_vec   (mat2x4 a,   vec4 b);


//--------MAT3--------
typedef struct mat3{
    float m[3][3];
} mat3;

mat3 mat3_add       (mat3 a,  mat3 b);
mat3 mat3_sub       (mat3 a,  mat3 b);
mat3 mat3_mul       (mat3 a,  mat3 b);
mat3 mat3_add_scalar(mat3 a, float b);
mat3 mat3_sub_scalar(mat3 a, float b);
mat3 mat3_mul_scalar(mat3 a, float b);
mat3 mat3_div_scalar(mat3 a, float b);
vec3 mat3_mul_vec   (mat3 a,  vec3 b);


//--------MAT3x2--------
typedef struct mat3c2{
    float m[2][3];
} mat3x2;

mat3x2 mat3x2_add       (mat3x2 a, mat3x2 b);
mat3x2 mat3x2_sub       (mat3x2 a, mat3x2 b);
mat3x2 mat3x2_mul       (mat3x2 a, mat3x2 b);
mat3x2 mat3x2_add_scalar(mat3x2 a,  float b);
mat3x2 mat3x2_sub_scalar(mat3x2 a,  float b);
mat3x2 mat3x2_mul_scalar(mat3x2 a,  float b);
mat3x2 mat3x2_div_scalar(mat3x2 a,  float b);
vec2   mat3x2_mul_vec   (mat3x2 a,   vec2 b);


//--------MAT3x4--------
typedef struct mat3x4{
    float m[4][3];
} mat3x4;

mat3x4 mat3x4_add       (mat3x4 a, mat3x4 b);
mat3x4 mat3x4_sub       (mat3x4 a, mat3x4 b);
mat3x4 mat3x4_mul       (mat3x4 a, mat3x4 b);
mat3x4 mat3x4_add_scalar(mat3x4 a,  float b);
mat3x4 mat3x4_sub_scalar(mat3x4 a,  float b);
mat3x4 mat3x4_mul_scalar(mat3x4 a,  float b);
mat3x4 mat3x4_div_scalar(mat3x4 a,  float b);
vec4   mat3x4_mul_vec   (mat3x4 a,   vec4 b);


//--------MAT4--------
typedef struct mat4{
    float m[4][4];
} mat4;

mat4 mat4_add       (mat4 a,  mat4 b);
mat4 mat4_sub       (mat4 a,  mat4 b);
mat4 mat4_mul       (mat4 a,  mat4 b);
mat4 mat4_add_scalar(mat4 a, float b);
mat4 mat4_sub_scalar(mat4 a, float b);
mat4 mat4_mul_scalar(mat4 a, float b);
mat4 mat4_div_scalar(mat4 a, float b);
mat4 mat4_transpose (mat4 a);
mat4 mat4_identity  (void);
vec4 mat4_mul_vec   (mat4 a,  vec4 b);
void mat4_print     (mat4 a);


//--------MAT4x2--------
typedef struct mat4x2{
    float m[2][4];
} mat4x2;

mat4x2 mat4x2_add       (mat4x2 a, mat4x2 b);
mat4x2 mat4x2_sub       (mat4x2 a, mat4x2 b);
mat4x2 mat4x2_mul       (mat4x2 a, mat4x2 b);
mat4x2 mat4x2_add_scalar(mat4x2 a,  float b);
mat4x2 mat4x2_sub_scalar(mat4x2 a,  float b);
mat4x2 mat4x2_mul_scalar(mat4x2 a,  float b);
mat4x2 mat4x2_div_scalar(mat4x2 a,  float b);
vec2   mat4x2_mul_vec   (mat4x2 a,   vec2 b);


//--------MAT4x3--------
typedef struct mat4x3{
    float m[3][4];
} mat4x3;

mat4x3 mat4x3_add       (mat4x3 a, mat4x3 b);
mat4x3 mat4x3_sub       (mat4x3 a, mat4x3 b);
mat4x3 mat4x3_mul       (mat4x3 a, mat4x3 b);
mat4x3 mat4x3_add_scalar(mat4x3 a,  float b);
mat4x3 mat4x3_sub_scalar(mat4x3 a,  float b);
mat4x3 mat4x3_mul_scalar(mat4x3 a,  float b);
mat4x3 mat4x3_div_scalar(mat4x3 a,  float b);
vec3   mat4x3_mul_vec   (mat4x3 a,   vec3 b);


#endif