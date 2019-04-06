#include "Geometry.h"

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


Fragment operator-(const Fragment& v1, const Fragment& v2)
{
	Fragment ans;
	ans.screen_pos = v1.screen_pos - v2.screen_pos;
	ans.world_pos = v1.world_pos - v2.world_pos;
	ans.normal = v1.normal - v2.normal;
	ans.tex = v1.tex - v2.tex;
	return ans;
}
Fragment operator*(const Fragment& v1, const float f)
{
	Fragment ans;
	ans.screen_pos = v1.screen_pos*f;
	ans.world_pos = v1.world_pos*f;
	ans.normal = v1.normal*f;
	ans.tex = v1.tex*f;
	return ans;
}
Fragment operator+(const Fragment& v1, const Fragment& v2)
{
	Fragment ans;
	ans.screen_pos = v1.screen_pos + v2.screen_pos;
	ans.world_pos = v1.world_pos + v2.world_pos;
	ans.normal = v1.normal + v2.normal;
	ans.tex = v1.tex + v2.tex;
	return ans;
}

Fragment Interpolate(Fragment a, Fragment b, float grad)
{
	Fragment ans;
	ans = (b - a)*grad + a;
	return ans;
}