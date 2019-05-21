#pragma once
#include "Shader.h"
#include "lodepng.h"
#include <iostream>
#include "Geometry.h"
#include <vector>
#include "CPMLight.h"
//using namespace std::std::vector;
typedef unsigned int IUINT32;
typedef std::vector<std::vector<unsigned int>> texture;
class PixelShader :
	public Shader
{
public:
	PixelShader(unsigned int **_frame_buffer,
		float **_zbuffer);
	PixelShader();
	~PixelShader();
	virtual void Shading(std::vector<Fragment> & fragment_buffer);
	unsigned int **frame_buffer;
	float **zbuffer;
	Vector3 camPos;

	texture albedo;// , roughness, metallic, ao;
	texture roughness;
	texture metallic;
	texture ao;
	CPMLight light;
	void LoadTextures(std::vector<const char*> filenames);
	void decodeOneStep(const char* filename, std::vector<std::vector<unsigned int>>& tex);

	Vector3 GetColor(texture& tex, float u, float v);


private:
	void PBRShading(std::vector<Fragment> & fragment_buffer);
	void LightShading(std::vector<Fragment> & fragment_buffer);
	//void ConstantShading(std::vector<Fragment> & fragment_buffer);
};

