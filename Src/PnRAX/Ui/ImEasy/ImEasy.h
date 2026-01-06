#pragma once

#include "ImEasyConfig.h"
#include <dxgi.h>
#include <d3d11.h>
#include <assert.h>
#include <iomanip>
#include <ShellScalingApi.h>
#include <vector>
#include <map>
#include <imgui.h>
#include <functional>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>


#ifdef WIN32
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"d3dcompiler.lib")
#pragma comment(lib,"DXGI.lib")
#pragma comment(lib,"Shcore.lib")
#endif
#ifndef WIN32
#error "This library suit is only compatible with windows"
#endif

using namespace std;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

class ImGuiWindow;

struct MessageBoxData {
	enum class Level {
		Info,
		Warn,
		Error,
		Success
	} level = Level::Info;

	std::string title;
	std::string message;
	std::vector<std::string> buttons;
	std::function<void(int)> callback;

	bool isOpen = false;
};

namespace Imeasy {
	extern int width;
	extern int height;

	void ShowMessageBox(const std::string& title, const std::string& message,
		MessageBoxData::Level lv,
		const std::vector<std::string>& buttons,
		std::function<void(int)> callback = nullptr);

	void PrintErrorMessage(HRESULT hr);
	void StartRendering(string title, UINT height, UINT width);
	void SetRenderingList(vector<ImGuiWindow*> vWin);
	void SetTopMenu(function<void()> top);

	namespace __imeasy_internal{
		extern bool render;

		extern ID3D11Device* pdxDev;
		extern ID3D11DeviceContext* pdxDevCtx;
		extern ID3D11RenderTargetView* pRTV;
		extern IDXGISwapChain* pswCh;

		extern vector<ImGuiWindow*> vWindow;
		extern function<void()> tpHandler;
		extern MessageBoxData g_MsgBox;
		extern HWND hw;
		extern int rh, rw;

		bool CreateDXDevice();
		bool CreateDxSwapChain(HWND hw);
		bool CreateDxRenderTargetView();
		HWND CreateDxWindow(string title, UINT height, UINT width);
		LRESULT WndProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
		void ClearDxRenderingTarget();

		void CalculateTileWindowCord(float containerWidth, float containerHeight, float curX, float curY, float padding);
		void RenderTileWindows();
		void RenderMessageBox();
	}
}