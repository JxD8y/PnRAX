
#include <dxgi.h>
#include <d3d11.h>
#include <assert.h>
#include <iomanip>
#include <ShellScalingApi.h>
#include <vector>
#include <map>
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#pragma once

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

namespace Imeasy {
	extern int width;
	extern int height;
	void PrintErrorMessage(HRESULT hr);
	void StartRendering(string title, UINT height, UINT width);
	void SetRenderingList(vector<ImGuiWindow*> vWin);

	namespace {
		LRESULT WndProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp);
		int rh, rw = 0;
		bool render = true;
		ID3D11Device* pdxDev = NULL;
		ID3D11DeviceContext* pdxDevCtx = NULL;
		ID3D11RenderTargetView* pRTV = NULL;
		IDXGISwapChain* pswCh = NULL;
		vector<ImGuiWindow*> vWindow;
		HWND hw;
		bool CreateDXDevice();
		bool CreateDxSwapChain(HWND hw);
		bool CreateDxRenderTargetView();
		HWND CreateDxWindow(string title, UINT height, UINT width);
		void ClearDxRenderingTarget();

		void CalculateTileWindowCord(float padding);
		void RenderTileWindows();
	}
}