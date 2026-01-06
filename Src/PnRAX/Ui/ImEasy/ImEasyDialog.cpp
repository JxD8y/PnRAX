#include "ImEasy.h"

MessageBoxData Imeasy::__imeasy_internal::g_MsgBox;

void Imeasy::ShowMessageBox(const std::string& title, const std::string& message,
    MessageBoxData::Level lv,
	const std::vector<std::string>& buttons,
	std::function<void(int)> callback) {

    __imeasy_internal::g_MsgBox.level = lv;
    __imeasy_internal::g_MsgBox.title = title;
    __imeasy_internal::g_MsgBox.message = message;
    __imeasy_internal::g_MsgBox.buttons = buttons;
    __imeasy_internal::g_MsgBox.callback = callback;
    __imeasy_internal::g_MsgBox.isOpen = true;

}
void Imeasy::__imeasy_internal::RenderMessageBox() {
    if (g_MsgBox.isOpen) {
        ImGui::OpenPopup(g_MsgBox.title.c_str());
        g_MsgBox.isOpen = false;
    }

    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, DialogCornerRadius);

    switch (g_MsgBox.level) {
        case MessageBoxData::Level::Info:
            ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ColorBlueInfo);
            break;
        case MessageBoxData::Level::Warn:
            ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ColorOrangeWarn);
            break;
        case MessageBoxData::Level::Error:
            ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ColorRedError);
            break;
        case MessageBoxData::Level::Success:
            ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ColorGreenSucc);
            break;
        default:
            break;
    }
    
    if (ImGui::BeginPopupModal(g_MsgBox.title.c_str(), nullptr,ImGuiWindowFlags_AlwaysAutoResize| ImGuiWindowFlags_NoMove)) {
        ImGui::Dummy(ImVec2(0, Imeasy::height / 10));

        ImGui::SetWindowFontScale(1.5f);
        ImGui::Text("%s", g_MsgBox.message.c_str());
        ImGui::SetWindowFontScale(1);

        ImGui::Dummy(ImVec2(Imeasy::width/2, Imeasy::height / 10));

        ImGui::SetWindowFontScale(1.1f);
        ImGui::PushStyleColor(ImGuiCol_Button, ColorAccent);

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ElementCornerRadius);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(15, 5));

        for (int i = 0; i < g_MsgBox.buttons.size(); i++) {
            if (i > 0) ImGui::SameLine();


            if (ImGui::Button(g_MsgBox.buttons[i].c_str())) {
                if (g_MsgBox.callback) g_MsgBox.callback(i);
                ImGui::CloseCurrentPopup();
            }

        }
        ImGui::PopStyleColor();
        ImGui::SetWindowFontScale(1);
        ImGui::PopStyleVar(2);
        ImGui::EndPopup();
    }
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}