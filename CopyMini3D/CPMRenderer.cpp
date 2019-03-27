#include "CPMRenderer.h"
#include <iostream>

#include "matrix.h"
using namespace std;
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
CPMRenderer::CPMRenderer(CPMScene& _scene, CPMDevice& _device)
{
	scene = &_scene;
	device = &_device;
	g_frame_buffer = device->framebuffer;
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



void CPMRenderer::Draw()
{

	//DrawLine(10, 100, 300, 40, RED);
	auto obj = scene->obj_list[0];
	std::vector<Vector4> points_buffer;
	std::vector<int> index_buffer;

	for (int i = 0; i < obj->vertex_list.size(); i++)
	{
		Vector4 tmp = obj->vertex_list[i].pos;
		points_buffer.push_back(tmp);
	}
	index_buffer.insert(index_buffer.end(),
		obj->index_list.begin(), obj->index_list.end());


	//Vector4 
	for (int i = 0; i < points_buffer.size(); i++)
	{
		matrix wvp;
		Vector4 viewpoint, target, up;
		viewpoint.SetVector(0, 0, 20, 1);
		target.SetVector(0, 0, 0, 1);
		up.SetVector(0, 1, 0, 0);
		matrix view = LooAtMat(viewpoint, target, up);
		matrix proj = ProjMat(3.1415926f * 0.1f, 800.0f / 600.0f, 1.0f, 500.0f);
		float size = obj->size;
		matrix scal = ScalMat(size, size, size);


		matrix rotaX = RotaXMat(obj->rotaX);
		matrix rotaY = RotaYMat(obj->rotaY);

		points_buffer[i] = Mul(points_buffer[i], scal);
		points_buffer[i] = Mul(points_buffer[i], rotaX);
		points_buffer[i] = Mul(points_buffer[i], rotaY);
		points_buffer[i] = Mul(points_buffer[i], view);
		points_buffer[i] = Mul(points_buffer[i], proj);


		//perspective divsion
		points_buffer[i].x = points_buffer[i].x / points_buffer[i].w;
		points_buffer[i].y = points_buffer[i].y / points_buffer[i].w;
		points_buffer[i].z = points_buffer[i].z / points_buffer[i].w;
		points_buffer[i].w = points_buffer[i].w / points_buffer[i].w;

		//screen mapping
		points_buffer[i].x = points_buffer[i].x * 100.0f + 800 / 2;
		points_buffer[i].y = -points_buffer[i].y * 100.0f + 600 / 2;
	}
	if (mode == WIRE_FRAME)
	{
		for (int i = 0; i < index_buffer.size(); i += 3)
		{
			DrawLine(points_buffer[index_buffer[i]], points_buffer[index_buffer[i + 1]], RED);
			DrawLine(points_buffer[index_buffer[i + 1]], points_buffer[index_buffer[i + 2]], RED);
			DrawLine(points_buffer[index_buffer[i]], points_buffer[index_buffer[i + 2]], RED);
		}
	}
	if (mode == CONSTANT_COLOR)
	{
		for (int i = 0; i < index_buffer.size(); i += 3)
		{
			Vector4 a = points_buffer[index_buffer[i]];
			Vector4 b = points_buffer[index_buffer[i + 1]];
			Vector4 c = points_buffer[index_buffer[i + 2]];
			Vector4 tmp;
			if (a.y < b.y)
			{
				tmp = a;
				a = b;
				b = tmp;
			}
			if (a.y < c.y)
			{
				tmp = a;
				a = c;
				c = tmp;
			}
			if (b.y < c.y)
			{
				tmp = b;
				b = c;
				c = tmp;
			}
			Vector4 d;
			d.y = b.y;
			if (b.x > a.x)
			{
				/*
				a
				| \
				d	b
				| /
				c
				*/
				d.x = (d.y - c.y)*(a.x - c.x) / (a.y - c.y) + c.x;
				d.z = (d.y - c.y)*(a.z - c.z) / (a.y - c.y) + c.z;
				d.w = 1;
				float k1 = (a.y - d.y) / (a.x - d.x);
				float b1 = a.y - k1 * a.x;
				float k2 = (a.y - b.y) / (a.x - b.x);
				float b2 = a.y - k2 * a.x;
				for (int y = d.y; y <= a.y; y++)
				{
					int x1 = (y - b1) / k1;
					int x2 = (y - b2) / k2;
					if (x1 < 0 || x2 < 0)
					{
						cout << x1 << " " << x2 << endl;
						break;
						getchar();
						int l = 0;
					}
					DrawLine(x1, y, x2, y, RED);
				}
			}
			else
			{
				/*
					a
				  /	|
				b	d
				  \	|
					c
				*/
			}
			//getchar();
		}
	}
}
