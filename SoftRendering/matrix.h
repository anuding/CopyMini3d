#pragma once
#include <math.h>
#include <algorithm>
struct matrix { float m[4][4]; };

struct Vector3 {
	float x, y, z;
	Vector3() {
		x = y = z = 0;
	}
	//Vector3(Vector4 v);
	Vector3(float _x, float _y, float _z) {
		x = _x; y = _y; z = _z;
	}
	void SetVector(float _x, float _y, float _z) {
		x = _x; y = _y; z = _z;
	}
};

struct Vector4 {
	float x, y, z, w;
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

float Clamp(float x, float minval, float maxval);
Vector3 operator/(float k, const Vector3& v);
Vector3 operator*(float k, const Vector3& v);
Vector3 operator*(const Vector3& v, float k);
Vector3 operator+(float k, const Vector3& v);
Vector3 operator+(const Vector3& v, float k);
Vector3 operator-(float k, const Vector3& v);
Vector3 operator-(const Vector3& v, float k);
Vector3 operator-(const Vector3& v1, const Vector3& v2);
Vector3 operator+(const Vector3& v1, const Vector3& v2);
Vector3 Normalize(const Vector3& v1);
Vector3 Modulate(const Vector3& v1, const Vector3& v2);
float Dot(const Vector3& v1, const Vector3& v2);
float Length(const Vector3& v);

Vector4 operator*(const Vector4& v, float k);
Vector4 operator*(float k, const Vector4& v);
Vector4 operator-(const Vector4& v1, const Vector4& v2);
Vector4 operator+(const Vector4& v1, const Vector4& v2);
Vector4 Normalize(const Vector4& v1);
Vector4 Mul(const Vector4& v, const matrix& m);
Vector4 Cross(const Vector4& v1, const Vector4& v2);
float Dot(const Vector4& v1, const Vector4& v2);




matrix Mul(matrix& a, matrix& b);
matrix IdentityMat();
matrix TransMat(float x, float y, float z);
matrix RotaXMat(float theta);
matrix RotaYMat(float theta);
matrix RotaZMat(float theta);
matrix ScalMat(float x, float y, float z);
matrix LooAtMat(Vector4 viewpoint, Vector4 target, Vector4 up);
matrix ProjMat(float fovY, float aspect, float nearZ, float farZ);