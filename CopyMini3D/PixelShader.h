#pragma once
#include "Shader.h"
#include "lodepng.h"
#include <iostream>
#include "Geometry.h"
#include <vector>
using namespace std;
typedef unsigned int IUINT32;
typedef vector<vector<unsigned int>> texture;
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

	texture albedo;// , roughness, metallic, ao;
	texture roughness;
	texture metallic;
	texture ao;

	void LoadTextures(vector<const char*> filenames);
	void decodeOneStep(const char* filename, vector<vector<unsigned int>>& tex);

	Vector3 GetColor(texture& tex, float u, float v);
};

