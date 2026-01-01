#include <iostream>
#include "UI/ImGui/ImGuiWindow.h"
#include "UI/ImGui/ImGuiWindowManager.h"

int main(int argc, char* argv[]) {
    ImGuiWindow* win1 = new ImGuiWindow("Decompiler - App.exe:(.text+0xffffff)",ImVec2(0,0),ImVec2(400,600));
    win1->SetRenderingFunction(std::function<void(ImGuiWindow&)>([](ImGuiWindow& iWin) {
        ImGui::Text("Test text from 1 window");
    }));
    ImGuiWindow* win2 = new ImGuiWindow("COFF browser - App.exe", ImVec2(1, 1), ImVec2(400, 300));
    win2->SetRenderingFunction(std::function<void(ImGuiWindow&)>([](ImGuiWindow& iWin) {
        ImGui::Text("Test text from 2 window");
        }));
    ImGuiWindow* win3 = new ImGuiWindow("Import Table - App.exe", ImVec2(1, 2), ImVec2(400, 300));
    win3->SetRenderingFunction(std::function<void(ImGuiWindow&)>([](ImGuiWindow& iWin) {
        ImGui::Text("Test text from 3 window");
        }));
    ImGuiWindowManager mgr("The main", ImVec2(850, 650));
    mgr.AddWindow(win1);
    mgr.AddWindow(win2);
    mgr.AddWindow(win3);
    mgr.StartRendering();
    return 0;
}