#include <iostream>
#include <capstone/capstone.h>
#include <iomanip>

#include "Ui/DecompilerWindow.h"
#include "Ui/GlobalContext.h"
#include "Ui/ImGui/ImGuiWindowManager.h"
#include "PnLdr/PnLdr.h"

int main(int argc, char* argv[]) {
    
    /*ImGuiWindowManager wMgr("PnRAX", ImVec2(800,600));
    GlobalContext gCtx;
    gCtx.targetName = "TEST_TARGET.exe";

    DecompilerWindow wDecompiler(gCtx,ImVec2(0,0));
    DecompilerWindow wDecompiler1(gCtx,ImVec2(1,0));
    wMgr.AddWindow(&wDecompiler);
    wMgr.AddWindow(&wDecompiler1);
    wMgr.StartRendering();*/
    
    csh ch;

    auto d = PnLdr::OpenFile("test.exe");
    char* code = d->pTextSection;
    size_t codesz = d->textSectionSize;
    size_t base = 0x10000000;
    if (cs_open(CS_ARCH_X86, CS_MODE_32, &ch) == CS_ERR_OK) {
        cs_insn* insn = cs_malloc(ch);

        std::cout << std::left << std::setw(10) << "Address"
            << std::setw(10) << "Opcode"
            << "Operands" << std::endl;
        std::cout << "----------------------------------------" << std::endl;
        while (codesz > 0) {
            if (cs_disasm_iter(ch, const_cast<const uint8_t**>((uint8_t**)&code), &codesz, &base, insn)) {
                std::cout << "0x" << std::hex << insn->address << ":\t "
                    << std::left << std::setw(8) << insn->mnemonic << " "
                    << insn->op_str << std::endl;
            }
            else {
                code++;
                base++;
                codesz--;
            }
        }
    }
    return 0;
}