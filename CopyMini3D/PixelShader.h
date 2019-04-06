#pragma once
#include "Shader.h"
#include "lodepng.h"
#include <iostream>
#include "Geometry.h"
#include <vector>
using namespace std;
typedef unsigned int IUINT32;
class PixelShader :
	public Shader
{
public:
	PixelShader(unsigned int **_frame_buffer,
		float **_zbuffer);
	PixelShader();
	~PixelShader();
	virtual void Shading(vector<Fragment> & fragment_buffer);
	unsigned int **frame_buffer;
	float **zbuffer;
	Vector3 camPos;
	vector<vector<unsigned int>> albedo;// , roughness, metallic, ao;
	void LoadTextures(vector<const char*> filenames);
	void decodeOneStep(const char* filename);
	Vector3 Uint32ToVector(IUINT32 color);
	IUINT32 GetColor(float u, float v);
};

