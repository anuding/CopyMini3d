#include "GUI.h"
// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
//#include <stdio.h>
#include"GLHeader.h"
namespace CPM::Editor
{
	ImVec4 clear_color;

	void UI::UpdateUI()
	{
		static bool show_demo_window = false;
		static bool show_another_window = false;
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				//ShowExampleMenuFile();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::ColorEdit3("clear color", (float*)& clear_color); // Edit 3 floats representing a color
				ImGui::EndMenu();
			}
			ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::EndMainMenuBar();
		}
		// Create hierarchy window
		{
			ImGui::Begin("Hierarchy");
			if (ImGui::TreeNode("Advanced, with Selectable nodes"))
			{
				static int selection_mask = (1 << 2); // Dumb representation of what may be user-side selection state. You may carry selection state inside or outside your objects in whatever format you see fit.
				int node_clicked = -1;                // Temporary storage of what node we have clicked to process selection at the end of the loop. May be a pointer to your own node type, etc.
				ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.
				for (int i = 0; i < 3; i++)
				{
					// Disable the default open on single-click behavior and pass in Selected flag according to our selection state.
					ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
					if (selection_mask & (1 << i))
						node_flags |= ImGuiTreeNodeFlags_Selected;
					if (i < 3)
					{
						// Items 0..2 are Tree Node
						bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, "Selectable Node %d", i);
						if (ImGui::IsItemClicked())
							node_clicked = i;
						if (node_open)
						{
							ImGui::Text("Blah blah\nBlah Blah");
							ImGui::TreePop();
						}
					}
				}
				if (node_clicked != -1)
				{
					// Update selection state. Process outside of tree loop to avoid visual inconsistencies during the clicking-frame.
					if (ImGui::GetIO().KeyCtrl)
						selection_mask ^= (1 << node_clicked);          // CTRL+click to toggle
					else //if (!(selection_mask & (1 << node_clicked))) // Depending on selection behavior you want, this commented bit preserve selection when clicking on item that is part of the selection
						selection_mask = (1 << node_clicked);           // Click to single-select
				}
				ImGui::PopStyleVar();

				ImGui::TreePop();
			}
			ImGui::End();
		}

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;
			static float pos[3];
			static float rota[3];
			static float size[3];
			ImGui::Begin("Properties");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Checkbox("Demo Window", &show_demo_window);
			DragInputFloat3("pos ", pos);
			DragInputFloat3("rota", rota);
			DragInputFloat3("size", size);


			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

  
		
	}
	void UI::RenderUI(){
		DrawBorder();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}


	void UI::DrawBorder()
	{
		//glColor3f(1.0f, 1.0f, 1.0f);
		////设置点的大小
		////glPointSize(20);
		////先画出3个点，以好观察。
		//glBegin(GL_POINTS);
		//glVertex3f(1, 1, 0);
		//glVertex3f(-1, -1, 0);
		//glVertex3f(-1, 1, 0);
		//glVertex3f(1, -1, 0);
		//glEnd();

		////画线
		////glLineWidth(10);
		//glBegin(GL_LINES);
		//glVertex3f(1, 1, 0);
		//glVertex3f(1, -1, 0);

		//glVertex3f(1, -1, 0);
		//glVertex3f(-1, -1, 0);

		//glVertex3f(-1, -1, 0);
		//glVertex3f(-1, 1, 0);

		//glVertex3f(-1, 1, 0);
		//glVertex3f(1, 1, 0);
		//glEnd();
		//glFlush();
	}
	void UI::DragInputFloat3(const char* label, float val[])
	{
		ImGui::PushID(label);
		{
			ImGui::Text(label);
			ImGui::SameLine(50);

			ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.15f);
			ImGui::PushID("x");
			ImGui::DragFloat("x", &val[0], 0.005f, -1000, 1000);
			ImGui::PopID();
			ImGui::SameLine();
			ImGui::PushID("y");
			ImGui::DragFloat("y", &val[1], 0.005f, -1000, 1000);
			ImGui::PopID();
			ImGui::SameLine();
			ImGui::PushID("z");
			ImGui::DragFloat("z", &val[2], 0.005f, -1000, 1000);
			ImGui::PopID();
			ImGui::SameLine();
			ImGui::PushID("r");
			if (ImGui::Button("r"))
				val[0] = val[1] = val[2] = 0;
			ImGui::PopID();
			ImGui::PopItemWidth();
		}
		ImGui::PopID();
	}
}
