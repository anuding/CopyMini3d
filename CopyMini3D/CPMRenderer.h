#pragma once
#include "CPMScene.h"
#include "CPMDevice.h"
#include "lodepng.h"
#include <Windows.h>
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

	void Draw();
};