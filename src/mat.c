#include "mat.h"
#include <stdio.h>

mat4 mat4_mul(mat4 a, mat4 b){
    mat4 bt = mat4_transpose(b);
    mat4 res = {{{0.0f},{0.0f},{0.0f},{0.0f}}};
    for(int i = 0; i < 4; i++)
        for(int j = 0; j < 4; j++)
            for(int k = 0; k < 4; k++){
                res.m[i][j] += a.m[i][k] * bt.m[j][k];
            }
    return res;
}

mat4 mat4_mul3 (mat4 a,  mat4 b, mat4 c) {
    return mat4_mul(a, mat4_mul(b, c));
}

mat4 mat4_transpose(mat4 a){
    mat4 res = {{
        {a.m[0][0],a.m[1][0],a.m[2][0],a.m[3][0]},
        {a.m[0][1],a.m[1][1],a.m[2][1],a.m[3][1]},
        {a.m[0][2],a.m[1][2],a.m[2][2],a.m[3][2]},
        {a.m[0][3],a.m[1][3],a.m[2][3],a.m[3][3]}
    }};
    return res;
}


mat4 mat4_identity(void){
    mat4 res ={{
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {0.0f, 0.0f, 0.0f, 1.0f}
    }};
    return res;
}


void mat4_print(mat4 a){
    printf("[%2.2f,%2.2f,%2.2f,%2.2f]\n[%2.2f,%2.2f,%2.2f,%2.2f]\n[%2.2f,%2.2f,%2.2f,%2.2f]\n[%2.2f,%2.2f,%2.2f,%2.2f]\n",
    a.m[0][0], a.m[0][1], a.m[0][2], a.m[0][3],
    a.m[1][0], a.m[1][1], a.m[1][2], a.m[1][3],
    a.m[2][0], a.m[2][1], a.m[2][2], a.m[2][3],
    a.m[3][0], a.m[3][1], a.m[3][2], a.m[3][3]);
}