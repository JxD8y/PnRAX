#include "ImEasyWindow.h"

#include <algorithm>
#include <iostream>

using namespace std;
namespace _imeasy = Imeasy::__imeasy_internal;

namespace Imeasy {
	bool TerminateOnClose = true;
	int width = 0;
	int height = 0;

	void PrintErrorMessage(HRESULT hr) {
		LPVOID lpMsgBuf;
		DWORD dw = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			nullptr,
			hr,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPSTR)&lpMsgBuf,
			0, nullptr);

		if (dw) {
			std::wcerr << L"Error: " << (LPWSTR)lpMsgBuf << std::endl;
			LocalFree(lpMsgBuf);
		}
		else {
			std::cerr << "Unknown error occurred." << std::endl;
		}
	}

	void SetRenderingList(vector<ImGuiWindow*> vWindow) {
		_imeasy::vWindow = vWindow;
	}

	
	void StartRendering(string title, UINT height, UINT width) {
		if (!_imeasy::CreateDXDevice())
		{
			throw exception("failed to create dx dev");
		}
		HWND hw = _imeasy::CreateDxWindow(title.c_str(), height, width);
		if (hw == NULL)
		{
			throw exception("failed to create window");
		}
		ShowWindow(hw, SW_SHOW);
		if (!_imeasy::CreateDxSwapChain(hw)) {
			throw exception("failed to create swap chain");
		}
		if (!_imeasy::CreateDxRenderTargetView()) {
			throw exception("failed to create dx render target view");
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGuiIO& io = ImGui::GetIO();
		auto pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Arial.ttf", 22.0);
		if (pFont == NULL) {
			io.Fonts->AddFontDefault();
		}
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ImGuiStyle& style = ImGui::GetStyle();
		

		ImGui::StyleColorsDark(&style);
		ImGui_ImplWin32_Init(hw);
		ImGui_ImplDX11_Init(_imeasy::pdxDev, _imeasy::pdxDevCtx);
		bool run = true;
		const float cColor[4] = { 0,0,0,0 };
		while (run) {
			MSG msg;
			while (PeekMessageA(&msg, _imeasy::hw, 0, 0, PM_REMOVE)) {
				TranslateMessage(&msg);
				DispatchMessageA(&msg);
				if (msg.message == WM_CLOSE)
				{
					run = false;
				}
			}

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			if (_imeasy::tpHandler != NULL)
				_imeasy::tpHandler();

			ImGuiViewport* vp = ImGui::GetMainViewport();
			_imeasy::CalculateTileWindowCord(vp->WorkSize.x,vp->WorkSize.y,vp->WorkPos.x,vp->WorkPos.y ,0);
			_imeasy::RenderTileWindows();
			_imeasy::RenderMessageBox();
			
			if (_imeasy::rw != 0 && _imeasy::rh != 0)
			{
				_imeasy::ClearDxRenderingTarget();
				_imeasy::pswCh->ResizeBuffers(0, _imeasy::rw, _imeasy::rh, DXGI_FORMAT_UNKNOWN, 0);
				_imeasy::rw = _imeasy::rh = 0;
				_imeasy::CreateDxRenderTargetView();
			}

			ImGui::Render();
			_imeasy::pdxDevCtx->OMSetRenderTargets(1, &_imeasy::pRTV, NULL);
			_imeasy::pdxDevCtx->ClearRenderTargetView(_imeasy::pRTV, cColor);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			_imeasy::pswCh->Present(1, 0);

		}
	}
	void SetTopMenu(function<void()> top) {
		_imeasy::tpHandler = top;
	}


	namespace __imeasy_internal {

		bool render = true;

		ID3D11Device* pdxDev = NULL;
		ID3D11DeviceContext* pdxDevCtx = NULL;
		ID3D11RenderTargetView* pRTV = NULL;
		IDXGISwapChain* pswCh = NULL;

		vector<ImGuiWindow*> vWindow;
		function<void()> tpHandler;
		HWND hw;
		int rh, rw = 0;

		LRESULT WndProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
			if (msg == WM_SIZE) {
				if (wp == SIZE_MINIMIZED)
					return 0;
				width = _imeasy::rw = (UINT)LOWORD(lp);
				height = _imeasy::rh = (UINT)HIWORD(lp);
				return 0;
			}
			if (msg == WM_CLOSE)
				if (TerminateOnClose) {
					HANDLE hProc = GetCurrentProcess();
					TerminateProcess(hProc, 0);
				}
			if (ImGui_ImplWin32_WndProcHandler(hw, msg, wp, lp))
				return true;
			return DefWindowProcA(hw, msg, wp, lp);
		}
		bool CreateDXDevice() {
			HRESULT hr = D3D11CreateDevice(
				NULL,
				D3D_DRIVER_TYPE_HARDWARE,
				NULL,
				0,
				NULL,
				0,
				D3D11_SDK_VERSION,
				&_imeasy::pdxDev,
				NULL,
				&_imeasy::pdxDevCtx
			);
			if (hr != S_OK)
			{
				cout << "Couldn't create DX device" << endl;
				PrintErrorMessage(hr);
				return false;
			}
			return true;
		}

		bool CreateDxSwapChain(HWND hw) {
			IDXGIFactory* factory = NULL;
			HRESULT hr = CreateDXGIFactory(__uuidof(factory), (void**)(&factory));
			if (hr != S_OK) {
				cout << "Cannot create DX factory" << endl;
				PrintErrorMessage(hr);
				return false;
			}
			DXGI_SWAP_CHAIN_DESC pdDx;
			ZeroMemory(&pdDx, sizeof(pdDx));
			pdDx.BufferCount = 2;
			pdDx.BufferDesc.Width = 0;
			pdDx.BufferDesc.Height = 0;
			pdDx.BufferDesc.RefreshRate.Numerator = 120;
			pdDx.BufferDesc.RefreshRate.Denominator = 1;
			pdDx.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			pdDx.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			pdDx.SampleDesc.Count = 1;
			pdDx.SampleDesc.Quality = 0;
			pdDx.OutputWindow = hw;
			pdDx.Windowed = TRUE;
			pdDx.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
			pdDx.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

			hr = factory->CreateSwapChain(_imeasy::pdxDev, &pdDx, &_imeasy::pswCh);
			if (hr != S_OK) {
				cout << "Cannot create the swap chain" << std::hex << hr << endl;
				PrintErrorMessage(hr);
				return false;
			}

			factory->Release();
			return true;
		}

		bool CreateDxRenderTargetView() {
			ID3D11Texture2D* bBuffer = NULL;
			HRESULT hr = _imeasy::pswCh->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&bBuffer));
			if (hr != S_OK) {
				cout << "Cannot get the back buffer from swap chain" << endl;
				PrintErrorMessage(hr);
				return false;
			}

			hr = _imeasy::pdxDev->CreateRenderTargetView(bBuffer, NULL, &_imeasy::pRTV);
			if (hr != S_OK) {
				cout << "Cannot create RTV" << endl;
				PrintErrorMessage(hr);
				return false;
			}
			bBuffer->Release();
			return true;
		}

		HWND CreateDxWindow(string title, UINT height, UINT width) {
			SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
			WNDCLASSA wcx = {};
			wcx.hInstance = GetModuleHandleA(NULL);
			wcx.lpszClassName = title.c_str();
			wcx.lpfnWndProc = WndProc;
			RegisterClassA(&wcx);
			HWND hw = CreateWindowExA(0, wcx.lpszClassName, title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, wcx.hInstance, NULL);
			return hw;
		}

		void ClearDxRenderingTarget() {
			pRTV->Release();
			pRTV = NULL;
		}
	}
}