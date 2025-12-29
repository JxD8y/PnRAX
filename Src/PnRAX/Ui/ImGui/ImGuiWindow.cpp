#include "ImGuiWindow.h"

ImGuiWindow::ImGuiWindow(string t){
	this->m_title = t;
	this->m_id = rand() % 10000; //Probably Uniqueness is ensured
	this->m_hw = IMG_DEF_HW;
	this->m_xy = IMG_DEF_XY;
}

void ImGuiWindow::SetRenderingFunction(RenderFunction pFunc) {
	if (pFunc == NULL)
		throw std::invalid_argument("pFunc was null");
	this->pRdFunc = pFunc;
}
void ImGuiWindow::SetWindowTitle(string title) {
	this->m_title = title;
}
void ImGuiWindow::SetWindowRect(ImVec2 hw) {
	this->m_hw = hw;
}
void ImGuiWindow::SetLWindowPos(ImVec2 xy) {
	this->m_xy = xy;
}
[[nodiscard]] int ImGuiWindow::GetId() {
	return this->m_id;
}
[[nodiscard]] string ImGuiWindow::GetTitle() {
	return this->m_title;
}
[[nodiscard]] ImVec2 ImGuiWindow::GetSize() {
	return this->m_hw;
}
[[nodiscard]] ImVec2 ImGuiWindow::GetPos() {
	return this->m_xy;
}

void ImGuiWindow::Render() {
	if (this->pRdFunc != NULL) {
		this->pRdFunc(*this);
	}
}

void ImGuiWindow::Close() {
	this->is_open = false;
}