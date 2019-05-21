#pragma once
#include "CPMScene.h"
#include "CPMDevice.h"
#include "utils.h"
#include "matrix.h"
#include <Windows.h>
#include "VertexShader.h"
#include "PixelShader.h"
enum MODES{
	WIRE_FRAME = 0,
	CONSTANT_COLOR=1,
	TEXTURE=2
}; 
extern float theta;
class CPMRenderer
{
public:
	
	CPMRenderer(CPMScene &_scene, CPMDevice &_device);
	~CPMRenderer();

	MODES mode = WIRE_FRAME;

	CPMScene* scene;
	CPMDevice* device;
	VertexShader VS;
	PixelShader* PS;
	void Draw();
};