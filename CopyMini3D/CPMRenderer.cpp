#include "CPMRenderer.h"
#include <iostream>
#include "matrix.h"
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

void DrawLine(Vector4 p0, Vector4 p1, COLORREF color)
{
	int x0 = p0.v[0]; int y0 = p0.v[1];
	int x1 = p1.v[0]; int y1 = p1.v[1];
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
		Vector4 tmp;
		tmp.v[0] = obj->vertex_list[i].x; tmp.v[1] = obj->vertex_list[i].y;
		tmp.v[2] = obj->vertex_list[i].z; tmp.v[3] = 1;
		points_buffer.push_back(tmp);
	}
	index_buffer.insert(index_buffer.end(),
	obj->index_list.begin(), obj->index_list.end());


	//Vector4 
	for (int i = 0; i < points_buffer.size(); i++)
	{
		matrix wvp;
		Vector4 viewpoint, target, up;
		viewpoint.SetVector(0, 0, 10, 1);
		target.SetVector(0, 0, 0, 1);
		up.SetVector(0, 1, 0, 0);
		matrix view = LooAtMat(viewpoint, target, up);
		matrix proj = ProjMat(0.25f*PI,800.0f/600.0f,1.0f,1000.0f);
		matrix scal = ScalMat(55,55, 55);

		static float theta = 0.0f;
		theta += 0.001f;
		matrix rota = RotaXMat(theta);
		matrix world=Mul(scal,rota);
		matrix worldview = Mul(world, view);
		wvp = Mul(worldview, proj);
		points_buffer[i] = Mul(points_buffer[i], wvp);

		//perspective divsion
		points_buffer[i].v[0] = points_buffer[i].v[0] / points_buffer[i].v[3];
		points_buffer[i].v[1] = points_buffer[i].v[1] / points_buffer[i].v[3];
		points_buffer[i].v[2] = points_buffer[i].v[2] / points_buffer[i].v[3];
		points_buffer[i].v[3] = points_buffer[i].v[3] / points_buffer[i].v[3];

		//screen mapping
		points_buffer[i].v[0] = points_buffer[i].v[0]  + 800 / 2;
		points_buffer[i].v[1] = -points_buffer[i].v[1] + 600 / 2;
	}

	for (int i = 0; i < index_buffer.size(); i += 3)
	{
		DrawLine(points_buffer[index_buffer[i]],   points_buffer[index_buffer[i+1]], RED);
		DrawLine(points_buffer[index_buffer[i+1]], points_buffer[index_buffer[i+2]], RED);
		DrawLine(points_buffer[index_buffer[i]],   points_buffer[index_buffer[i+2]], RED);
	}

}
