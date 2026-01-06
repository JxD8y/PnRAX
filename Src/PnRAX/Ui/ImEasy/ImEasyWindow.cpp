#include "ImEasyWindow.h"

ImEasyWindow::ImEasyWindow(string title,ImVec2 grid, ImVec2 rec){
	this->m_title = title;
	this->Grid = grid;
	this->HW = rec;
	this->m_id = rand() % 10000;
}

void ImEasyWindow::SetWindowTitle(string title) {
	this->m_title = title;
}
[[nodiscard]] int ImEasyWindow::GetId() {
	return this->m_id;
}
[[nodiscard]] string ImEasyWindow::GetTitle() {
	return this->m_title;
}