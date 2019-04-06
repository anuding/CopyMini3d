#pragma once
#include "Shader.h"
#include "Geometry.h"
#include <vector>
using namespace std;
class VertexShader :
	public Shader
{
public:
	VertexShader();
	~VertexShader();
	virtual void Shading(
		vector<Vector3>& pos_list,
		vector<Index>& index_list,
		vector<Vector3>& tex_list,
		vector<Vector3>& normal_list);
public://In resource
	matrix world, view, proj;
public://Out
	std::vector<Fragment> fragment_buffer;//screen_pos tex normal
	std::vector<pair<Vector4, Vector3>> pos_buffer;
private:
	void Scanline(Fragment a, Fragment b);
	void DrawTriA(Fragment top, Fragment left, Fragment right);
	void DrawTriV(Fragment bottom, Fragment left, Fragment right);
};

