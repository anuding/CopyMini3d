#include "CPMRenderer.h"
#include "CPMLight.h"
#include <iostream>
#define RGBA(r,g,b,a)   ((COLORREF) ((uint8_t)(a)<<24)|\
									((uint8_t)(r)<<16|\
									((uint8_t)(g)<<8)|\
									((uint8_t)(b))))                                   
#define ABS(a) ((a) >= 0 ? (a) : -(a))
#define PI 3.1415926

#define RED RGBA(255,0,0,0)
#define GREEN RGBA(0,255,0,0)
#define BLUE RGBA(0,0,255,0)
#define YELLOW RGBA(255,255,0,0)
#define PURPLE RGBA(255,0,255,0)
#define LIGHTBLUE RGBA(0,255,255,0)
#define WHITE RGBA(255,255,255,0)
#define BLACK RGBA(0,0,0,0)
#define CR_BK RGBA(255,250,250,0)


IUINT32 **g_frame_buffer;
float **zbuffer;


Timer timer;



CPMRenderer::CPMRenderer(CPMScene& _scene, CPMDevice& _device)
{
	scene = &_scene;
	device = &_device;
	g_frame_buffer = device->framebuffer;
	zbuffer = device->zbuffer;

	
	matrix proj = ProjMat(3.1415926f * 0.1f, 800.0f / 800.0f, 1.0f, 500.0f);
	VS.proj = proj;

	PS = new PixelShader(g_frame_buffer, zbuffer);

	std::vector<const char*> texture_names(4);
	texture_names[0] = "test.png";

	texture_names[0] = "../res/rustediron1-alt2-Unreal-Engine/rustediron2_basecolor.png";
	//texture_names[0] = "../res/skybox/left.png";
	texture_names[1] = "../res/rustediron1-alt2-Unreal-Engine/rustediron2_roughness.png";
	texture_names[2] = "../res/rustediron1-alt2-Unreal-Engine/rustediron2_metallic.png";
	texture_names[3] = "white.png";

	//texture_names[0] = "../res/Copper-scuffed_Unreal-Engine/Copper-scuffed_basecolor.png";
	//texture_names[1] = "../res/Copper-scuffed_Unreal-Engine/Copper-scuffed_roughness.png";
	//texture_names[2] = "../res/Copper-scuffed_Unreal-Engine/Copper-scuffed_metallic.png";
	texture_names[3] = "../res/white.png";
	PS->LoadTextures(texture_names);
}

CPMRenderer::~CPMRenderer()
{
}

void SetPixel(int x, int y, COLORREF cr)
{
	if (x >= 800 || x <= 0 || y >= 600 || y <= 0)
		return;
	g_frame_buffer[y][x] = cr;
}

void SetPixel(Vector4 v, COLORREF cr)
{
	if (v.x >= 800 || v.x <= 0 || v.y >= 600 || v.y <= 0)
		return;
	int x = v.x, y = v.y;
	g_frame_buffer[y][x] = cr;
}


float theta = 0.0f;

//bool SetPixel(Vertex v)
//{
//	if (v.pos.x >= 800 || v.pos.x <= 0 || v.pos.y >= 600 || v.pos.y <= 0)
//		return true;
//	int x = v.pos.x, y = v.pos.y;
//
//	if (isnan(v.pos.z))
//		return false;
//	if (v.pos.z < zbuffer[y][x])
//	{
//		
//		Vector3 tex_color = Uint32ToVector(GetColor(v.tc.x, v.tc.y));
//		
//		Vector3 ld(cos(theta),1 , sin(theta));//light dir
//		ld = Normalize(ld);
//		Vector3 l(0.8, 0.8, 0.8);//light color
//		Vector3 m(1.0, 1.0, 1.0);//material
//		Vector3 n = Normalize(v.normal);
//
//		Vector3 diffuse = Modulate(l, tex_color) * max(Dot(ld, n), 0);
//
//		g_frame_buffer[y][x] = RGB(diffuse.z*255, 
//			diffuse.y * 255, 
//			diffuse.x * 255);
//		zbuffer[y][x] = v.pos.z;
//	}
//	return true;
//}

void DrawLine(int x0, int y0, int x1, int y1, COLORREF color)
{
	int dx = ABS(x1 - x0);
	int dy = ABS(y1 - y0);
	int x = x0;
	int y = y0;
	int stepX = 1;
	int stepY = 1;
	if (x0 > x1)  //从右向左画
		stepX = -1;
	if (y0 > y1)
		stepY = -1;

	if (dx > dy)  //沿着最长的那个轴前进
	{
		int e = -dx;
		int i;
		for (i = 0; i <= dx; i++)
		{
			SetPixel(x, y, color);
			x += stepX;
			e += 2 * dy;
			if (e >= 0)
			{
				y += stepY;
				e -= 2 * dx;
			}
		}
	}
	else
	{
		int e = 2 * dx - dy;
		int i;
		for (i = 0; i <= dy; i++)
		{
			SetPixel(x, y, color);
			y += stepY;
			e += dx;
			if (e >= 0)
			{
				x += stepX;
				e -= dy;
			}
		}
	}
}
void DrawLine(Vector4 p0, Vector4 p1, COLORREF color)
{
	int x0 = p0.x; int y0 = p0.y;
	int x1 = p1.x; int y1 = p1.y;
	int dx = ABS(x1 - x0);
	int dy = ABS(y1 - y0);
	int x = x0;
	int y = y0;
	int stepX = 1;
	int stepY = 1;
	if (x0 > x1)  //从右向左画
		stepX = -1;
	if (y0 > y1)
		stepY = -1;

	if (dx > dy)  //沿着最长的那个轴前进
	{
		int e = -dx;
		int i;
		for (i = 0; i <= dx; i++)
		{
			SetPixel(x, y, color);
			x += stepX;
			e += 2 * dy;
			if (e >= 0)
			{
				y += stepY;
				e -= 2 * dx;
			}
		}
	}
	else
	{
		int e = 2 * dx - dy;
		int i;
		for (i = 0; i <= dy; i++)
		{
			SetPixel(x, y, color);
			y += stepY;
			e += dx;
			if (e >= 0)
			{
				x += stepX;
				e -= dy;
			}
		}
	}
}
void DrawLine(Vertex p0, Vertex p1, COLORREF color)
{
	int x0 = p0.pos.x; int y0 = p0.pos.y;
	int x1 = p1.pos.x; int y1 = p1.pos.y;
	int dx = ABS(x1 - x0);
	int dy = ABS(y1 - y0);
	int x = x0;
	int y = y0;
	int stepX = 1;
	int stepY = 1;
	if (x0 > x1)  //从右向左画
		stepX = -1;
	if (y0 > y1)
		stepY = -1;

	if (dx > dy)  //沿着最长的那个轴前进
	{
		int e = -dx;
		int i;
		for (i = 0; i <= dx; i++)
		{
			SetPixel(x, y, color);
			x += stepX;
			e += 2 * dy;
			if (e >= 0)
			{
				y += stepY;
				e -= 2 * dx;
			}
		}
	}
	else
	{
		int e = 2 * dx - dy;
		int i;
		for (i = 0; i <= dy; i++)
		{
			SetPixel(x, y, color);
			y += stepY;
			e += dx;
			if (e >= 0)
			{
				x += stepX;
				e -= dy;
			}
		}
	}
}

void CPMRenderer::Draw()
{
	//DrawLine(10, 100, 300, 40, RED);
	auto obj = scene->obj_list[0];

	Vector4 viewpoint, target, up;
	viewpoint.SetVector(20 * cos(obj->rotaY), obj->rotaX, 20 * sin(obj->rotaY), 1);
	target.SetVector(0, 0, 0, 1);
	up.SetVector(0, 1, 0, 0);
	matrix view = LooAtMat(viewpoint, target, up);
	float size = obj->size;
	matrix world = ScalMat(size, size, size);

	VS.viewpoint = target - viewpoint;
	VS.world = world;
	VS.view = view;
	VS.Shading(obj->pos_list, obj->index_list, obj->tex_list, obj->normal_list);
	VS.culling_mode = BACKFACE;
	//VS.culling_mode = FRONTFACE;

	auto index_list = obj->index_list;
	if (mode == WIRE_FRAME)
	{
		for (int i = 0; i < obj->index_list.size(); i += 3)
		{
			DrawLine(VS.pos_buffer[index_list[i].pos].first, VS.pos_buffer[index_list[i + 1].pos].first, RED);
			DrawLine(VS.pos_buffer[index_list[i + 1].pos].first, VS.pos_buffer[index_list[i + 2].pos].first, RED);
			DrawLine(VS.pos_buffer[index_list[i].pos].first, VS.pos_buffer[index_list[i + 2].pos].first, RED);
		}
	}
	if (mode == CONSTANT_COLOR)
	{
		Vector3 cam(viewpoint.x, viewpoint.y, viewpoint.z);
		CPMLight light;
		light.theta = theta;
		PS->camPos = cam;
		PS->light = light;
		PS->Shading(VS.fragment_buffer);
	}
	if (mode == TEXTURE)
	{

	}
	VS.fragment_buffer.clear();
}
