#include "ImGui_Impl_easy.h"
#include <functional>
#include <stdexcept>

#define IMG_DEF_HW ImVec2(800,600)
#define IMG_DEF_XY ImVec2(0,0)

class ImGuiWindow { //Base class
public:
	//Only provide the inside of the Begin/End in this
	using RenderFunction = std::function<void(ImGuiWindow& imWin)>;

	explicit ImGuiWindow(string title,ImVec2 grid,ImVec2 rec = IMG_DEF_HW);
	ImGuiWindow(ImGuiWindow& cObj) = delete;
	ImGuiWindow(ImGuiWindow&& mObj) = delete;

	void SetRenderingFunction(RenderFunction pRdFunc);
	void SetWindowTitle(string title);

	void Close();

	[[nodiscard]] int GetId();
	[[nodiscard]] string GetTitle();

	void Render();

	ImVec2 HW;
	ImVec2 ComputedHW;
	ImVec2 ComputedXY;
	ImVec2 Grid; // X: COL Y: ROW
private:
	bool is_open = true;
	int m_id = -1;
	string m_title = "BLANK_TITLE";
	RenderFunction pRdFunc = NULL;
};