#pragma once
#include <imgui.h>
#include <iostream>
#include <commdlg.h>
#include "GlobalContext.h"
#include "PnLdr/PnLdr.h"
#include <shlwapi.h>

#pragma comment(lib ,"Shlwapi.lib")

using namespace std;

bool OpenPeFileDialog(string& filePath, HWND hwndOwner = NULL) {
	OPENFILENAME ofn;
	char szFile[260];

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwndOwner;
	ofn.lpstrFile = szFile;

	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "*.exe\0*.EXE\0*.dll\0*.DLL";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE) {
		filePath = ofn.lpstrFile;
		return true;
	}

	return false;
}
void StartDisassembling(string fileName);

namespace TopMenu {

	GlobalContext* g_ctx = NULL;

	void RenderApplicationTopMenu() {
		if (g_ctx) {
			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Open")) {
						string path = "";
						if (!OpenPeFileDialog(path)) {
							Imeasy::ShowMessageBox("Operation Canceled"
								, "No file has been selected", MessageBoxData::Level::Warn, { "Ok"}, NULL);
						}
						else {
							StartDisassembling(path);
						}
						
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Exit")) {
						Imeasy::ShowMessageBox("Exit", "Are you sure to quit?", MessageBoxData::Level::Warn, { "Yes" , "No"}, [](int i) {
							if (i == 0)
								exit(0);
						});
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("View")) {
					ImGui::MenuItem("Decompiler", NULL, &g_ctx->pDecompilerWindow->is_open);
					ImGui::MenuItem("PE Info", NULL, &g_ctx->pPEViewerWindow->is_open);
					ImGui::MenuItem("Import Table", NULL, &g_ctx->pImportTableWindow->is_open);
					ImGui::MenuItem("Export Table", NULL, &g_ctx->pExportTableWindow->is_open);
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
				if (ImGui::BeginMenu("About")) {

					if (ImGui::MenuItem("GitHub")) {
						ShellExecuteA(NULL, "open", "https://github.com/jxd8y", NULL, NULL, SW_SHOWNORMAL);
					}
					if (ImGui::MenuItem("About Me")) {
						Imeasy::ShowMessageBox("About me",
							"Im JxD8y!\nNothing to say now \nobviously im filling this with garbage text!", MessageBoxData::Level::Info, {"Ok"},NULL);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
		}
	}

}
void StartDisassembling(string fileName) {
	GlobalContext* g_ctx = TopMenu::g_ctx;
	TopMenu::g_ctx->targetName = string(PathFindFileNameA(fileName.c_str()));
	auto d = PnLdr::OpenFile(fileName);
	g_ctx->targetData = d;
	((DecompilerWindow*)g_ctx->pDecompilerWindow)->_init_cs();
}