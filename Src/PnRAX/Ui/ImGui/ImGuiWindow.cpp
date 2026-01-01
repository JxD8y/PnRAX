#include "ImGuiWindow.h"

ImGuiWindow::ImGuiWindow(string title,ImVec2 grid, ImVec2 rec){
	this->m_title = title;
	this->Grid = grid;
	this->HW = rec;
	this->m_id = rand() % 10000;
}

void ImGuiWindow::SetRenderingFunction(RenderFunction pFunc) {
	if (pFunc == NULL)
		throw std::invalid_argument("pFunc was null");
	this->pRdFunc = pFunc;
}
void ImGuiWindow::SetWindowTitle(string title) {
	this->m_title = title;
}
[[nodiscard]] int ImGuiWindow::GetId() {
	return this->m_id;
}
[[nodiscard]] string ImGuiWindow::GetTitle() {
	return this->m_title;
}

void ImGuiWindow::Render() {
	if (this->pRdFunc != NULL) {
		this->pRdFunc(*this);
	}
}

void ImGuiWindow::Close() {
	this->is_open = false;
}