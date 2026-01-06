#pragma once
#include "ImEasy.h"

#include <functional>
#include <stdexcept>
#include <algorithm>

using namespace std;

class ImEasyWindow;

#define DEF_BG_COL ImColor(0,0,0,0)

class ImEasyWindowManager {
public: 
	using OnCloseHandler = function<void()>;
	using TopMenuHandler = function<void()>;

	explicit ImEasyWindowManager(string title, ImVec2 perfDim);
	ImEasyWindowManager(const ImEasyWindowManager&) = delete;
	ImEasyWindowManager(const ImEasyWindowManager&&) = delete;

	void SetTopMenu(TopMenuHandler tpMenu);
	void SetOnCloseHandler(OnCloseHandler onClose);
	void AddWindow(ImEasyWindow* window);

	void StartRendering();

private:
	string m_title;
	vector<ImEasyWindow*> m_vWindow;
	ImVec2 m_pDim;
	ImColor m_bgColor;
	OnCloseHandler m_onClosed;
	OnCloseHandler m_topMenu;
};