#include "PnLdr.h"

DWORD Rva2Offset(DWORD rva, IMAGE_NT_HEADERS pNtHdr, vector<IMAGE_SECTION_HEADER> vSech) {
	if (rva == 0) return 0;
	for (auto sec : vSech) {
		DWORD vStart = sec.VirtualAddress;
		DWORD vSize = sec.Misc.VirtualSize;
		if (rva >= vStart && rva < vStart + vSize) {
			return sec.PointerToRawData + (rva - vStart);
		}
	}
	return 0;
}


shared_ptr<PnLPE> PnLdr::OpenFile(string path) {
	ifstream file(path, std::ios::binary);
	if (!file.is_open()) {
		throw runtime_error("Failed to open file: " + path);
	}
	auto pLp = make_shared<PnLPE>();

	file.read(reinterpret_cast<char*>(&pLp->dosHeader), sizeof(pLp->dosHeader));
	if (pLp->dosHeader.e_magic != IMAGE_DOS_SIGNATURE) {
		throw runtime_error("Invalid DOS signature");
	}
	LONG ntHeaderOffset = pLp->dosHeader.e_lfanew;
	file.seekg(ntHeaderOffset, std::ios::beg);
	file.read(reinterpret_cast<char*>(&pLp->ntHeader), sizeof(pLp->ntHeader));
	if (pLp->ntHeader.Signature != IMAGE_NT_SIGNATURE) {
		throw runtime_error("Couldn't find the NT header");
	}

	//Ignoring the import/export entry point and ... for now
	//pLp->ntHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT];
	pLp->optionalHeader = pLp->ntHeader.OptionalHeader;
	pLp->is_64 = pLp->ntHeader.OptionalHeader.Magic == 0x20b;
	LONG sectionHeaderOffset = pLp->dosHeader.e_lfanew + sizeof(pLp->ntHeader);
	DWORD EntryPointVA = pLp->ntHeader.OptionalHeader.AddressOfEntryPoint;
	cout << "Entry Point: " << EntryPointVA << endl;

	for (int i = 0; i < pLp->ntHeader.FileHeader.NumberOfSections; i++) { //check the entry point RVA
		IMAGE_SECTION_HEADER _shdr = { 0 };
		file.seekg(sectionHeaderOffset+i*sizeof(IMAGE_SECTION_HEADER), std::ios::beg);
		file.read(reinterpret_cast<char*>(&_shdr), sizeof(IMAGE_SECTION_HEADER));

		DWORD VAbegin = _shdr.VirtualAddress;
		DWORD VAend = _shdr.Misc.VirtualSize;

		if (string((char*)_shdr.Name) == "")
		{
			string tName;
			if (EntryPointVA >= VAbegin && EntryPointVA <= VAend)
				tName = ".text";
			else
				tName = ("Unnamed_Section_" + to_string(i));

			memset(_shdr.Name, 0, sizeof(_shdr.Name));
			strncpy((char*)_shdr.Name, tName.c_str(), tName.size());
		}

		pLp->sectionHeaders.push_back(_shdr);
		cout << "[" << i << "]" << _shdr.Name << " " << _shdr.VirtualAddress << "\0" << endl;
	}
	for (auto& sc : pLp->sectionHeaders) {
		//Parsing and storing the sections we need
		if ((string(".text") == string((char*)sc.Name) || (sc.Characteristics & IMAGE_SCN_MEM_EXECUTE)) && sc.SizeOfRawData > 0) {

			//I assumed that the first executable section is .text ( a fallback if no .text name is found)
			//In next updates i will add a dynamic section viewer

			cout << "Found .text" << endl;
			pLp->pTextSection = (char*)malloc(sc.SizeOfRawData);
			pLp->textSectionSize = sc.SizeOfRawData;
			file.seekg(sc.PointerToRawData, std::ios::beg);
			file.read(pLp->pTextSection, sc.SizeOfRawData);
			break;
		}
	}

	long size = file.seekg(0, std::ios::end).tellg();
	file.seekg(0, std::ios::beg);

	char* fileData = (char*)malloc(size);
	file.read(fileData, size);

	DWORD importRVA = pLp->optionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	DWORD importOffset = Rva2Offset(importRVA, pLp->ntHeader, pLp->sectionHeaders);
	if (importRVA == 0)
		goto End;
	PIMAGE_IMPORT_DESCRIPTOR pImport = (PIMAGE_IMPORT_DESCRIPTOR)((BYTE*)fileData + importOffset);
	while (pImport->Name != 0) {
		DWORD nameOffset = Rva2Offset(pImport->Name, pLp->ntHeader, pLp->sectionHeaders);
		char* libName = (char*)((BYTE*)fileData + nameOffset);
		DWORD thunkRVA = pImport->OriginalFirstThunk;
		if (thunkRVA == 0) thunkRVA = pImport->FirstThunk;

		DWORD thunkOffset = Rva2Offset(thunkRVA, pLp->ntHeader, pLp->sectionHeaders);
		PIMAGE_THUNK_DATA pThunk = (PIMAGE_THUNK_DATA)((BYTE*)fileData + thunkOffset);
		while (pThunk->u1.AddressOfData != 0) {
			if (pThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG) {
				pLp->importTable.insert({ libName, to_string(pThunk->u1.Ordinal & 0xFFFF) });
			}
			else {
				DWORD nameDataRVA = (DWORD)pThunk->u1.AddressOfData;
				DWORD nameDataOffset = Rva2Offset(nameDataRVA, pLp->ntHeader, pLp->sectionHeaders);
				PIMAGE_IMPORT_BY_NAME pNameData = (PIMAGE_IMPORT_BY_NAME)((BYTE*)fileData + nameDataOffset);
				pLp->importTable.insert({ libName, pNameData->Name });
			}

			pThunk++;
		}

		pImport++;
	}
	DWORD exportRVA = pLp->optionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	DWORD exportSize = pLp->optionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
	if (exportRVA == 0)
		goto End;

	DWORD exportOffset = Rva2Offset(exportRVA, pLp->ntHeader, pLp->sectionHeaders);
	PIMAGE_EXPORT_DIRECTORY pExport = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)fileData + exportOffset);

	DWORD dllNameOffset = Rva2Offset(pExport->Name, pLp->ntHeader, pLp->sectionHeaders);

	DWORD funcOffset = Rva2Offset(pExport->AddressOfFunctions, pLp->ntHeader, pLp->sectionHeaders);
	DWORD nameOffset = Rva2Offset(pExport->AddressOfNames, pLp->ntHeader, pLp->sectionHeaders);
	DWORD ordinalOffset = Rva2Offset(pExport->AddressOfNameOrdinals, pLp->ntHeader, pLp->sectionHeaders);

	DWORD* pFuncs = (DWORD*)((BYTE*)fileData + funcOffset);
	DWORD* pNames = (DWORD*)((BYTE*)fileData + nameOffset);
	WORD* pOrdinals = (WORD*)((BYTE*)fileData + ordinalOffset);

	for (DWORD i = 0; i < pExport->NumberOfNames; i++) {
		DWORD nameStrOffset = Rva2Offset(pNames[i], pLp->ntHeader, pLp->sectionHeaders);
		char* funcName = (char*)((BYTE*)fileData + nameStrOffset);

		WORD ordinalIndex = pOrdinals[i];

		if (ordinalIndex < pExport->NumberOfFunctions) {
			DWORD funcRVA = pFuncs[ordinalIndex];

			if (funcRVA >= exportRVA && funcRVA < (exportRVA + exportSize)) {
				DWORD forwarderOffset = Rva2Offset(funcRVA, pLp->ntHeader, pLp->sectionHeaders);
				char* forwarderStr = (char*)((BYTE*)fileData + forwarderOffset);
				pLp->exportTable.insert({ funcRVA, funcName });
			}
			else {
				pLp->exportTable.insert({ funcRVA, funcName });
			}
		}
	}
	End:
	free(fileData);

	return pLp;
}