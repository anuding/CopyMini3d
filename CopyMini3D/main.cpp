#include <iostream>
//#include "CPMDevice.h"


#include "CPMRenderer.h"
#include "utils.h"
using namespace std;

int main() {
	

	
	const TCHAR *title = _T("CPM");
	if (screen_init(800, 600, title))
		return -1;
	
	Timer timer;
	CPMDevice device(800, 600, screen_fb);
	CPMScene scene;
	CPMGameObject* tri=new CPMGameObject();
	//tri->InitAsCube();
	tri->InitFromObj("SphereDetailed.obj");//Sphere1 SphereDetailed.obj Cube bunny_c4d_less
	//tri->InitAsTriangle();
	scene.AddGameObject(tri);
	CPMRenderer renderer(scene,device);
	float rox = 0;
	float roy = 0;
	MODES mode = CONSTANT_COLOR;
	int indicator = 0;
	int kbhit = 0;

	while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0) {
		timer.tick();
		screen_dispatch();
		device.Clear(0);

		if (screen_keys['W']) rox -= 0.6f;
		if (screen_keys['S']) rox += 0.6f;
		if (screen_keys['A']) roy += 0.1f;
		if (screen_keys['D']) roy -= 0.1f;

		if (screen_keys['X']) tri->size -= 0.01f;
		if (screen_keys['C']) tri->size += 0.01f;

		if (screen_keys['J']) theta -= 0.1f;
		if (screen_keys['K']) theta += 0.1f;
		if (screen_keys[VK_SPACE])
		{
			rox = 0;
			roy = 0;
			tri->size=3.0f / 100.0f;
		}
		if (screen_keys[VK_TAB]) 
		{
			if (kbhit == 0) {
				kbhit = 1;
				//if (++indicator >= 3) indicator = 0;
				if (mode == WIRE_FRAME)
				{
					mode = CONSTANT_COLOR;
					cout << "change to CONSTANT_COLOR mode\n";
				}
				else
				{
					mode = WIRE_FRAME;
					cout << "change to WIRE_FRAME mode\n";
				}
			}
		}
		else {
			kbhit = 0;
		}
		renderer.mode = mode;
		tri->rotaX = rox;
		tri->rotaY = roy;
		//cout << rox << " " << roy << endl;
		renderer.Draw();
		
		screen_update();
		int fps = 1000.0 / timer.tick();
		bool res = SetWindowTextA(screen_handle, (LPCSTR)fps);
		cout << "FPS: "<<fps << endl;
		Sleep(1);
	}
	

	return 0;
}
