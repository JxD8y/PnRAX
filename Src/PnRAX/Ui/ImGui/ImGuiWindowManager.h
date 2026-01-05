#pragma once
#include "ImGui_Impl_easy.h"
#include <functional>
#include <stdexcept>
#include <algorithm>

using namespace std;

/*
* This class will only render one win32 window and
* renders the ImGuiWindow that registered to it
* inside that win32 window in a tiled style.
*/
class ImGuiWindow;

#define DEF_BG_COL ImColor(0,0,0,0)

class ImGuiWindowManager {
public: 
	using OnCloseHandler = function<void()>;
	using TopMenuHandler = function<void()>;

	explicit ImGuiWindowManager(string title, ImVec2 perfDim);
	ImGuiWindowManager(const ImGuiWindowManager&) = delete;
	ImGuiWindowManager(const ImGuiWindowManager&&) = delete;

	void SetTopMenu(TopMenuHandler tpMenu);
	void SetOnCloseHandler(OnCloseHandler onClose);
	void AddWindow(ImGuiWindow* window);

	void StartRendering();

private:
	string m_title;
	vector<ImGuiWindow*> m_vWindow;
	ImVec2 m_pDim;
	ImColor m_bgColor;
	OnCloseHandler m_onClosed;
	OnCloseHandler m_topMenu;
};