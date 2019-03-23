#include "CPMRenderer.h"
#include <iostream>
#include "matrix.h"
#define RGBA(r,g,b,a)   ((COLORREF) ((uint8_t)(a)<<24)|\
									((uint8_t)(r)<<16|\
									((uint8_t)(g)<<8)|\
									((uint8_t)(b))))                                   
#define ABS(a) ((a) >= 0 ? (a) : -(a))
#define PI 3.14159

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

void DrawLine(Vertex p0, Vertex p1, COLORREF color)
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
	vertex_buffer.clear();
	index_buffer.clear();
	vertex_buffer.insert(vertex_buffer.end(),
	obj->vertex_list.begin(), obj->vertex_list.end());
	index_buffer.insert(index_buffer.end(),
	obj->index_list.begin(), obj->index_list.end());


	for (int i = 0; i < vertex_buffer.size(); i++)
	{
		vertex_buffer[i].x = vertex_buffer[i].x * 20 + 50;
		vertex_buffer[i].y = vertex_buffer[i].y * (-20)+50;
	}

	for (int i = 0; i < index_buffer.size(); i += 3)
	{
		DrawLine(vertex_buffer[index_buffer[i]], vertex_buffer[index_buffer[i+1]], RED);
		DrawLine(vertex_buffer[index_buffer[i+1]], vertex_buffer[index_buffer[i+2]], RED);
		DrawLine(vertex_buffer[index_buffer[i]], vertex_buffer[index_buffer[i+2]], RED);
	}

	//matrix a = matrix_set_translate(1,2,3);
	//matrix b= matrix_set_translate(1, 2, 3);
	//auto m = MatMul(&a, &b);
	//for (int i = 0; i < 4; i++)
	//{
	//	for (int j = 0; j < 4; j++)
	//		std::cout << m.m[i][j] << " ";
	//	std::cout << std::endl;
	//}
	//getchar();
	
}
