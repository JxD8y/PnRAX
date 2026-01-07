#include <windows.h>
#include <iostream>
#include "imgui.h"
#include "resource.h" 

bool LoadFontFromResource(int resourceID)
{
    HMODULE hInstance = GetModuleHandle(NULL);

    HRSRC hRes = FindResource(hInstance, MAKEINTRESOURCE(resourceID), RT_RCDATA);
    if (!hRes) {
        std::cerr << "Error: Could not find font resource!" << std::endl;
        return false;
    }
    HGLOBAL hMem = LoadResource(hInstance, hRes);
    if (!hMem) {
        std::cerr << "Error: Could not load font resource!" << std::endl;
        return false;
    }

    void* dataPtr = LockResource(hMem);
    DWORD dataSize = SizeofResource(hInstance, hRes);

    if (!dataPtr || dataSize == 0) {
        std::cerr << "Error: Font resource is empty or invalid!" << std::endl;
        return false;
    }

    void* fontDataCopy = malloc(dataSize);
    if (fontDataCopy) {
        memcpy(fontDataCopy, dataPtr, dataSize);

        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig config;
        config.FontDataOwnedByAtlas = false; 
        config.OversampleH = 3;
        config.OversampleV = 1; 
        config.PixelSnapH = true;

        ImFont* font = io.Fonts->AddFontFromMemoryTTF(fontDataCopy, (int)dataSize, 19.0f,&config);

        if (font == NULL) {
            std::cerr << "Error: ImGui failed to load the font." << std::endl;
            free(fontDataCopy);
            return false;
        }
        return true;
    }
}