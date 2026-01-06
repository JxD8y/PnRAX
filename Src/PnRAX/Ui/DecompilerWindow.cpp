#include "DecompilerWindow.h"

DecompilerWindow::DecompilerWindow(GlobalContext& ctx,ImVec2 grid, ImVec2 rec )
:ImEasyWindow("Decompiler: ",grid), m_gctx(ctx)
{
    this->HW = rec;
    this->_cs_hnd = -1;
	this->SetWindowTitle(fmt::format("Decompiler: {}##{}", this->m_gctx.targetName, this->m_id));
}

DecompilerWindow::~DecompilerWindow() {
    cs_close(&this->_cs_hnd);
}
void RenderBytes(const uint8_t* bytes, uint16_t size) {
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
    cs_option(this->_cs_hnd, CS_OPT_DETAIL, CS_OPT_ON);

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
const char* get_ptr_size_name(uint8_t size) {
    switch (size) {
    case 1:  return "byte ptr";
    case 2:  return "word ptr";
    case 4:  return "dword ptr";
    case 8:  return "qword ptr";
    case 16: return "xmmword ptr";
    case 32: return "ymmword ptr";
    case 64: return "zmmword ptr";
    case 10: return "tbyte ptr";
    default: return "";
    }
}

void DecompilerWindow::Render() {
    ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(.80f, 0.65f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(1.0f, 0.65f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(1.0f, 0.65f, 0.2f, 1.0f));
	ImGui::Begin(this->m_title.c_str(), NULL ,ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);
    ImGui::PopStyleColor(3);
    int columns = 3;

    ImGuiTableFlags flags =  ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY;
    if (ImGui::BeginTable("DisassemblyTable", columns, flags)) { 
        ImGui::TableSetupColumn("Address", ImGuiTableColumnFlags_WidthFixed, 80.0f);
        ImGui::TableSetupColumn("Bytes", ImGuiTableColumnFlags_WidthFixed, 80.0f);
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
                    cs_x86 *pDetail = &(pIns->detail->x86);

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
                    if(this->m_gctx.isDark)
                        ImGui::PushStyleColor(ImGuiCol_Text, ColorMnemonicDark);
                    else
                        ImGui::PushStyleColor(ImGuiCol_Text, ColorMnemonicLight);
                    ImGui::Text("%s ", pIns->mnemonic);
                    ImGui::SameLine();
                    ImGui::PopStyleColor();

                    for (int i = 0; i < pDetail->op_count; i++) {
                        if (i >= 1) {
                            ImGui::Text(",");
                            ImGui::SameLine();
                        }
                        cs_x86_op* op = &pDetail->operands[i];
                        switch (op->type) {
                            case X86_OP_REG:
                                ImGui::PushStyleColor(ImGuiCol_Text, ColorReg);
                                ImGui::Text("%s ", cs_reg_name(this->_cs_hnd,op->reg));
                                ImGui::SameLine();
                                ImGui::PopStyleColor();
                                break;

                            case X86_OP_IMM:
                                ImGui::PushStyleColor(ImGuiCol_Text, ColorImm);
                                ImGui::Text("0x%" PRIx64,op->imm);
                                ImGui::SameLine();
                                ImGui::PopStyleColor();
                                break;

                            case X86_OP_MEM:
                                ImGui::PushStyleColor(ImGuiCol_Text, ColorMemSize);
                                ImGui::Text("%s",get_ptr_size_name(op->size));
                                ImGui::PopStyleColor();
                                ImGui::SameLine();
                                ImGui::PushStyleColor(ImGuiCol_Text, ColorMem);
                                ImGui::Text("[");
                                ImGui::SameLine();
                                if (op->mem.base != X86_REG_INVALID) {
                                    auto rgName = cs_reg_name(this->_cs_hnd, op->mem.base);
                                    ImGui::Text("%s", rgName);
                                    if (op->mem.index != X86_REG_INVALID) {
                                        ImGui::SameLine();
                                        ImGui::Text(" + %s", cs_reg_name(this->_cs_hnd, op->mem.index));
                                        ImGui::SameLine();
                                        if (op->mem.scale > 1)
                                            ImGui::Text("*%d", op->mem.scale);

                                    }
                                }
                                else {
                                    ImGui::Text("0x%" PRIx64 ,op->mem.disp);
                                }
                                ImGui::SameLine();
                                ImGui::Text("]");
                                ImGui::PopStyleColor();
                                ImGui::SameLine();
                                break;
                            default:
                                break;
                        }
                    }

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
                    ImGui::Text("%02X", code_ptr[offset]);
                    
                }
                cs_free(pIns, 1);
                
            }
        }
        ImGui::EndTable();
    }
	ImGui::End();
}