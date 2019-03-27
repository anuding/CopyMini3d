#include "matrix.h"
float Dot(const Vector4& v1, const Vector4& v2)
{
	float ans;
	float a1, b1, c1, a2, b2, c2;
	a1 = v1.x; b1 = v1.y; c1 = v1.z;
	a2 = v2.x; b2 = v2.y; c2 = v2.z;
	ans = a1 * a2 + b1 * b2 + c1 * c2;
	return ans;
}

Vector4 Cross(const Vector4& v1, const Vector4& v2)
{
	Vector4 ans;
	float a1, b1, c1, a2, b2, c2;
	a1 = v1.x; b1 = v1.y; c1 = v1.z;
	a2 = v2.x; b2 = v2.y; c2 = v2.z;
	ans.SetVector(b1*c2 - b2 * c1, c1*a2 - a1 * c2, a1*b2 - a2 * b1, 0);
	return ans;
}
Vector4 Normalize(const Vector4& v1)
{
	Vector4 ans;
	float k = sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
	ans.SetVector(v1.x / k, v1.y / k, v1.z / k, 0);
	return ans;
}

Vector4 Mul(const Vector4& v, const matrix& m)
{
	Vector4 ans = { 0 };
	ans.x = v.x*m.m[0][0] + v.y*m.m[1][0] + v.z*m.m[2][0] + v.w*m.m[3][0];
	ans.y = v.x*m.m[0][1] + v.y*m.m[1][1] + v.z*m.m[2][1] + v.w*m.m[3][1];
	ans.z = v.x*m.m[0][2] + v.y*m.m[1][2] + v.z*m.m[2][2] + v.w*m.m[3][2];
	ans.w = v.x*m.m[0][3] + v.y*m.m[1][3] + v.z*m.m[2][3] + v.w*m.m[3][3];
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
	Vector4 z = Normalize(target - viewpoint);
	Vector4 x = Normalize(Cross(up, z));
	Vector4 y = Normalize(Cross(z, x));
	m.m[0][0] = x.x;
	m.m[1][0] = x.y;
	m.m[2][0] = x.z;
	m.m[3][0] = -Dot(viewpoint, x);

	m.m[0][1] = y.x;
	m.m[1][1] = y.y;
	m.m[2][1] = y.z;
	m.m[3][1] = -Dot(viewpoint, y);

	m.m[0][2] = z.x;
	m.m[1][2] = z.y;
	m.m[2][2] = z.z;
	m.m[3][2] = -Dot(viewpoint, z);

	return m;
}

matrix ProjMat(float fovY, float aspect, float nearZ, float farZ) {
	matrix m = IdentityMat();
	m.m[0][0] = 1 / (aspect*tan(fovY / 2));
	m.m[1][1] = 1 / (tan(fovY / 2));
	m.m[2][2] = farZ / (farZ - nearZ);
	m.m[3][2] = -farZ * nearZ / (farZ - nearZ);
	m.m[2][3] = 1;
	m.m[3][3] = 0;
	return m;
}

void ScreenMapping(float x, float y, float z) {

}