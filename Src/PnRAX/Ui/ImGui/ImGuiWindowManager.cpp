#include "ImGuiWindowManager.h"
#include "ImGuiWindow.h"

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
void ImGuiWindowManager::closeWindow(int id) {
	auto pWin = find_if(this->m_vWindow.begin(), this->m_vWindow.end(), [id](ImGuiWindow* win) {return win->GetId() == id; });
	if (pWin != this->m_vWindow.end()) {
		(*pWin)->Close();
	}
}
void ImGuiWindowManager::closeWindow(ImGuiWindow* win) {
	if (win)
		win->Close();
}

void ImGuiWindowManager::StartRendering() {
	Imeasy::SetRenderingList(this->m_vWindow);
	Imeasy::StartRendering(this->m_title, this->m_pDim.y, this->m_pDim.x);
}