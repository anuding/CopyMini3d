#pragma once
#include "matrix.h"

//uint pos; uint tex; uint normal;
struct Index
{
	unsigned int pos;
	unsigned int tex;
	unsigned int normal;
};
typedef Vector4 Position;
typedef Vector3 color_t;
typedef Vector3 texcoord_t;
typedef Vector3 normal_t;
struct Vertex { Position pos; texcoord_t tc; color_t color; normal_t normal; float rhw; };
Vertex Interpolate(Vertex a, Vertex b, float grad);

//Vector3 screen_pos; Vector3 world_pos; Vector3 normal; Vector3 tex;
struct Fragment
{
	Vector4 screen_pos;
	Vector3 world_pos;
	Vector3 normal;
	Vector3 tex;
	Fragment() {
		
	}
};
Fragment Interpolate(Fragment a, Fragment b, float grad);