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
IUINT32 texture[256][256] = { 0 };
float maxz = INT_MIN, minz = INT_MAX, sum = 0;
Timer timer;
void decodeOneStep(const char* filename)
{
	std::vector<unsigned char> image; //the raw pixels
	unsigned width, height;

	//decode
	unsigned error = lodepng::decode(image, width, height, filename);

	//if there's an error, display it
	if (error) std::cout << "decoder error " << error << ": " << lodepng_error_text(error) << std::endl;

	//the pixels are now in the vector "image", 4 bytes per pixel, ordered RGBARGBA..., use it as texture, draw it, ...
	int cnt = 0;
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			texture[i][j] =RGB(image[cnt], image[cnt+1], image[cnt+2]);
			cnt += 4;
		}
	}
}



CPMRenderer::CPMRenderer(CPMScene& _scene, CPMDevice& _device)
{
	scene = &_scene;
	device = &_device;
	g_frame_buffer = device->framebuffer;
	zbuffer = device->zbuffer;
	//texture = device->texture;
	const char* filename = "test.png";
	decodeOneStep(filename);
}

CPMRenderer::~CPMRenderer()
{
}
IUINT32 GetColor(float u, float v)
{
	if (u < 0 || v < 0 || isnan(u) || isnan(v))
	{
		cout << "u: " << u << ", v: " << v << endl;
		COLORREF cr = RGB(0, 0, 0);
		return cr;
	}

	int x = u * 256.0f;
	int y = v * 256.0f;
	IUINT32 color = texture[x][y];
	return color;
}

Vector3 Uint32ToVector(IUINT32 color)
{
	BYTE r = color;
	BYTE g = color>>8;
	BYTE b = color >> 16;
	BYTE a = color >> 24;

	Vector3 ans(r/255.0f,g / 255.0f,b / 255.0f);
	return ans;
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

bool SetPixel(Vertex v)
{
	if (v.pos.x >= 800 || v.pos.x <= 0 || v.pos.y >= 600 || v.pos.y <= 0)
		return true;
	int x = v.pos.x, y = v.pos.y;

	if (isnan(v.pos.z))
		return false;
	if (v.pos.z < zbuffer[y][x])
	{
		//max(L*n,0)*B@m
		/*COLORREF cr = RGB(v.tc.x * 255, v.tc.y * 255, v.tc.z * 255);
		cr = RGB(v.normal.x * 255, v.normal.y * 255, v.normal.z * 255);*/
		Vector3 tex_color = Uint32ToVector(GetColor(v.tc.x, v.tc.y));
		
		//theta += 0.01f;
		//static float phi = 0.0f;
		Vector3 ld(cos(theta),1 , sin(theta));//light dir
		ld = Normalize(ld);
		Vector3 l(0.8, 0.8, 0.8);//light color
		Vector3 m(0.1, 1.0, 0.25);//material
		Vector3 n = Normalize(v.normal);

		Vector3 diffuse = Modulate(l, tex_color) * max(Dot(ld, n), 0);

		g_frame_buffer[y][x] = RGB(diffuse.z*255, 
			diffuse.y * 255, 
			diffuse.x * 255);
		zbuffer[y][x] = v.pos.z;
	}
	return true;
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
	for (float x = a.pos.x; x <= b.pos.x; x++)
	{
		float grad = (x - a.pos.x) / (b.pos.x - a.pos.x);
		if (isnan(grad) || grad < 0)
			continue;
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
	
	auto pos_list = obj->pos_list;
	std::vector<Vector4> pos_buffer;
	pos_buffer.resize(pos_list.size());

	std::vector<Vector4> normal_buffer;
	normal_buffer.resize(obj->normal_list.size());

	auto index_list=obj->index_list;


	//World -> View -> Projection 
	for (int i = 0; i < pos_list.size(); i++)
	{
		matrix wvp;
		Vector4 viewpoint, target, up;
		viewpoint.SetVector(20*cos(obj->rotaY), obj->rotaX, 20*sin(obj->rotaY), 1);
		target.SetVector(0, 0, 0, 1);
		up.SetVector(0, 1, 0, 0);
		matrix view = LooAtMat(viewpoint, target, up);
		matrix proj = ProjMat(3.1415926f * 0.1f, 800.0f / 800.0f, 1.0f, 500.0f);
		float size = obj->size;
		matrix scal = ScalMat(size, size, size);


		matrix rotaX = RotaXMat(0);
		matrix rotaY = RotaYMat(0);

		Vector4 tmp(pos_list[i], 1);
		pos_buffer[i] = Mul(tmp, scal);
		pos_buffer[i] = Mul(pos_buffer[i], rotaX);
		pos_buffer[i] = Mul(pos_buffer[i], rotaY);
		pos_buffer[i] = Mul(pos_buffer[i], view);
		pos_buffer[i] = Mul(pos_buffer[i], proj);


		//perspective divsion
		pos_buffer[i].x = pos_buffer[i].x / pos_buffer[i].w;
		pos_buffer[i].y = pos_buffer[i].y / pos_buffer[i].w;
		pos_buffer[i].z = pos_buffer[i].z / pos_buffer[i].w;
		pos_buffer[i].w = pos_buffer[i].w / pos_buffer[i].w;

		//screen mapping
		pos_buffer[i].x = pos_buffer[i].x * 100.0f + 800 / 2;
		pos_buffer[i].y = -pos_buffer[i].y * 100.0f + 600 / 2;

	}

	if (mode == WIRE_FRAME)
	{
		for (int i = 0; i < index_list.size(); i += 3)
		{
			DrawLine(pos_buffer[index_list[i].pos], pos_buffer[index_list[i + 1].pos], RED);
			DrawLine(pos_buffer[index_list[i + 1].pos], pos_buffer[index_list[i + 2].pos], RED);
			DrawLine(pos_buffer[index_list[i].pos], pos_buffer[index_list[i + 2].pos], RED);
		}
	}
	if (mode == CONSTANT_COLOR)
	{
		for (int i = 0; i < index_list.size(); i += 3)
		{
			Vertex a, b, c;
			a.pos = pos_buffer[index_list[i].pos];
			b.pos = pos_buffer[index_list[i + 1].pos];
			c.pos = pos_buffer[index_list[i + 2].pos];

			a.tc = obj->tex_list[index_list[i].tex];
			b.tc = obj->tex_list[index_list[i+1].tex];
			c.tc = obj->tex_list[index_list[i+2].tex];

			a.normal = obj->normal_list[index_list[i].normal];
			b.normal = obj->normal_list[index_list[i + 1].normal];
			c.normal = obj->normal_list[index_list[i + 2].normal];

			//判断绕序
			Vector4 e0 = a.pos - b.pos;
			Vector4 e1 = c.pos - b.pos;
			Vector4 n = Normalize(Cross(e0, e1));
			Vector4 eyesight = { 0,0,1,0 };
			if (Dot(eyesight, n) > 0)//同向,需要剔除
				continue;

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
	if (mode == TEXTURE)
	{

	}
}
