#pragma once
#include <math.h>

struct matrix { float m[4][4]; };

struct Vector4 {
	float x, y, z, w;
	void SetVector(float _x, float _y, float _z, float _w){
		x = _x; y = _y; z = _z; w = _w;
	}
	
	Vector4 operator-(const Vector4& v1)
	{
		Vector4 ans;
		ans.SetVector(x - v1.x, y - v1.y, z - v1.z, 0);
		return ans;
	}
};

typedef Vector4 Point;
typedef struct { float r, g, b; } color_t;
typedef struct { float u, v; } texcoord_t;
struct Vertex { Point pos; texcoord_t tc; color_t color; float rhw; } ;

typedef struct _oper {
	int a;
	int b;
} oper;

Vertex Interpolate(Vertex a, Vertex b, float grad);
float Dot(const Vector4& v1, const Vector4& v2);
Vector4 Cross(const Vector4& v1, const Vector4& v2);
Vector4 Normalize(const Vector4& v1);
Vector4 Mul(const Vector4& v, const matrix& m);
matrix Mul(matrix& a, matrix& b);

matrix IdentityMat();
matrix TransMat(float x, float y, float z);
matrix RotaXMat(float theta);
matrix RotaYMat(float theta);
matrix RotaZMat(float theta);

matrix ScalMat(float x, float y, float z);
matrix LooAtMat(Vector4 viewpoint, Vector4 target, Vector4 up);
matrix ProjMat(float fovY, float aspect, float nearZ, float farZ);
void ScreenMapping(float x, float y, float z);