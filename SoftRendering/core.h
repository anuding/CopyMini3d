#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <tchar.h>
#include <windows.h>
//=====================================================================
// Win32 窗口及图形绘制：为 device 提供一个 DibSection 的 FB
//=====================================================================
extern int screen_w;
extern unsigned char *screen_fb;		// frame buffer
extern long screen_pitch;
extern int screen_w;
extern int screen_h;
extern int screen_exit;
extern int screen_mx;
extern int screen_my;
extern int screen_mb;
extern int screen_keys[512];	// 当前键盘按下状态
extern int screen_init(int w, int h, const TCHAR *title);	// 屏幕初始化
extern int screen_close(void);								// 关闭屏幕
extern void screen_dispatch(void);							// 处理消息
extern void screen_update(void);							// 显示 FrameBuffer


static HWND screen_handle = NULL;		// 主窗口 HWND
static HDC screen_dc = NULL;			// 配套的 HDC
static HBITMAP screen_hb = NULL;		// DIB
static HBITMAP screen_ob = NULL;		// 老的 BITMAP
// win32 event handler
static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);

#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#endif