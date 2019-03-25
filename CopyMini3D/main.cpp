#include <iostream>
//#include "CPMDevice.h"
#include "CPMRenderer.h"
#include "utils.h"
using namespace std;

int main() {
	const TCHAR *title = _T("Mini3d (software render tutorial) - Left/Right: rotation, Up/Down: forward/backward, Space: switch state");
	if (screen_init(800, 600, title))
		return -1;
	
	Timer timer;
	CPMDevice device(800, 600, screen_fb);
	CPMScene scene;
	CPMGameObject* tri=new CPMGameObject();
	tri->InitAsTriangle();
	scene.AddGameObject(tri);
	CPMRenderer renderer(scene,device);

	while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0) {
		timer.tick();
		screen_dispatch();
		device.Clear(0);
		

		renderer.Draw();
		
		screen_update();
		int fps = 1000.0 / timer.tick();
		bool res = SetWindowTextA(screen_handle, (LPCSTR)fps);
		//cout << "FPS: "<<fps << endl;
		Sleep(1);
	}
	

	return 0;
}