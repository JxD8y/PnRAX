#include <ImGui_Impl_easy.h>

typedef void(*RenderingFunction)();
class ImGuiWindow {
public:
	ImGuiWindow(ImGuiWindow& cObj) = delete;
	ImGuiWindow(ImGuiWindow&& mObj) = delete;

	void SetRenderingFunction(RenderingFunction pRdFunc);
	void SetWindowTitle(string title);
	void SetWindowRect(ImVec2 hw);//size
	void SetLWindowPos(ImVec2 xy);

	[[nodiscard]] int GetId();
	[[nodiscard]] string GetTitle();
	[[nodiscard]] ImVec2 GetSizeVec();
	[[nodiscard]] ImVec2 GetLPosVec();
private:
	int id = -1;
		
};