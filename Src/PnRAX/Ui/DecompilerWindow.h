#include <fmt/core.h>

#include "ImGui/ImGuiWindow.h"
#include "GlobalContext.h"
#include <capstone/capstone.h>

#define ColorAddress IM_COL32(150, 150, 150, 255) // Grey
#define ColorMnemonic IM_COL32(255, 255, 0, 255)  // Yellow
#define ColorBytes IM_COL32(100, 255, 100, 255) //Green

#define ColorDB IM_COL32(220,0,0, 255) //Red 

class DecompilerWindow : public ImGuiWindow
{
public:
	explicit DecompilerWindow(GlobalContext& ctx,ImVec2 grid);
	DecompilerWindow(DecompilerWindow& cObj) = delete;
	DecompilerWindow(DecompilerWindow&& mObj) = delete;

	~DecompilerWindow();
	
	void Render() override;

	void _init_cs();
private:

	csh _cs_hnd;
	vector<uint8_t> _decomp_offset;
	GlobalContext& m_gctx;
};