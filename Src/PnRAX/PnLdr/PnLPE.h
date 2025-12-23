//Contains definitions for the PE
#ifndef PNLPE_H
#define PNLPE_H
#include <winnt.h>

class PnLPE {
public:

	PIMAGE_DOS_HEADER dosHeader = { 0 }; // emagic/e_lfanew e_lfanew is used to jump over DOS_STUB
	PIMAGE_NT_HEADERS ntHeader = { 0 }; //signature: PE\0\0	machine , numberofsections , sizeofoptionalheader
	PIMAGE_OPTIONAL_HEADER optionalHeader = { 0 }; //magic addrofentrypoint , imagebase,sectionAlignment, fileAlignment, dataDirectory ( import/export table) 
	PIMAGE_SECTION_HEADER sectionHeader = { 0 }; //multiple of them exist should be read by sizeof(image_section_headers) * sectionCount -> give us the name VADR and offset of the section in the file alongside with its size
	
};
#endif