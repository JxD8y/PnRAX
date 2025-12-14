//Fill the PnLPE structures
#ifndef PNLDR_H
#define PNLDR_H
#include <Windows.h>
#include <memory>
#include "PnLPE.h"

namespace PnLdr {
	std::shared_ptr<PnLPE> LoadPE(char* fileBytes);
	void PrintPEDigest(PnLPE& pnPEdata);
}

#endif