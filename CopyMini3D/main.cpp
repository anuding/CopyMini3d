#include <iostream>
#include "renderer.h"
#include "utils.h"
using namespace std;

int main() {
	const TCHAR *title = _T("Mini3d (software render tutorial) - Left/Right: rotation, Up/Down: forward/backward, Space: switch state");

	if (screen_init(800, 600, title))
		return -1;
	device_t state;
	InitStateMachine(&state, 800, 600, screen_fb);
	device_clear(&state, 0);
	
	for (int i = 0; i < 300; i++)
		state.framebuffer[500][i] = 0;
	screen_update();

	while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0) {
		screen_dispatch();
		GetNowTime();
		//Sleep(1);
	}
	

	return 0;
}