#include "PEViewer.h"

PEViewerWindow::PEViewerWindow(GlobalContext& ctx, ImVec2 grid, ImVec2 rec)
    :ImEasyWindow("PE Viewer: ", grid), m_gctx(ctx)
{
    this->HW = rec;
    this->SetWindowTitle(fmt::format("PE Viewer: {}##{}", this->m_gctx.targetName, this->m_id));
}

[[nodiscard]] string PEViewerWindow::GetTitle()
{
    return fmt::format("PE Viewer: {}##{}", this->m_gctx.targetName, this->m_id);
}

void PEViewerWindow::Render() {
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.13f, 0.40f, 0.28f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.18f, 0.55f, 0.34f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(0.20f, 0.50f, 0.35f, 1.0f));
    ImGui::Begin(this->GetTitle().c_str(), NULL, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    ImGui::PopStyleColor(3);
    if (this->m_gctx.targetData == NULL) {
        ImGui::Text("No PE loaded.");
        ImGui::End();
        return;
    }

    ImGui::Text(this->m_gctx.targetData->is_64  ? "Machine: x86-x64" : "Machine: x86-x32");
    ImGui::Text("TimeStamp: %d", this->m_gctx.targetData->ntHeader.FileHeader.TimeDateStamp);
    ImGui::Text(this->m_gctx.targetData->ntHeader.FileHeader.Characteristics == IMAGE_FILE_DLL ? "Type: DLL" : "Type: PE");
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 5));

    ImGui::Text("ImageBase: 0x%X", this->m_gctx.targetData->optionalHeader.ImageBase);

    ImGui::Text("SubSystem: ");
    ImGui::SameLine();
    if(this->m_gctx.targetData->optionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_CUI)
        ImGui::Text("Console");
    else if (this->m_gctx.targetData->optionalHeader.Subsystem == IMAGE_SUBSYSTEM_WINDOWS_GUI)
        ImGui::Text("Gui");
	else
        ImGui::Text("Native");

	ImGui::Text("EntryPoint: 0x%X", this->m_gctx.targetData->optionalHeader.AddressOfEntryPoint);
    ImGui::Separator();
    ImGui::Dummy(ImVec2(0, 5));

	if (this->m_gctx.targetData->optionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size > 0)
        ImGui::Text("Reloc: yes");
    else
        ImGui::Text("Reloc: no");
    
    //Ignored the Resources for now should create a seperate window for them

    if (this->m_gctx.targetData->optionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size > 0)
        ImGui::Text("TLS Callback: found");
    else
        ImGui::Text("TLS Callback: not-found");

    ImGui::Dummy(ImVec2(0, 5));
    ImGui::Text("Sections: ");
    if (ImGui::BeginTable("PESECTIONTABLE", 3, ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY |ImGuiTableFlags_ScrollX)) {
        ImGui::TableSetupColumn("Section Name", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableSetupColumn("Perm.", ImGuiTableColumnFlags_WidthFixed, 70.0f);
        ImGui::TableSetupColumn("RVA", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        for (auto sec : this->m_gctx.targetData->sectionHeaders) {
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%s", sec.Name);

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%s%s%s",
                (sec.Characteristics & IMAGE_SCN_MEM_READ) ? "R" : "",
                (sec.Characteristics & IMAGE_SCN_MEM_WRITE) ? "W" : "",
                (sec.Characteristics & IMAGE_SCN_MEM_EXECUTE) ? "X" : ""
			);

            ImGui::TableSetColumnIndex(2);
			ImGui::Text("0x%X", sec.VirtualAddress);

        }

        ImGui::EndTable();
    }
    
    ImGui::End();
}