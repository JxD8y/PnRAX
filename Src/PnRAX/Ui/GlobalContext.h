#pragma once
#include <string>
#include <memory>

#include "../PnLdr/PnLPE.h"
#include "ImEasy/ImEasyWindow.h"

using namespace std;

class GlobalContext {
public:
	//Global data
	string targetName;
	string targetPath;
	bool isDark = true;

	//Window desc
	ImEasyWindow* pDecompilerWindow = NULL;
	ImEasyWindow* pPEViewerWindow = NULL;
	ImEasyWindow* pImportTableWindow = NULL;
	ImEasyWindow* pExportTableWindow = NULL;

	//Disassm data
	shared_ptr<PnLPE> targetData;
};