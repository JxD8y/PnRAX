#include "ImGui_Impl_easy.h"
#include "ImGuiWindow.h"
#include <algorithm>
#include <iostream>

using namespace std;

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
		Imeasy::vWindow = vWindow;
	}

	void CalculateTileWindowCord(float containerWidth, float containerHeight, float curX, float curY, float padding) {
		std::map<int, float> colBaseWidths;
		std::map<int, float> colTotalHeights;

		for (ImGuiWindow* win : Imeasy::vWindow) {
			if (win->HW.x > colBaseWidths[win->Grid.x])
				colBaseWidths[win->Grid.x] = win->HW.x;
			if (colTotalHeights.find(win->Grid.x) == colTotalHeights.end()) {
				colTotalHeights[win->Grid.x] = 0.0f;
			}
			colTotalHeights[win->Grid.x] += win->HW.y;
		}
		colTotalHeights.clear();
		std::map<int, int> colItemCounts;
		for (ImGuiWindow* win : Imeasy::vWindow) {
			if (win->HW.x > colBaseWidths[win->Grid.x]) colBaseWidths[win->Grid.x] = win->HW.x;
			colTotalHeights[win->Grid.x] += win->HW.y;
			colItemCounts[win->Grid.x]++;
		}
		for (auto& p : colTotalHeights) {
			int count = colItemCounts[p.first];
			if (count > 1) p.second += (count - 1) * padding;
		}
		float totalBaseW = 0.0f;
		for (auto const& p : colBaseWidths) totalBaseW += p.second;
		float totalPaddingX = (colBaseWidths.size() > 0) ? (colBaseWidths.size() - 1) * padding : 0.0f;

		float maxStackH = 0.0f;
		for (auto const& p : colTotalHeights) {
			if (p.second > maxStackH) maxStackH = p.second;
		}
		float scaleX = (totalBaseW > 0) ? (containerWidth - totalPaddingX) / totalBaseW : 1.0f;
		float scaleY = (maxStackH > 0) ? (containerHeight) / maxStackH : 1.0f; 

		std::map<int, float> finalColWidths;
		for (auto const& p : colBaseWidths) finalColWidths[p.first] = p.second * scaleX;

		std::vector<ImGuiWindow*> cvWindow = Imeasy::vWindow;
		std::sort(cvWindow.begin(), cvWindow.end(), [](ImGuiWindow* a, ImGuiWindow* b) {
			if (a->Grid.y != b->Grid.y) return a->Grid.y < b->Grid.y;
			return a->Grid.x < b->Grid.x;
			});
		std::map<int, float> colStackY;

		for (ImGuiWindow* win : cvWindow) {
			float finalX = curX;
			for (auto const& p : finalColWidths) {
				if (p.first < win->Grid.x) {
					finalX += p.second + padding;
				}
			}
			if (colStackY.find(win->Grid.x) == colStackY.end()) {
				colStackY[win->Grid.x] = curY;
			}

			win->ComputedXY.x = finalX;
			win->ComputedXY.y = colStackY[win->Grid.x];

			win->ComputedHW.x = finalColWidths[win->Grid.x];
			win->ComputedHW.y = win->HW.y * scaleY;

			colStackY[win->Grid.x] += win->ComputedHW.y + padding;
		}
	}
	void RenderTileWindows() {
		for (ImGuiWindow* img : Imeasy::vWindow) {
			if (img->is_open) {
				ImGui::SetNextWindowPos(img->ComputedXY, ImGuiCond_Always);
				ImGui::SetNextWindowSize(img->ComputedHW, ImGuiCond_Always);

				ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoSavedSettings;

				img->Render();
			}
		}
	}
	
	void StartRendering(string title, UINT height, UINT width) {
		if (!Imeasy::CreateDXDevice())
		{
			throw exception("failed to create dx dev");
		}
		HWND hw = CreateDxWindow(title.c_str(), height, width);
		if (hw == NULL)
		{
			throw exception("failed to create window");
		}
		ShowWindow(hw, SW_SHOW);
		if (!CreateDxSwapChain(hw)) {
			throw exception("failed to create swap chain");
		}
		if (!CreateDxRenderTargetView()) {
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
		ImGui_ImplDX11_Init(Imeasy::pdxDev, Imeasy::pdxDevCtx);
		bool run = true;
		const float cColor[4] = { 0,0,0,0 };
		while (run) {
			MSG msg;
			while (PeekMessageA(&msg, Imeasy::hw, 0, 0, PM_REMOVE)) {
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
			if (Imeasy::tpHandler != NULL)
				Imeasy::tpHandler();
			ImGuiViewport* vp = ImGui::GetMainViewport();
			Imeasy::CalculateTileWindowCord(vp->WorkSize.x,vp->WorkSize.y,vp->WorkPos.x,vp->WorkPos.y ,0);
			Imeasy::RenderTileWindows();
			
			if (Imeasy::rw != 0 && Imeasy::rh != 0)
			{
				Imeasy::ClearDxRenderingTarget();
				Imeasy::pswCh->ResizeBuffers(0, Imeasy::rw, Imeasy::rh, DXGI_FORMAT_UNKNOWN, 0);
				Imeasy::rw = Imeasy::rh = 0;
				CreateDxRenderTargetView();
			}

			ImGui::Render();
			Imeasy::pdxDevCtx->OMSetRenderTargets(1, &Imeasy::pRTV, NULL);
			Imeasy::pdxDevCtx->ClearRenderTargetView(Imeasy::pRTV, cColor);
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
			Imeasy::pswCh->Present(1, 0);

		}
	}
	void SetTopMenu(function<void()> top) {
		Imeasy::tpHandler = top;
	}


	namespace {
		LRESULT WndProc(HWND hw, UINT msg, WPARAM wp, LPARAM lp) {
			if (msg == WM_SIZE) {
				if (wp == SIZE_MINIMIZED)
					return 0;
				Imeasy::width = Imeasy::rw = (UINT)LOWORD(lp);
				Imeasy::height = Imeasy::rh = (UINT)HIWORD(lp);
				return 0;
			}
			if (msg == WM_CLOSE)
				if (Imeasy::TerminateOnClose) {
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
				&Imeasy::pdxDev,
				NULL,
				&Imeasy::pdxDevCtx
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

			hr = factory->CreateSwapChain(Imeasy::pdxDev, &pdDx, &Imeasy::pswCh);
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
			HRESULT hr = Imeasy::pswCh->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&bBuffer));
			if (hr != S_OK) {
				cout << "Cannot get the back buffer from swap chain" << endl;
				PrintErrorMessage(hr);
				return false;
			}

			hr = Imeasy::pdxDev->CreateRenderTargetView(bBuffer, NULL, &Imeasy::pRTV);
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