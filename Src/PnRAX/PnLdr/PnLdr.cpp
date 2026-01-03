#include "PnLdr.h"

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
	pLp->is_64 = pLp->ntHeader.OptionalHeader.Magic == 0x20b;
	LONG sectionHeaderOffset = pLp->dosHeader.e_lfanew + sizeof(pLp->ntHeader);

	for (int i = 0; i < pLp->ntHeader.FileHeader.NumberOfSections; i++) {
		IMAGE_SECTION_HEADER _shdr = { 0 };
		file.seekg(sectionHeaderOffset+i*sizeof(IMAGE_SECTION_HEADER), std::ios::beg);
		file.read(reinterpret_cast<char*>(&_shdr), sizeof(IMAGE_SECTION_HEADER));
		pLp->sectionHeaders.push_back(_shdr);
		cout << "[" << i << "]" << _shdr.Name << "\0" << endl;
	}
	for (auto& sc : pLp->sectionHeaders) {
		//Parsing and storing the sections we need
		if (string(".text") == string((char*)sc.Name)) {
			cout << "Found .text" << endl;
			pLp->pTextSection = (char*)malloc(sc.SizeOfRawData);
			pLp->textSectionSize = sc.Misc.VirtualSize;
			file.seekg(sc.PointerToRawData, std::ios::beg);
			file.read(pLp->pTextSection, sc.SizeOfRawData);
			
		}
	}
	return pLp;
}