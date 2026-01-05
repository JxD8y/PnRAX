#pragma once
#include <imgui.h>

void RenderApplicationTopMenu() {
	if (ImGui::BeginMainMenuBar()) {
		if(ImGui::BeginMenu("File")) {
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
		ImGui::EndMainMenuBar();
	}
}