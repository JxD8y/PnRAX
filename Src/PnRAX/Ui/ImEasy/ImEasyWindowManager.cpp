#include "ImEasyWindowManager.h"
#include "ImEasyWindow.h"

ImEasyWindowManager::ImEasyWindowManager(string title, ImVec2 perDim): m_title(title),m_pDim(perDim)  {
	this->m_bgColor = DEF_BG_COL;
}
void ImEasyWindowManager::SetOnCloseHandler(OnCloseHandler onClose) {
	if(onClose == NULL){
		throw invalid_argument("onClose was null");
	}
	this->m_onClosed = onClose;
}
void ImEasyWindowManager::AddWindow(ImEasyWindow* window) {
	if (!any_of(this->m_vWindow.begin(), this->m_vWindow.end(), [window](ImEasyWindow* win) {return win->GetId() == window->GetId(); })) {
		this->m_vWindow.push_back(window);
	}
}
void ImEasyWindowManager::SetTopMenu(TopMenuHandler tpMenu) {
	this->m_topMenu = tpMenu;
}
void ImEasyWindowManager::StartRendering() {
	Imeasy::SetTopMenu(this->m_topMenu);
	Imeasy::SetRenderingList(this->m_vWindow);
	Imeasy::StartRendering(this->m_title, this->m_pDim.y, this->m_pDim.x,this->hIcon,this->hIconSmall);
}
void ImEasyWindowManager::SetWin32Icons(HICON icon, HICON smallIco) {
	this->hIcon = icon;
	this->hIconSmall = smallIco;
}