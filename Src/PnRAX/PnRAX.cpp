#include "Ui/ImEasy/ImEasyWindowManager.h"
#include "Ui/DecompilerWindow.h"
#include "Ui/PEViewer.h"
#include "Ui/IETableWindow.h"
#include "Ui/GlobalContext.h"
#include "Ui/TopMenu.h"
#include "PnLdr/PnLdr.h"
#include "Ui/Res/ResUtil.h"

#include <iostream>
#include <capstone/capstone.h>
#include <iomanip>
#include <shlwapi.h>
#include "PnRAX.h"

#pragma comment(lib ,"Shlwapi.lib")

GlobalContext gCtx;

void LoadNewFile(string filePath) {

    GlobalContext* g_ctx = TopMenu::g_ctx;
    gCtx.targetName = string(PathFindFileNameA(filePath.c_str()));
    auto d = PnLdr::OpenFile(filePath);
    g_ctx->targetData = d;

    ((DecompilerWindow*)g_ctx->pDecompilerWindow)->_clear_offset();
    ((DecompilerWindow*)g_ctx->pDecompilerWindow)->_init_cs();
}

int WinMain(HINSTANCE hin,HINSTANCE hpr, LPSTR cmd, int cmdShow) {
    ImEasyWindowManager wMgr("PnRAX", ImVec2(1200,700));

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    LoadFontFromResource(IDR_APP_FONT_GLOBAL);

    DecompilerWindow wDecompiler(gCtx,ImVec2(0,0),ImVec2(800,700));
    PEViewerWindow wPEViewer(gCtx, ImVec2(1, 0),ImVec2(500,700));
	IETableWindow wImportTable(gCtx, ImVec2(2, 0), true, ImVec2(400, 350));
	IETableWindow wExportTable(gCtx, ImVec2(2, 1), false, ImVec2(400, 350));

	gCtx.pDecompilerWindow = &wDecompiler;
	gCtx.pPEViewerWindow = &wPEViewer;
	gCtx.pImportTableWindow = &wImportTable;
	gCtx.pExportTableWindow = &wExportTable;
    
    TopMenu::g_ctx = &gCtx;
	TopMenu::vStr = "V0.1";

    csh ch;

    HICON hIcon = (HICON)LoadImage(hin, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, GetSystemMetrics(SM_CXICON), GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
    HICON hIconSmall = (HICON)LoadImage(hin, MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);

    wMgr.SetTopMenu(TopMenu::RenderApplicationTopMenu);
    wMgr.SetWin32Icons(hIcon, hIconSmall);

    wMgr.AddWindow(&wDecompiler);
    wMgr.AddWindow(&wImportTable);
    wMgr.AddWindow(&wExportTable);
    wMgr.AddWindow(&wPEViewer);
    wMgr.StartRendering();
    return 0;
}