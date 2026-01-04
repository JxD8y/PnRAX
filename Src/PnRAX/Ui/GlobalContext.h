#pragma once
#include <string>
#include <memory>
#include "../PnLdr/PnLPE.h"

using namespace std;

class GlobalContext {
public:
	string targetName;
	string targetPath;
	
	shared_ptr<PnLPE> targetData;
};