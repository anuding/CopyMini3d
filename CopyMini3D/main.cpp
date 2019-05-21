#include <iostream>
#include "GLHeader.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>


#include "GUI.h"
#include "Scene.h"
#include "MeshRenderer.h"
#include "MeshFilter.h"
using namespace std;
using namespace Engine;
//Configuration
int window_width = 1280;
int window_height = 720;
ImVec4 clear_color = ImVec4(255.0f / 255.0f, 42.0f / 255.0f, 72.0f / 255.0f, 1.00f);

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


int init(GLFWwindow* &window)
{
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if (!glfwInit())
		return 1;

	// Decide GL+GLSL versions
	// GL 3.0 + GLSL 130
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	// Create window with graphics context
	window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);

	if (window == NULL)
		return 1;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	// Initialize OpenGL loader
	bool err = gladLoadGL() == 0;
	if (err)
	{
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}
	return 0;
}

int main() {	
	const char* glsl_version = "#version 130";
	GLFWwindow* window;
	init(window);

	
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	ImFont* font = io.Fonts->AddFontFromFileTTF("../res/Futura LT Bold.ttf", 23.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	IM_ASSERT(font != NULL);

	Scene::Scene scene;
	typedef GameObjects::GameObject GameObject;
	typedef Components::MeshRenderer MeshRenderer;
	typedef Components::Component Component;
	GameObject *obj1 = new GameObject();
	scene.AddGameObject(obj1);
	
	Components::MeshRenderer* renderer = new Components::MeshRenderer();
	Components::MeshFilter* filter = new Components::MeshFilter();
	renderer->Bind(filter->vertices, filter->indices);
	obj1->AddCompoent(filter);
	obj1->AddCompoent(renderer);
	// Main loop
	while (!glfwWindowShouldClose(window) && !io.KeysDown[256])
	{
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		glfwPollEvents();

		
		static float eyex = 0.0f;
		static float eyey = 0.0f;
		static float eyez = -4.0f;
		float step = 0.01f;
		if (io.KeysDown['A'])
			eyex -= step;
		if (io.KeysDown['D'])
			eyex += step;
		if (io.KeysDown['W'])
			eyey -= step;
		if (io.KeysDown['S'])
			eyey += step;
		//cout << eyex << " " << eyey << " " << eyez << endl;


		scene.obj_list[0]->compoent_list[0]->Update();
		scene.obj_list[0]->compoent_list[1]->Update();
		//Update
		{
			//SettingsUpdate
			{
				//clear_color = CPM::Editor::UI::clear_color;
			}
			CPM::Editor::UI::UpdateUI();
		}
		
		//Render
		{
			CPM::Editor::UI::RenderUI();
		}
		
		

		int display_w, display_h;
		glfwGetFramebufferSize(window, &display_w, &display_h);
		//glViewport(display_w / 3, 50, display_w / 3 * 2 - 50, display_h - 100);
		glViewport(0, 0, display_w, display_h);
		glfwMakeContextCurrent(window);
		glfwSwapBuffers(window);
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

//int main()
//{
//	const TCHAR* title = _T("CPM");
//	if (screen_init(800, 600, title))
//		return -1;
//
//	Timer timer;
//	CPMDevice device(800, 600, screen_fb);
//	CPMScene scene;
//	CPMGameObject* tri = new CPMGameObject();
//	//tri->InitAsCube();
//	tri->InitFromObj("../res/SphereDetailed.obj");//Sphere1 SphereDetailed.obj Cube bunny_c4d_less
//	//tri->InitAsTriangle();
//	scene.AddGameObject(tri);
//	CPMRenderer renderer(scene, device);
//	float rox = 0;
//	float roy = 0;
//	MODES mode = CONSTANT_COLOR;
//	int indicator = 0;
//	int kbhit = 0;
//
//	while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0) {
//		timer.tick();
//		screen_dispatch();
//		device.Clear(0);
//
//		if (screen_keys['W']) rox -= 0.6f;
//		if (screen_keys['S']) rox += 0.6f;
//		if (screen_keys['A']) roy += 0.1f;
//		if (screen_keys['D']) roy -= 0.1f;
//
//		if (screen_keys['X']) tri->size -= 0.01f;
//		if (screen_keys['C']) tri->size += 0.01f;
//
//		if (screen_keys['J']) theta -= 0.1f;
//		if (screen_keys['K']) theta += 0.1f;
//		if (screen_keys[VK_SPACE])
//		{
//			rox = 0;
//			roy = 0;
//			tri->size = 3.0f / 100.0f;
//		}
//		if (screen_keys[VK_TAB])
//		{
//			if (kbhit == 0) {
//				kbhit = 1;
//				//if (++indicator >= 3) indicator = 0;
//				if (mode == WIRE_FRAME)
//				{
//					mode = CONSTANT_COLOR;
//					cout << "change to CONSTANT_COLOR mode\n";
//				}
//				else
//				{
//					mode = WIRE_FRAME;
//					cout << "change to WIRE_FRAME mode\n";
//				}
//			}
//		}
//		else {
//			kbhit = 0;
//		}
//		renderer.mode = mode;
//		tri->rotaX = rox;
//		tri->rotaY = roy;
//		//cout << rox << " " << roy << endl;
//		renderer.Draw();
//
//		screen_update();
//		int fps = 1000.0 / timer.tick();
//		bool res = SetWindowTextA(screen_handle, (LPCSTR)fps);
//		cout << "FPS: " << fps << endl;
//		Sleep(1);
//	}
//
//}