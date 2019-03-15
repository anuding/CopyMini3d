#include "CPMDevice.h"
CPMDevice::CPMDevice(int width, int height, void* fb)
{
	Init(width, height, fb);
}

void CPMDevice::Init(int width, int height, void* fb)
{
	int need = sizeof(void*) * (height * 2 + 1024) + width * height * 8;
	char *ptr = (char*)malloc(need + 64);
	char *framebuf, *zbuf;
	int j;
	assert(ptr);
	framebuffer = (IUINT32**)ptr;
	zbuffer = (float**)(ptr + sizeof(void*) * height);
	ptr += sizeof(void*) * height * 2;
	texture = (IUINT32**)ptr;
	ptr += sizeof(void*) * 1024;
	framebuf = (char*)ptr;
	zbuf = (char*)ptr + width * height * 4;
	ptr += width * height * 8;
	if (fb != NULL) framebuf = (char*)fb;
	for (j = 0; j < height; j++) {
		framebuffer[j] = (IUINT32*)(framebuf + width * 4 * j);
		zbuffer[j] = (float*)(zbuf + width * 4 * j);
	}
	texture[0] = (IUINT32*)ptr;
	texture[1] = (IUINT32*)(ptr + 16);
	memset(texture[0], 0, 64);
	tex_width = 2;
	tex_height = 2;
	max_u = 1.0f;
	max_v = 1.0f;
	this->width = width;
	this->height = height;
	background = 0xc0c0c0;
	foreground = 0;
	//transform_init(&transform, width, height);
	render_state = RENDER_STATE_WIREFRAME;
}


// Çå¿Õ framebuffer ºÍ zbuffer
void CPMDevice::Clear(int mode) {
	int y, x;
	for (y = 0; y < height; y++) {
		IUINT32 *dst = framebuffer[y];
		IUINT32 cc = (height - 1 - y) * 230 / (height - 1);
		cc = (cc << 16) | (cc << 8) | cc;
		if (mode == 0) cc = background;
		for (x = width; x > 0; dst++, x--) dst[0] = cc;
	}
	for (y = 0; y < height; y++) {
		float *dst = zbuffer[y];
		for (x = width; x > 0; dst++, x--) dst[0] = 0.0f;
	}

}


