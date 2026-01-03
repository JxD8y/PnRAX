#pragma once
#include <Windows.h>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>
#include "PnLPE.h"


using namespace std;
namespace PnLdr{
	shared_ptr<PnLPE> OpenFile(string path);
}
