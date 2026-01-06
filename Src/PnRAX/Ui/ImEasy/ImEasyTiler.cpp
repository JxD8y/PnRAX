#include "ImEasy.h"
#include "ImEasyWindow.h"
#include <algorithm>

void Imeasy::__imeasy_internal::CalculateTileWindowCord(float containerWidth, float containerHeight, float curX, float curY, float padding) {
	std::map<int, float> colBaseWidths;
	std::map<int, float> colTotalHeights;

	for (ImEasyWindow* win : vWindow) {
		if (!win->is_open)
			continue;
		if (win->HW.x > colBaseWidths[win->Grid.x])
			colBaseWidths[win->Grid.x] = win->HW.x;
		if (colTotalHeights.find(win->Grid.x) == colTotalHeights.end()) {
			colTotalHeights[win->Grid.x] = 0.0f;
		}
		colTotalHeights[win->Grid.x] += win->HW.y;
	}
	colTotalHeights.clear();
	std::map<int, int> colItemCounts;
	for (ImEasyWindow* win : vWindow) {
		if (!win->is_open)
			continue;
		if (win->HW.x > colBaseWidths[win->Grid.x]) colBaseWidths[win->Grid.x] = win->HW.x;
		colTotalHeights[win->Grid.x] += win->HW.y;
		colItemCounts[win->Grid.x]++;
	}
	for (auto& p : colTotalHeights) {
		int count = colItemCounts[p.first];
		if (count > 1) p.second += (count - 1) * padding;
	}
	float totalBaseW = 0.0f;
	for (auto const& p : colBaseWidths) totalBaseW += p.second;
	float totalPaddingX = (colBaseWidths.size() > 0) ? (colBaseWidths.size() - 1) * padding : 0.0f;

	float maxStackH = 0.0f;
	for (auto const& p : colTotalHeights) {
		if (p.second > maxStackH) maxStackH = p.second;
	}
	float scaleX = (totalBaseW > 0) ? (containerWidth - totalPaddingX) / totalBaseW : 1.0f;
	float scaleY = (maxStackH > 0) ? (containerHeight) / maxStackH : 1.0f;

	std::map<int, float> finalColWidths;
	for (auto const& p : colBaseWidths) finalColWidths[p.first] = p.second * scaleX;

	std::vector<ImEasyWindow*> cvWindow = vWindow;
	std::sort(cvWindow.begin(), cvWindow.end(), [](ImEasyWindow* a, ImEasyWindow* b) {
		if(!a->is_open)
			return false;
		if (a->Grid.y != b->Grid.y) return a->Grid.y < b->Grid.y;
		return a->Grid.x < b->Grid.x;
		});
	std::map<int, float> colStackY;

	for (ImEasyWindow* win : cvWindow) {
		if (!win->is_open)
			continue;
		float finalX = curX;
		for (auto const& p : finalColWidths) {
			if (p.first < win->Grid.x) {
				finalX += p.second + padding;
			}
		}
		if (colStackY.find(win->Grid.x) == colStackY.end()) {
			colStackY[win->Grid.x] = curY;
		}

		win->ComputedXY.x = finalX;
		win->ComputedXY.y = colStackY[win->Grid.x];

		win->ComputedHW.x = finalColWidths[win->Grid.x];
		win->ComputedHW.y = win->HW.y * scaleY;

		colStackY[win->Grid.x] += win->ComputedHW.y + padding;
	}
}

void Imeasy::__imeasy_internal::RenderTileWindows() {
	for (ImEasyWindow* img : vWindow) {
		if (img->is_open) {
			ImGui::SetNextWindowPos(img->ComputedXY, ImGuiCond_Always);
			ImGui::SetNextWindowSize(img->ComputedHW, ImGuiCond_Always);

			ImGuiWindowFlags flags =
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoCollapse |
				ImGuiWindowFlags_NoSavedSettings;

			img->Render();
		}
	}
}

