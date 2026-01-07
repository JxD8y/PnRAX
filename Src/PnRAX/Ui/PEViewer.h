#include <fmt/core.h>

#include "ImEasy/ImEasyWindow.h"
#include "GlobalContext.h"

class PEViewerWindow : public ImEasyWindow
{
public:
	explicit PEViewerWindow(GlobalContext& ctx, ImVec2 grid, ImVec2 rec = IMG_DEF_HW);
	PEViewerWindow(PEViewerWindow& cObj) = delete;
	PEViewerWindow(PEViewerWindow&& mObj) = delete;

	[[nodiscard]] string GetTitle() override;

	void Render() override;

private:
	GlobalContext& m_gctx;
};