#include "ImGui_Impl_easy.h"
#include <functional>
#include <stdexcept>

#define IMG_DEF_HW ImVec2(800,600)
#define IMG_DEF_XY ImVec2(0,0)

class ImGuiWindow { //Base class
public:
	using RenderFunction = std::function<void(ImGuiWindow& imWin)>

	friend class ImGuiWindowManager;
	explicit ImGuiWindow(string title);
	ImGuiWindow(ImGuiWindow& cObj) = delete;
	ImGuiWindow(ImGuiWindow&& mObj) = delete;

	void SetRenderingFunction(RenderFunction pRdFunc);
	void SetWindowTitle(string title);
	void SetWindowRect(ImVec2 hw);
	void SetLWindowPos(ImVec2 xy);
	
	void Close();
	[[nodiscard]] int GetId();
	[[nodiscard]] string GetTitle();

private:
	bool is_open = true;
	int m_id = -1;
	string m_title = "BLANK_TITLE";
	RenderFunction pRdFunc = NULL;
	ImVec2 m_hw;
	ImVec2 m_xy;

	void Render();
};