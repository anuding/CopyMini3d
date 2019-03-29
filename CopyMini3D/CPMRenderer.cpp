#include "CPMRenderer.h"
#include <iostream>
#include "utils.h"
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
float **zbuffer;
float maxz = INT_MIN, minz = INT_MAX, sum = 0;
Timer timer;


CPMRenderer::CPMRenderer(CPMScene& _scene, CPMDevice& _device)
{
	scene = &_scene;
	device = &_device;
	g_frame_buffer = device->framebuffer;
	zbuffer = device->zbuffer;
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

void SetPixel(Vertex v)
{
	if (v.pos.x >= 800 || v.pos.x <= 0 || v.pos.y >= 600 || v.pos.y <= 0)
		return;
	int x = v.pos.x, y = v.pos.y;
	//sum+=timer.tick();
	//maxz = max(maxz, v.pos.z);
	//minz = min(minz, v.pos.z);
	//if (sum > 10000)
	//{
	//	cout << maxz << " " << minz << endl;
	//	sum = 0;
	//}
	COLORREF cr =RGB(v.color.r*255, v.color.g*255, v.color.b*255);
	if (v.pos.z < zbuffer[y][x])
	{
		g_frame_buffer[y][x] = cr;
		zbuffer[y][x] = v.pos.z;
	}
		
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
void DrawXScanline(Vertex a, Vertex b)
{
	for (float x = a.pos.x+1; x <= b.pos.x; x++)
	{
		float grad = (x - a.pos.x) / (b.pos.x - a.pos.x);
		Vertex v = Interpolate(a, b, grad);

		SetPixel(v);
	}
}

void DrawTriA(Vertex top, Vertex left, Vertex right)
{
	/*
	top
	|\
	| \
	|  \
	|___\
	left right
	*/
	if (abs(left.pos.y - top.pos.y) < 1 || abs(right.pos.y - top.pos.y) < 1)
		return;
	for (int y = left.pos.y; y <= top.pos.y; y++)
	{
		float grad = (y - left.pos.y) / (top.pos.y - left.pos.y);
		if (grad < 0)
			continue;
		Vertex lv = Interpolate(left, top, grad);
		Vertex rv = Interpolate(right, top, grad);

		//if (lv.pos.x < 200)
		//	getchar();
		DrawXScanline(lv, rv);
	}
}
void DrawTriV(Vertex bottom, Vertex left, Vertex right)
{
	/*
	left  right
	_____
	|   /
	|  /
	| /
	|/
	bottom
	*/
	if (abs(left.pos.y - bottom.pos.y) < 1 || abs(right.pos.y - bottom.pos.y) < 1)
		return;
	for (int y = bottom.pos.y; y <= left.pos.y; y++)
	{
		float grad = (y - bottom.pos.y) / (left.pos.y - bottom.pos.y);
		if (grad < 0)
			continue;
		Vertex lv = Interpolate(bottom, left, grad);
		Vertex rv = Interpolate(bottom, right, grad);

		DrawXScanline(lv, rv);
	}
}

void CPMRenderer::Draw()
{
	//DrawLine(10, 100, 300, 40, RED);
	auto obj = scene->obj_list[0];
	std::vector<Vertex> points_buffer;
	std::vector<int> index_buffer;

	points_buffer = obj->vertex_list;
	index_buffer = obj->index_list;


	//World -> View -> Projection 
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

		points_buffer[i].pos = Mul(points_buffer[i].pos, scal);
		points_buffer[i].pos = Mul(points_buffer[i].pos, rotaX);
		points_buffer[i].pos = Mul(points_buffer[i].pos, rotaY);
		points_buffer[i].pos = Mul(points_buffer[i].pos, view);
		points_buffer[i].pos = Mul(points_buffer[i].pos, proj);


		//perspective divsion
		points_buffer[i].pos.x = points_buffer[i].pos.x / points_buffer[i].pos.w;
		points_buffer[i].pos.y = points_buffer[i].pos.y / points_buffer[i].pos.w;
		points_buffer[i].pos.z = points_buffer[i].pos.z / points_buffer[i].pos.w;
		points_buffer[i].pos.w = points_buffer[i].pos.w / points_buffer[i].pos.w;

		//screen mapping
		points_buffer[i].pos.x = points_buffer[i].pos.x * 100.0f + 800 / 2;
		points_buffer[i].pos.y = -points_buffer[i].pos.y * 100.0f + 600 / 2;
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
			Vertex a = points_buffer[index_buffer[i]];
			Vertex b = points_buffer[index_buffer[i + 1]];
			Vertex c = points_buffer[index_buffer[i + 2]];
			
			if (a.pos.y < b.pos.y)
				swap(a, b);
			if (a.pos.y < c.pos.y)
				swap(a, c);
			if (b.pos.y < c.pos.y)
				swap(b, c);
			Vertex d;
			float grad = (b.pos.y-c.pos.y) / (a.pos.y-c.pos.y);

			d = Interpolate(c, a, grad);
			if (b.pos.x > d.pos.x)
			{
				/*
				a
				| \
				d	b
				| /
				c
				*/
				DrawTriA(a, d, b);
				DrawTriV(c, d, b);
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
				DrawTriA(a, b, d);
				DrawTriV(c, b, d);
			}
		}
	}
}
