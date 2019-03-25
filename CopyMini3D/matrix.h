#pragma once
#include <math.h>
typedef struct { float m[4][4]; } matrix;

struct Vector4 {
	float v[4] = { 0 };
	//Vector4(){
	//	v = { 0 };
	//};
	void SetVector(float x, float y, float z, float w){
		v[0] = x; v[1] = y; v[2] = z; v[3] = w;
	}
	Vector4 operator-(const Vector4& v1)
	{
		Vector4 ans;
		ans.SetVector(v[0] - v1.v[0], v[1] - v1.v[1], v[2] - v1.v[2], 0);
		return ans;
	}
	Vector4 operator/(const Vector4& v1)
	{
		Vector4 ans;
		ans.SetVector(v[0] / v1.v[0], v[1] / v1.v[1], v[2] / v1.v[2], v[3] / v1.v[3]);
		return ans;
	}
};
float Dot(const Vector4& v1, const Vector4& v2)
{
	float ans;
	float a1, b1, c1, a2, b2, c2;
	a1 = v1.v[0]; b1 = v1.v[1]; c1 = v1.v[2];
	a2 = v2.v[0]; b2 = v2.v[1]; c2 = v2.v[2];
	ans = a1 * a2 + b1 * b2 + c1 * c2;
	return ans;
}

Vector4 Cross(const Vector4& v1, const Vector4& v2)
{
	Vector4 ans;
	float a1, b1, c1, a2, b2, c2;
	a1 = v1.v[0]; b1 = v1.v[1]; c1 = v1.v[2];
	a2 = v2.v[0]; b2 = v2.v[1]; c2 = v2.v[2];
	ans.SetVector(b1*c2 - b2 * c1, c1*a2 - a1 * c2, a1*b2 - a2 * b1, 0);
	return ans;
}
Vector4 Normalize(const Vector4& v1)
{
	Vector4 ans;
	int k = sqrt(v1.v[0] * v1.v[0]+v1.v[1] * v1.v[1]+ v1.v[2] * v1.v[2]);
	ans.SetVector(v1.v[0] / k, v1.v[1] / k, v1.v[2] / k, 0);
	return ans;
}

Vector4 Mul(const Vector4& v, const matrix& m)
{
	Vector4 ans = { 0 };
	for (int i = 0; i < 4; i++)
	{	
		for (int j = 0; j < 4; j++)
		{
			ans.v[i] = ans.v[i]+v.v[j] * m.m[j][i];
		}
	}
	return ans; 
}
matrix Mul(matrix& a, matrix& b)
{
	matrix z;
	int i, j;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			z.m[j][i] = (a.m[j][0] * b.m[0][i]) +
				(a.m[j][1] * b.m[1][i]) +
				(a.m[j][2] * b.m[2][i]) +
				(a.m[j][3] * b.m[3][i]);
		}
	}
	return z;
}

matrix IdentityMat() {
	matrix m;
	m.m[0][0] = m.m[1][1] = m.m[2][2] = m.m[3][3] = 1.0f;
	m.m[0][1] = m.m[0][2] = m.m[0][3] = 0.0f;
	m.m[1][0] = m.m[1][2] = m.m[1][3] = 0.0f;
	m.m[2][0] = m.m[2][1] = m.m[2][3] = 0.0f;
	m.m[3][0] = m.m[3][1] = m.m[3][2] = 0.0f;
	return m;
}


matrix TransMat(float x, float y, float z) {
	matrix m = IdentityMat();
	m.m[3][0] = x;
	m.m[3][1] = y;
	m.m[3][2] = z;
	return m;
}
matrix RotaXMat(float theta) {
	matrix m = IdentityMat();
	m.m[1][1] = cos(theta); m.m[1][2] = sin(theta);
	m.m[2][1] = -sin(theta); m.m[2][2] = cos(theta);
	return m;
}
matrix RotaYMat(float theta) {
	matrix m = IdentityMat();
	m.m[0][0] = cos(theta); m.m[0][2] = -sin(theta);
	m.m[2][0] = sin(theta); m.m[2][2] = cos(theta);
	return m;
}
matrix RotaZMat(float theta) {
	matrix m = IdentityMat();
	m.m[0][0] = cos(theta); m.m[0][1] = sin(theta);
	m.m[1][0] = -sin(theta); m.m[1][1] = cos(theta);
	return m;
}
matrix ScalMat(float x, float y, float z) {
	matrix m = IdentityMat();
	m.m[0][0] = x;
	m.m[1][1] = y;
	m.m[2][2] = z;
	return m;
}

matrix LooAtMat(Vector4 viewpoint, Vector4 target, Vector4 up) {
	matrix m = IdentityMat();
	matrix trans = IdentityMat(); 
	matrix rota = IdentityMat();

	Vector4 dir = target - viewpoint;
	trans.m[3][0] = dir.v[0];
	trans.m[3][1] = dir.v[1];
	trans.m[3][2] = dir.v[2];

	Vector4 x, y = up, z;
	z = Normalize(viewpoint-target);
	x = Normalize(Cross(y, z));

	rota.m[0][0] = x.v[0];
	rota.m[1][0] = x.v[1];
	rota.m[2][0] = x.v[2];

	rota.m[0][1] = y.v[0];
	rota.m[1][1] = y.v[1];
	rota.m[2][1] = y.v[2];

	rota.m[0][2] = z.v[0];
	rota.m[1][2] = z.v[1];
	rota.m[2][2] = z.v[2];

	m = Mul(trans, rota);
	return m;
}

matrix ProjMat(float fovY, float aspect, float nearZ, float farZ) {
	matrix m = IdentityMat();
	m.m[0][0] = 1 / (aspect*tan(fovY / 2));
	m.m[1][1] = 1 / (tan(fovY/2));
	m.m[2][2] = farZ / (farZ-nearZ);
	m.m[3][2] = -farZ*nearZ / (farZ - nearZ);
	return m;
}

void ScreenMapping(float x, float y, float z) {

}