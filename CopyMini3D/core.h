#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <tchar.h>
#include <windows.h>
//=====================================================================
// Win32 ���ڼ�ͼ�λ��ƣ�Ϊ device �ṩһ�� DibSection �� FB
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
extern int screen_keys[512];	// ��ǰ���̰���״̬
extern int screen_init(int w, int h, const TCHAR *title);	// ��Ļ��ʼ��
extern int screen_close(void);								// �ر���Ļ
extern void screen_dispatch(void);							// ������Ϣ
extern void screen_update(void);							// ��ʾ FrameBuffer


static HWND screen_handle = NULL;		// ������ HWND
static HDC screen_dc = NULL;			// ���׵� HDC
static HBITMAP screen_hb = NULL;		// DIB
static HBITMAP screen_ob = NULL;		// �ϵ� BITMAP
// win32 event handler
static LRESULT screen_events(HWND, UINT, WPARAM, LPARAM);

#ifdef _MSC_VER
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "user32.lib")
#endif