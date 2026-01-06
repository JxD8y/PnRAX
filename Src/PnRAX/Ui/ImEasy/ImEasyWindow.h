#pragma once
#include "ImEasy.h"
#include <functional>
#include <stdexcept>

#define IMG_DEF_HW ImVec2(800,600)
#define IMG_DEF_XY ImVec2(0,0)

class ImGuiWindow { //Base class
public:

	explicit ImGuiWindow(string title,ImVec2 grid,ImVec2 rec = IMG_DEF_HW);
	ImGuiWindow(ImGuiWindow& cObj) = delete;
	ImGuiWindow(ImGuiWindow&& mObj) = delete;
	virtual ~ImGuiWindow() = default;

	void SetWindowTitle(string title);

	[[nodiscard]] int GetId();
	[[nodiscard]] string GetTitle();

	virtual void Render() = 0;

	bool is_open = true;

	ImVec2 HW;
	ImVec2 ComputedHW;
	ImVec2 ComputedXY;
	ImVec2 Grid; // X: COL Y: ROW
protected:
	int m_id = -1;
	string m_title = "BLANK_TITLE";
};