#pragma once
#include <imgui.h>
#include "GlobalContext.h"

namespace TopMenu {

	GlobalContext* g_ctx = NULL;

	void RenderApplicationTopMenu() {
		if (g_ctx) {
			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Open")) {

					}
					ImGui::Separator();
					if (ImGui::MenuItem("Exit")) {

					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("View")) {
					static bool show_disasm = true;
					if (ImGui::MenuItem("Disassembler", NULL, &show_disasm)) {

					}
					if (ImGui::MenuItem("COFF Info", NULL, &show_disasm)) {

					}
					if (ImGui::MenuItem("Import Table", NULL, &show_disasm)) {

					}
					if (ImGui::MenuItem("Export Table", NULL, &show_disasm)) {

					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Preferences")) {

					if (ImGui::MenuItem("Dark Mode", NULL, &g_ctx->isDark)) {
						ImGuiStyle& style = ImGui::GetStyle();
						if (g_ctx->isDark) {
							ImGui::StyleColorsDark(&style);
						}
						else {
							ImGui::StyleColorsLight(&style);
						}
					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
		}
	}
}