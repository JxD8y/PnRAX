#pragma once
#include <imgui.h>
#include <iostream>
#include <commdlg.h>
#include "GlobalContext.h"
#include "PnLdr/PnLdr.h"
#include "PnRAX.h"

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
	string vStr = "V0.0";
	void RenderApplicationTopMenu() {
		ImGuiIO& io = ImGui::GetIO();
		if (g_ctx) {
			if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_N,false)) {
				string path = "";
				if (!OpenPeFileDialog(path)) {
					Imeasy::ShowMessageBox("Operation Canceled"
						, "No file has been selected", MessageBoxData::Level::Warn, { "Ok" }, NULL);
				}
				else {
					LoadNewFile(path);
				}
			}
			if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_X)) {
				Imeasy::ShowMessageBox("Exit", "Are you sure to quit?", MessageBoxData::Level::Warn, { "Yes" , "No"}, [](int i) {
					if (i == 0)
						exit(0);
				});
			}
			if (ImGui::BeginMainMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("Open","Ctrl+N")) {
						string path = "";
						if (!OpenPeFileDialog(path)) {
							Imeasy::ShowMessageBox("Operation Canceled"
								, "No file has been selected", MessageBoxData::Level::Warn, { "Ok"}, NULL);
						}
						else {
							LoadNewFile(path);
						}
						
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Exit","Ctrl+X")) {
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
				if (ImGui::BeginMenu("Help")) {
					if (ImGui::MenuItem("About PnRAX")) {
						Imeasy::ShowMessageBox(string("About PnRAX ") + vStr,
							"PnRAX is a open-source disassembler and PE viewer\naimed to be simple and portable\n(Some AI features will be add in future updates)\n\nCopyrigh 2026 JxD8y.\nThis application is licensed under the Apache License, Version 2.0\nThis software is provided on an 'as is' basis, without warranties\nor conditions of any kind, either express or implied"
							, MessageBoxData::Level::Info, { "Open Project repo.","Close" }, [](int i) {
								if (i == 0) {
									ShellExecuteA(NULL, "open", "https://github.com/jxd8y/pnrax", NULL, NULL, SW_SHOWNORMAL);
								}
							});
					}
					if (ImGui::MenuItem("About Developer")) {
						Imeasy::ShowMessageBox(string("About Me"),
							"Hello im JxD8y\n\nEverything you should know about me is in my Github\n\n\t\tThank you for choosing this software"
							, MessageBoxData::Level::Info, { "Open Github","Close" }, [](int i) {
								if (i == 0) {
									ShellExecuteA(NULL, "open", "https://github.com/jxd8y", NULL, NULL, SW_SHOWNORMAL);
								}
							});
					}
					ImGui::EndMenu();
				}
				ImGui::EndMainMenuBar();
			}
		}
	}

}
void StartDisassembling(string fileName) {
	
}