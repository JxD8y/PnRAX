#include <iostream>
#include <capstone/capstone.h>
#include <iomanip>

#include "Ui/DecompilerWindow.h"
#include "Ui/GlobalContext.h"
#include "Ui/ImGui/ImGuiWindowManager.h"
#include "PnLdr/PnLdr.h"

int main(int argc, char* argv[]) {
    
    ImGuiWindowManager wMgr("PnRAX", ImVec2(800,600));
    GlobalContext gCtx;
    gCtx.targetName = "TEST_TARGET.exe";

    DecompilerWindow wDecompiler(gCtx,ImVec2(0,0));
    
    
    csh ch;

    auto d = PnLdr::OpenFile("hxd.exe");
    
	gCtx.targetData = d;
    wDecompiler._init_cs();
    wMgr.AddWindow(&wDecompiler);
    wMgr.StartRendering();
    return 0;
}