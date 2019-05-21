#pragma once

namespace CPM::Editor
{
	class UI {
	public:
		static void UpdateUI();
		static void RenderUI();

	private:
		static void DrawBorder();
		static void DragInputFloat3(const char* label, float val[]);
	};
	
}