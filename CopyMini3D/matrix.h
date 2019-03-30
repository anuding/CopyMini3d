#pragma once
#include <math.h>

struct matrix { float m[4][4]; };

struct Vector3 {
	float x, y, z;
	Vector3 operator*(float k);
	Vector3() {
		x = y = z = 0;
	}
	Vector3(float _x, float _y, float _z) {
		x = _x; y = _y; z = _z;
	}
	void SetVector(float _x, float _y, float _z) {
		x = _x; y = _y; z = _z;
	}
};

struct Vector4 {
	float x, y, z, w;
	Vector4 operator-(const Vector4& v1)const;
	Vector4() {
		x = y = z = 0; w = 1;
	}
	Vector4(float _x, float _y, float _z,float _w) {
		x = _x; y = _y; z = _z; w = _w;
	}
	Vector4(Vector3 v,float _w)
	{
		x = v.x; y = v.y; z = v.z; w = _w;
	}
	void SetVector(float _x, float _y, float _z, float _w) {
		x = _x; y = _y; z = _z; w = _w;
	}
};

typedef Vector4 Position;
typedef Vector3 color_t;
typedef Vector3 texcoord_t;
typedef Vector3 normal_t;
struct Vertex { Position pos; texcoord_t tc; color_t color; normal_t normal; float rhw; };

typedef struct _oper {
	int a;
	int b;
} oper;

Vector3 Modulate(const Vector3& v1, const Vector3& v2);
Vertex Interpolate(Vertex a, Vertex b, float grad);
float Dot(const Vector3& v1, const Vector3& v2);
float Dot(const Vector4& v1, const Vector4& v2);
Vector4 Cross(const Vector4& v1, const Vector4& v2);
Vector3 Normalize(const Vector3& v1);
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