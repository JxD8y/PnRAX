#include "Ui/ImEasy/ImEasyWindowManager.h"
#include "Ui/DecompilerWindow.h"
#include "Ui/PEViewer.h"
#include "Ui/IETableWindow.h"
#include "Ui/GlobalContext.h"
#include "Ui/TopMenu.h"
#include "PnLdr/PnLdr.h"

#include <iostream>
#include <capstone/capstone.h>
#include <iomanip>


int main(int argc, char* argv[]) {
    
    ImEasyWindowManager wMgr("PnRAX", ImVec2(1200,700));
    GlobalContext gCtx;

    DecompilerWindow wDecompiler(gCtx,ImVec2(0,0),ImVec2(800,700));
    PEViewerWindow wPEViewer(gCtx, ImVec2(1, 0),ImVec2(500,700));
	IETableWindow wImportTable(gCtx, ImVec2(2, 0), true, ImVec2(400, 350));
	IETableWindow wExportTable(gCtx, ImVec2(2, 1), false, ImVec2(400, 350));

	gCtx.pDecompilerWindow = &wDecompiler;
	gCtx.pPEViewerWindow = &wPEViewer;
	gCtx.pImportTableWindow = &wImportTable;
	gCtx.pExportTableWindow = &wExportTable;
    
    TopMenu::g_ctx = &gCtx;

    csh ch;

    wMgr.SetTopMenu(TopMenu::RenderApplicationTopMenu);
    wMgr.AddWindow(&wDecompiler);
    wMgr.AddWindow(&wImportTable);
    wMgr.AddWindow(&wExportTable);
    wMgr.AddWindow(&wPEViewer);
    wMgr.StartRendering();
    return 0;
}