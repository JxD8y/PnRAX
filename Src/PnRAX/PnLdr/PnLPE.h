//Contains definitions for the PE
#ifndef PNLPE_H
#define PNLPE_H
#include <winnt.h>

class PnLPE {
public:

	PIMAGE_DOS_HEADER dosHeader = { 0 };
	PIMAGE_NT_HEADERS ntHeader = { 0 };
	PIMAGE_OPTIONAL_HEADER optionalHeader = { 0 };
	PIMAGE_SECTION_HEADER sectionHeader = { 0 };

};
#endif