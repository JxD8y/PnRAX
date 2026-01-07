#include <fmt/core.h>

#include "ImEasy/ImEasyWindow.h"
#include "GlobalContext.h"

class IETableWindow : public ImEasyWindow
{
public:
	explicit IETableWindow(GlobalContext& ctx, ImVec2 grid, bool _import ,ImVec2 rec = IMG_DEF_HW);
	IETableWindow(IETableWindow& cObj) = delete;
	IETableWindow(IETableWindow&& mObj) = delete;

	[[nodiscard]] string GetTitle() override;

	void Render() override;

private:
	bool _import = true;
	GlobalContext& m_gctx;
};