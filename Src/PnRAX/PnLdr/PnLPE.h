//Contains definitions for the PE
/*

[ FILE START (0x00) ]
      |
      v
+------------------------+
|   IMAGE_DOS_HEADER     |
|  [0x00] e_magic (MZ)   |
|  [0x3C] e_lfanew ---------------------------------------+
+------------------------+                                |
           |                                              |
           v                                              |
+------------------------+                                |
|      MS-DOS STUB       | <--- (Just sits here)          |
| "This program cannot..."                                |
+------------------------+                                |
           |                                              |
           v                                              |
+------------------------+ <------------------------------+
|   IMAGE_NT_HEADERS     | (Found via e_lfanew)
|                        |
|  [ Signature ] ("PE")  |
|                        |
|  [ IMAGE_FILE_HEADER ] |
|   - Machine            |
|   - NumberOfSections   |
|   - SizeOfOptionalHeader --+
|                        |   |
| [IMAGE_OPTIONAL_HEADER]|   |
|   - EntryPoint         |   |
|   - ImageBase          |   |
|   - Data Directories   |   |
+------------------------+   |
           |                 |
           v                 |
+------------------------+ <++ (Calculated via SizeOfOptionalHeader)
|     SECTION TABLE      |
| [Section Header 1]     |
|   - Name: ".text"      |
|   - PointerToRawData ---------------------+
|   - VirtualAddress     |                  |
|                        |                  |
| [Section Header 2]     |                  |
|   - Name: ".data"      |                  |
|   - PointerToRawData -----------------------------------+
+------------------------+                  |             |
           |                                |             |
           v                                |             |
+------------------------+ <----------------+             |
|    SECTION 1 DATA      |                                |
|  (.text / Code bytes)  |                                |
+------------------------+                                |
           |                                              |
           v                                              |
+------------------------+ <------------------------------+
|    SECTION 2 DATA      |
|  (.data / Globals)     |
+------------------------+


*/

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