#include "DecompilerWindow.h"

DecompilerWindow::DecompilerWindow(GlobalContext& ctx,ImVec2 grid)
:ImGuiWindow("Decompiler - ",grid), m_gctx(ctx)
{
	this->SetWindowTitle(fmt::format("Decompiler - {}##{}", this->m_gctx.targetName, this->m_id));
}

DecompilerWindow::~DecompilerWindow() {
    cs_close(&this->_cs_hnd);
}
void RenderBytes(const uint8_t* bytes, uint16_t size) {
    // Render up to 8 bytes per instruction to keep it clean
    // You can increase this limit if you want
    char buffer[32];
    char* p = buffer;
    for (int i = 0; i < size && i < 8; i++) {
        p += sprintf(p, "%02X ", bytes[i]);
    }
    ImGui::TextUnformatted(buffer);
}
void DecompilerWindow::_init_cs() {
    if (cs_open(CS_ARCH_X86, CS_MODE_64, &this->_cs_hnd) != CS_ERR_OK) {
        throw runtime_error("Cannot initiate capstone");
    }
    cs_option(this->_cs_hnd, CS_OPT_DETAIL, CS_OPT_OFF);

    if (this->m_gctx.targetData) {
        uint64_t addr = 0;
        size_t sz = this->m_gctx.targetData->textSectionSize;
		const uint8_t* code = reinterpret_cast<const uint8_t*>(this->m_gctx.targetData->pTextSection);
        cs_insn* pIns = cs_malloc(this->_cs_hnd);
        while (sz > 0) {
            if (cs_disasm_iter(this->_cs_hnd, &code, &sz, &addr, pIns)) {
                this->_decomp_offset.push_back(pIns->address);
            }
            else {
                sz--;
                code++;
                addr++;
            }
        }
        cs_free(pIns, 1);
    }
}
void DecompilerWindow::Render() {
	ImGui::Begin(this->m_title.c_str(), &this->is_open);
    int columns = 3;

    ImGuiTableFlags flags =  ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY;
    if (ImGui::BeginTable("DisassemblyTable", columns, flags)) { 
        ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Bytes", ImGuiTableColumnFlags_WidthFixed, 100.0f);
        ImGui::TableSetupColumn("Assembly", ImGuiTableColumnFlags_WidthStretch);
        ImGui::TableHeadersRow();

        ImGuiListClipper clipper;
        clipper.Begin((int)this->_decomp_offset.size());
        while (clipper.Step()) {
            for (int i = clipper.DisplayStart; i < clipper.DisplayEnd; i++) {
                size_t offset = this->_decomp_offset[i];

                const uint8_t* code_ptr = (uint8_t*)this->m_gctx.targetData->pTextSection + offset;
                size_t code_len = this->m_gctx.targetData->textSectionSize - offset; 
                uint64_t address = this->_decomp_offset[0] + offset; 
                cs_insn* pIns = cs_malloc(this->_cs_hnd);

                ImGui::TableNextRow();
                if (cs_disasm_iter(this->_cs_hnd, &code_ptr, &code_len, &address, pIns)) {

                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushStyleColor(ImGuiCol_Text, ColorAddress);
                    ImGui::Text("0x%" PRIx64, pIns->address);
                    ImGui::PopStyleColor();

                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushStyleColor(ImGuiCol_Text, ColorBytes);
                    RenderBytes(pIns->bytes, pIns->size);
                    ImGui::PopStyleColor();

                    int col_idx = 2;
                    ImGui::TableSetColumnIndex(col_idx);
                    ImGui::PushStyleColor(ImGuiCol_Text, ColorMnemonic);
                    ImGui::Text("%s ", pIns->mnemonic);
                    ImGui::SameLine();
                    ImGui::PopStyleColor();
                    ImGui::Text("%s", pIns->op_str);
                }
                else {
                    ImGui::TableSetColumnIndex(0);
                    ImGui::PushStyleColor(ImGuiCol_Text, ColorAddress);
                    ImGui::Text("0x%" PRIx64, address);
                    ImGui::PopStyleColor();

                    ImGui::TableSetColumnIndex(1);
                    ImGui::PushStyleColor(ImGuiCol_Text, ColorBytes);
                    ImGui::Text("00");
                    ImGui::PopStyleColor();

                    int col_idx = 2;
                    ImGui::TableSetColumnIndex(col_idx);
                    ImGui::PushStyleColor(ImGuiCol_Text, ColorDB);
                    ImGui::Text("db ", pIns->mnemonic);
                    ImGui::SameLine();
                    ImGui::PopStyleColor();
                    ImGui::Text("0x%" PRIx64, code_ptr);
                }
                cs_free(pIns, 1);
                
            }
        }
        ImGui::EndTable();
    }
	ImGui::End();
}