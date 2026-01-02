#include <iostream>
#include "Ui/DecompilerWindow.h"
#include "Ui/GlobalContext.h"
#include "Ui/ImGui/ImGuiWindowManager.h"

int main(int argc, char* argv[]) {
    
    ImGuiWindowManager wMgr("PnRAX", ImVec2(800,600));
    GlobalContext gCtx;
    gCtx.targetName = "TEST_TARGET.exe";

    DecompilerWindow wDecompiler(gCtx,ImVec2(0,0));
    DecompilerWindow wDecompiler1(gCtx,ImVec2(1,0));
    wMgr.AddWindow(&wDecompiler);
    wMgr.AddWindow(&wDecompiler1);
    wMgr.StartRendering();
    return 0;
}