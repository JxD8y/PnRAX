#include "IETableWindow.h"

IETableWindow::IETableWindow(GlobalContext& ctx, ImVec2 grid, bool imp,ImVec2 rec)
    :ImEasyWindow("PE Viewer - ", grid), m_gctx(ctx),_import(imp)
{
    this->HW = rec;
    if(this->_import)
        this->SetWindowTitle(fmt::format("Import Table of: {}##{}", this->m_gctx.targetName, this->m_id));
    else
        this->SetWindowTitle(fmt::format("Export Table of: {}##{}", this->m_gctx.targetName, this->m_id));
}

[[nodiscard]] string IETableWindow::GetTitle()
{
    if (this->_import)
        return fmt::format("Import Table of: {}##{}", this->m_gctx.targetName, this->m_id);
    else
        return fmt::format("Export Table of: {}##{}", this->m_gctx.targetName, this->m_id);
}


void IETableWindow::Render() {
    if (this->_import) {
        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.10f, 0.25f, 0.50f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.15f, 0.40f, 0.80f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.20f, 0.45f, 0.85f, 1.0f));
    
        ImGui::Begin(this->GetTitle().c_str(), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        ImGui::PopStyleColor(3);
        if (this->m_gctx.targetData != NULL && this->m_gctx.targetData->importTable.size() > 0) {
            if (ImGui::BeginTable("PEIMPTBL", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX)) {
                ImGui::TableSetupColumn("Library", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Function", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();

                for (auto p : this->m_gctx.targetData->importTable) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text(p.first.c_str());
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(p.second.c_str());
                }
                ImGui::EndTable();
            }
        }
        else {
            ImGui::Text("No import table found.");
        }

        ImGui::End();
    }
    else {
        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.50f, 0.20f, 0.80f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.50f, 0.20f, 0.80f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.55f, 0.25f, 0.85f, 1.0f));
        ImGui::Begin(this->GetTitle().c_str(), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
        
        ImGui::PopStyleColor(3);
        if (this->m_gctx.targetData != NULL && this->m_gctx.targetData->exportTable.size() > 0) {

            if (ImGui::BeginTable("PEEXPTBL", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY | ImGuiTableFlags_ScrollX | ImGuiTableFlags_ScrollX)) {
                ImGui::TableSetupColumn("Function", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("RVA", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();

                for (auto p : this->m_gctx.targetData->exportTable) {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::Text("0x%" "llx", p.first);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text(p.second.c_str());
                }

                ImGui::EndTable();
            }
        }
        else {
            ImGui::Text("No Export table found.");
        }

        ImGui::End();
    }
}