#include <iostream>
#include "UI/ImGui/ImGuiWindow.h"
#include "UI/ImGui/ImGuiWindowManager.h"

int main(int argc, char* argv[]) {
    ImGuiWindow* win1 = new ImGuiWindow("Win1");
    win1->SetRenderingFunction(std::function<void(ImGuiWindow&)>([](ImGuiWindow& iWin) {
        ImGui::Text("Test text from 1 window");
    }));
    ImGuiWindow* win2 = new ImGuiWindow("Win2");
    win1->SetRenderingFunction(std::function<void(ImGuiWindow&)>([](ImGuiWindow& iWin) {
        ImGui::Text("Test text from 2 window");
        }));
    ImGuiWindow* win3 = new ImGuiWindow("Win3");
    win1->SetRenderingFunction(std::function<void(ImGuiWindow&)>([](ImGuiWindow& iWin) {
        ImGui::Text("Test text from 3 window");
        }));
    ImGuiWindowManager mgr("The main", ImVec2(800, 600));
    mgr.AddWindow(win1);
    mgr.AddWindow(win2);
    mgr.AddWindow(win3);
    mgr.StartRendering();
    return 0;
}