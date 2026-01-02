#include "ImGui/ImGuiWindow.h"
#include "GlobalContext.h"
#include <fmt/core.h>

class DecompilerWindow : public ImGuiWindow
{
public:
	explicit DecompilerWindow(GlobalContext& ctx,ImVec2 grid);
	DecompilerWindow(DecompilerWindow& cObj) = delete;
	DecompilerWindow(DecompilerWindow&& mObj) = delete;

	void Render() override;
private:
	GlobalContext& m_gctx;
};