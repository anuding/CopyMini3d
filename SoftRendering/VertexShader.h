#pragma once
#include "Shader.h"
#include "Geometry.h"
#include <vector>
//using namespace std;

enum CULLING
{
	BACKFACE = 0,
	FRONTFACE = 1,
	NONE = 2
};

class VertexShader :
	public Shader
{
public:
	VertexShader();
	~VertexShader();
	virtual void Shading(
		std::vector<Vector3>& pos_list,
		std::vector<Index>& index_list,
		std::vector<Vector3>& tex_list,
		std::vector<Vector3>& normal_list);
public://In resource
	Vector4 viewpoint;
	matrix world, view, proj;
	CULLING culling_mode= BACKFACE;
public://Out
	std::vector<Fragment> fragment_buffer;//screen_pos tex normal
	std::vector<std::pair<Vector4, Vector3>> pos_buffer;
private:
	void Scanline(Fragment a, Fragment b);
	void DrawTriA(Fragment top, Fragment left, Fragment right);
	void DrawTriV(Fragment bottom, Fragment left, Fragment right);
};

