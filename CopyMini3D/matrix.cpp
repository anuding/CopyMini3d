#include "matrix.h"

Vector4 Vector4::operator-(const Vector4& v1)const
{
	Vector4 ans;
	ans.SetVector(x - v1.x, y - v1.y, z - v1.z, 0);
	return ans;
}

Vector3 Modulate(const Vector3& v1, const Vector3& v2)
{
	Vector3 ans(v1.x*v2.x, v1.y*v2.y, v1.z*v2.z);
	return ans;
}
Vector3 Vector3::operator*(float k)
{
	Vector3 ans;
	ans.SetVector(x*k, y*k, z*k);
	return ans;
}
Vertex operator+(const Vertex& v1, const Vertex& v2)
{
	Vertex ans;
	Position pos = { v1.pos.x + v2.pos.x,v1.pos.y + v2.pos.y,v1.pos.z + v2.pos.z,0 };
	color_t color = { v1.color.x + v2.color.x,v1.color.y + v2.color.y,v1.color.z + v2.color.z };
	texcoord_t tex = { v1.tc.x + v2.tc.x,v1.tc.y + v2.tc.y,v1.tc.z + v2.tc.z };
	normal_t normal = { v1.normal.x + v2.normal.x,v1.normal.y + v2.normal.y,v1.normal.z + v2.normal.z };
	ans = { pos,tex,color,normal,1 };
	return ans;
}

Vertex operator-(const Vertex& v1, const Vertex& v2)
{
	Vertex ans;
	Position pos = { v1.pos.x - v2.pos.x,v1.pos.y - v2.pos.y,v1.pos.z - v2.pos.z,0 };
	color_t color = { v1.color.x - v2.color.x,v1.color.y - v2.color.y,v1.color.z - v2.color.z };
	texcoord_t tex = { v1.tc.x - v2.tc.x,v1.tc.y - v2.tc.y,v1.tc.z - v2.tc.z };
	normal_t normal = { v1.normal.x - v2.normal.x,v1.normal.y - v2.normal.y,v1.normal.z - v2.normal.z };
	ans = { pos,tex,color,normal,1 };
	return ans;
}

Vertex operator*(const Vertex& v1, float k)
{
	Vertex ans;
	Position pos = { v1.pos.x *k,v1.pos.y *k,v1.pos.z*k,0 };
	color_t color = { v1.color.x * k, v1.color.y * k, v1.color.z * k };
	texcoord_t tex = { v1.tc.x * k, v1.tc.y * k, v1.tc.z *k };
	normal_t normal = { v1.normal.x * k, v1.normal.y * k, v1.normal.z * k };
	ans = { pos,tex,color,normal,1 };
	return ans;
}


Vertex Interpolate(Vertex a, Vertex b, float grad)
{
	Vertex ans;
	ans = (b - a)*grad + a;
	return ans;
}

//v4 dot
float Dot(const Vector4& v1, const Vector4& v2)
{
	float ans;
	float a1, b1, c1, a2, b2, c2;
	a1 = v1.x; b1 = v1.y; c1 = v1.z;
	a2 = v2.x; b2 = v2.y; c2 = v2.z;
	ans = a1 * a2 + b1 * b2 + c1 * c2;
	return ans;
}

//v3 dot
float Dot(const Vector3& v1, const Vector3& v2)
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
Vector3 Normalize(const Vector3& v1)
{
	Vector3 ans;
	float k = sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
	ans.SetVector(v1.x / k, v1.y / k, v1.z / k);
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
	Vector4 ans;// = { 0 };
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

matrix LooAtMat(Vector4 viewVector4, Vector4 target, Vector4 up) {
	matrix m = IdentityMat();
	Vector4 z = Normalize(target - viewVector4);
	Vector4 x = Normalize(Cross(up, z));
	Vector4 y = Normalize(Cross(z, x));
	m.m[0][0] = x.x;
	m.m[1][0] = x.y;
	m.m[2][0] = x.z;
	m.m[3][0] = -Dot(viewVector4, x);

	m.m[0][1] = y.x;
	m.m[1][1] = y.y;
	m.m[2][1] = y.z;
	m.m[3][1] = -Dot(viewVector4, y);

	m.m[0][2] = z.x;
	m.m[1][2] = z.y;
	m.m[2][2] = z.z;
	m.m[3][2] = -Dot(viewVector4, z);

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