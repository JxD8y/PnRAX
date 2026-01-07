#pragma once
#include <fmt/core.h>

#include "ImEasy/ImEasyWindow.h"
#include "GlobalContext.h"
#include <capstone/capstone.h>

#define ColorAddress ImVec4(0.6f, 0.6f, 0.6f, 1.0f) 
#define ColorMnemonicDark ImVec4(1.0f, 1.0f, 1.0f, 1.0f)
#define ColorMnemonicLight ImVec4(0.0f, 0.0f, 0.0f, 1.0f)
#define ColorBytes ImVec4(0.45f, 0.45f, 0.45f, 1.0f)

#define ColorReg ImVec4(0.6f, 0.9f, 0.6f, 1.0f)
#define ColorImm ImVec4(0.3f, 0.8f, 0.95f, 1.0f)
#define ColorMem ImVec4(1.0f, 0.65f, 0.2f, 1.0f)
#define ColorMemSize ImVec4(0.9f, 0.4f, 0.8f, 1.0f)

#define ColorDB IM_COL32(220,0,0, 255) 

class DecompilerWindow : public ImEasyWindow
{
public:
	explicit DecompilerWindow(GlobalContext& ctx,ImVec2 grid, ImVec2 rec = IMG_DEF_HW);
	DecompilerWindow(DecompilerWindow& cObj) = delete;
	DecompilerWindow(DecompilerWindow&& mObj) = delete;

	~DecompilerWindow();
	
	[[nodiscard]] string GetTitle() override;

	void Render() override;
	
	void _init_cs();
private:

	csh _cs_hnd;
	vector<uint8_t> _decomp_offset;
	GlobalContext& m_gctx;
};