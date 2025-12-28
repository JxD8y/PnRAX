#include <ImGui_Impl_easy.h>
#include <stdexcept>

#define IMG_DEF_HW ImVec2(800,600)
#define IMG_DEF_XY ImVec2(0,0)

typedef void(*RenderingFunction)(ImGuiWindow* imWin);

class ImGuiWindow {
public:
	friend class ImGuiWindowManager;
	explicit ImGuiWindow(string title);
	ImGuiWindow(ImGuiWindow& cObj) = delete;
	ImGuiWindow(ImGuiWindow&& mObj) = delete;

	void SetRenderingFunction(RenderingFunction pRdFunc);
	void SetWindowTitle(string title);
	void SetWindowRect(ImVec2 hw);//size
	void SetLWindowPos(ImVec2 xy);

	[[nodiscard]] int GetId();
	[[nodiscard]] string GetTitle();

private:
	int m_id = -1;
	string m_title = "BLANK_TITLE";
	RenderingFunction pRdFunc = NULL;
	ImVec2 m_hw;
	ImVec2 m_xy;

	void Render(); //Should pass a CTX variable to the window maybe contain a instance of the winmgr to help window control other windows and its own pos
};