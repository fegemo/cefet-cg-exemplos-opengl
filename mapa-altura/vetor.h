#ifndef __VETOR_H__
#define __VETOR_H__

typedef struct Vec3 {
    union {
        float v[3];
        struct {
            float x, y, z;
        };
        struct {
            float r, g, b;
        };
    };
} vec3;


float length(vec3*);
vec3 normalize(vec3*);
vec3 add(vec3*, vec3*);
vec3 mult(vec3*, float);
vec3 sub(vec3*, vec3*);
vec3 cross(vec3*, vec3*);
void toString(vec3*);


typedef struct Vec4 {
  union {
    float v[4];
    struct {
      float x, y, z, w;
    };
    struct {
      float r, g, b, a;
    };
  };
} vec4;

#endif
