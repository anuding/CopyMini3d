#include "CPMRenderer.h"
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
			setPixel(x, y, color);
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
			setPixel(x, y, color);
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
	//Go through the pipeline
	for (int i = 0; i < 300; i++)
		SetPixel(i + 200, i, YELLOW);
	DrawLine(10, 100, 300, 40, RED);
}
