#include "ImEasyWindowManager.h"
#include "ImEasyWindow.h"

ImGuiWindowManager::ImGuiWindowManager(string title, ImVec2 perDim): m_title(title),m_pDim(perDim)  {
	this->m_bgColor = DEF_BG_COL;
}
void ImGuiWindowManager::SetOnCloseHandler(OnCloseHandler onClose) {
	if(onClose == NULL){
		throw invalid_argument("onClose was null");
	}
	this->m_onClosed = onClose;
}
void ImGuiWindowManager::AddWindow(ImGuiWindow* window) {
	if (!any_of(this->m_vWindow.begin(), this->m_vWindow.end(), [window](ImGuiWindow* win) {return win->GetId() == window->GetId(); })) {
		this->m_vWindow.push_back(window);
	}
}
void ImGuiWindowManager::SetTopMenu(TopMenuHandler tpMenu) {
	this->m_topMenu = tpMenu;
}
void ImGuiWindowManager::StartRendering() {
	Imeasy::SetTopMenu(this->m_topMenu);
	Imeasy::SetRenderingList(this->m_vWindow);
	Imeasy::StartRendering(this->m_title, this->m_pDim.y, this->m_pDim.x);
}