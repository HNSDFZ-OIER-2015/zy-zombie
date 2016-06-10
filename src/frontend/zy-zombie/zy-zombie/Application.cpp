#include"Application.h"
#include<iostream>

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib, "Windowscodecs.lib" )
using namespace App::Event;
using namespace D2D1;
namespace App {
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
	const float caption_height = 32.07f;
	const float close_button_width = 37.0f;
	typedef int Direction;


	class ApplicationHelper {
	public:
		static void ApplicationKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam);
		static void ApplicationKeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam);
		static void ApplicationMouseButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam, Direction direction);
		static void ApplicationMouseButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam, Direction direction);
		static void ApplicationMouseWheelMove(HWND hWnd, WPARAM wParam, LPARAM lParam);
		static void ApplicationMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam);
		static void ApplicationClose(HWND hWnd, WPARAM wParam, LPARAM lParam);
		static void ApplicationResize(HWND hWnd, WPARAM wParam, LPARAM lParam);
		static void ApplicationShow(HWND hWnd, WPARAM wParam, LPARAM lParam);
		static void ApplicationHide(HWND hWnd, WPARAM wParam, LPARAM lParam);
		static void ApplicationGetFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);
		static void ApplicationLostFocus(HWND hWnd, WPARAM wParam, LPARAM lParam);

		static void ApplicationCreateDefaultBrush(Application* &App);
		static void ApplicationCreateSwapChainAndRenderTarget(Application* &App);

		static void ApplicationCloseWindowsUI(Application* App);
	};

	class ApplicationManger {
	private:
		vector<Application*> app;
		ComPtr<ID3D11Device> a_Device;
		ComPtr<ID2D1Factory> a_Factory;
		ComPtr<ID3D11DeviceContext> a_DeviceContext;
		ComPtr<IWICImagingFactory> a_IwicFactory;
		ComPtr<IDWriteFactory> a_WriteFactory;
		friend class Application;
		friend class ApplicationHelper;
	public:
		ApplicationManger();
		~ApplicationManger();
		void push_back(Application* App);
		void pop();
		void erase(Application* App);
		auto get(HWND Hwnd)->Application*;
		auto getDevice()->ID3D11Device*;
		auto getFactory()->ID2D1Factory*;
		auto getIwicFactory()->IWICImagingFactory*;	
		auto getDeviceContext()->ID3D11DeviceContext*;
	};




	ApplicationManger::ApplicationManger() {
		app.clear();


		CoInitialize(NULL);
		CoCreateInstance(
			CLSID_WICImagingFactory,
			nullptr,
			CLSCTX_INPROC,
			IID_IWICImagingFactory,
			(void **)(&a_IwicFactory));

		D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, (ID2D1Factory**)&a_Factory);
		

		D3D_FEATURE_LEVEL Features[3] = {
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_10_1,
				D3D_FEATURE_LEVEL_10_0 };
		D3D_FEATURE_LEVEL MyFeature;
		D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_BGRA_SUPPORT, Features, 3,
			D3D11_SDK_VERSION, &a_Device, &MyFeature, &a_DeviceContext);

		
		DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
			__uuidof(IDWriteFactory),
			(IUnknown**)(&a_WriteFactory)
		);
	}

	ApplicationManger::~ApplicationManger() {
		a_Device.ReleaseAndGetAddressOf();
		a_DeviceContext.ReleaseAndGetAddressOf();
		a_Factory.ReleaseAndGetAddressOf();
		a_IwicFactory.ReleaseAndGetAddressOf();
	}

	void ApplicationManger::push_back(Application* App) {
		app.push_back(App);
		ApplicationHelper::ApplicationCreateSwapChainAndRenderTarget(App);
	}

	void ApplicationManger::pop() {
	
		app.pop_back();
	}

	void ApplicationManger::erase(Application* App) {
	
		int size = app.size();
		for (int i = 0; i <= size - 1; i++) {
			if (app[i] == App) {
				app.erase(app.begin() + i);
				return;
			}
		}
	}

	auto ApplicationManger::get(HWND Hwnd)->Application* {

		int size = app.size();
		for (int i = 0; i <= size - 1; i++) {
			if (app[i]->GetHwnd() == Hwnd) {
				return app[i];
			}
		}
	
		return NULL;
	}

	auto ApplicationManger::getDevice()->ID3D11Device* {
		return a_Device.Get();
	}

	auto ApplicationManger::getFactory()->ID2D1Factory* {
		return a_Factory.Get();
	}
	
	auto ApplicationManger::getIwicFactory()->IWICImagingFactory* {
		return a_IwicFactory.Get();
	}

	auto ApplicationManger::getDeviceContext()->ID3D11DeviceContext* {
		return a_DeviceContext.Get();
	}


	static ApplicationManger AppManger;


	void ApplicationHelper::ApplicationKeyDown(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		Application* App = AppManger.get(hWnd);
		ApplicationKeyBoardEvent* EventMsg = new ApplicationKeyBoardEvent();
		ApplicationEvent EventArg;
		EventMsg->key = (Keycode)wParam;
		EventMsg->state = DOWN;
		EventArg.eventarg = EventMsg;
		EventArg.type = ApplicationEventType::APPLICATION_EVENT_BOARD_CLICK;
		App->w_eventqueue.push(EventArg);
	

	}

	void ApplicationHelper::ApplicationKeyUp(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		Application* App = AppManger.get(hWnd);
		ApplicationKeyBoardEvent* EventMsg = new ApplicationKeyBoardEvent();
		ApplicationEvent EventArg;
		EventMsg->key = (Keycode)wParam;
		EventMsg->state = UP;
		EventArg.eventarg = EventMsg;
		EventArg.type = ApplicationEventType::APPLICATION_EVENT_BOARD_CLICK;
		App->w_eventqueue.push(EventArg);

	}

	void ApplicationHelper::ApplicationMouseButtonDown(HWND hWnd, WPARAM wParam, LPARAM lParam, Direction direction) {
		Application* App = AppManger.get(hWnd);
		if (LOWORD(lParam) <= App->w_width &&
			HIWORD(lParam) <= App->w_height&&
			LOWORD(lParam) >= 0 &&
			HIWORD(lParam) >= 0) {
			ApplicationMouseClickEvent* EventMsg = new ApplicationMouseClickEvent();
			ApplicationEvent EventArg;
			switch (direction)
			{
			case LEFT: {
				EventMsg->button = ApplicationMouseButton::BUTTON_LEFT;
				break;
			}
			case MIDDLE: {
				EventMsg->button = ApplicationMouseButton::BUTTON_MIDDLE;
				break;
			}
			case RIGHT: {
				EventMsg->button = ApplicationMouseButton::BUTTON_RIGHT;
				break;
			}
			default:
				break;
			}
			EventMsg->x = LOWORD(lParam);
			EventMsg->y = HIWORD(lParam);
			EventMsg->state = DOWN;
			EventArg.eventarg = EventMsg;
			EventArg.type = ApplicationEventType::APPLICATION_EVENT_MOUSE_CLICK;
			App->w_eventqueue.push(EventArg);
			App->w_mouse_x = LOWORD(lParam);
			App->w_mouse_y = HIWORD(lParam);
			return;
		}
	}

	void ApplicationHelper::ApplicationMouseButtonUp(HWND hWnd, WPARAM wParam, LPARAM lParam, Direction direction) {
		Application* App = AppManger.get(hWnd);
		if (LOWORD(lParam) <= App->w_width &&
			HIWORD(lParam) <= App->w_height&&
			LOWORD(lParam) >= 0 &&
			HIWORD(lParam) >= 0) {
			ApplicationMouseClickEvent* EventMsg = new ApplicationMouseClickEvent();
			ApplicationEvent EventArg;
			switch (direction)
			{
			case LEFT: {
				EventMsg->button = ApplicationMouseButton::BUTTON_LEFT;
				break;
			}
			case MIDDLE: {
				EventMsg->button = ApplicationMouseButton::BUTTON_MIDDLE;
				break;
			}
			case RIGHT: {
				EventMsg->button = ApplicationMouseButton::BUTTON_RIGHT;
				break;
			}
			default:
				break;
			}
			EventMsg->x = LOWORD(lParam);
			EventMsg->y = HIWORD(lParam);
			EventMsg->state = UP;
			EventArg.eventarg = EventMsg;
			EventArg.type = ApplicationEventType::APPLICATION_EVENT_MOUSE_CLICK;
			App->w_eventqueue.push(EventArg);
			App->w_mouse_x = LOWORD(lParam);
			App->w_mouse_y = HIWORD(lParam);
			return;
		}
	}

	void ApplicationHelper::ApplicationMouseWheelMove(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		Application* App = AppManger.get(hWnd);
		ApplicationMouseWheelEvent* EventMsg = new ApplicationMouseWheelEvent();
		ApplicationEvent EventArg;
		EventMsg->offest = GET_WHEEL_DELTA_WPARAM(wParam) / 120;
		EventArg.eventarg = EventMsg;
		EventArg.type = ApplicationEventType::APPLICATION_EVENT_MOUSE_WHEEL;

		App->w_eventqueue.push(EventArg);


	}

	void ApplicationHelper::ApplicationMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam) {

		Application* App = AppManger.get(hWnd);
		if (LOWORD(lParam) <= App->w_width &&
			HIWORD(lParam) <= App->w_height&&
			LOWORD(lParam) >= 0 &&
			HIWORD(lParam) >= 0) {
			ApplicationMouseMoveEvent* EventMsg = new ApplicationMouseMoveEvent();
			ApplicationEvent EventArg;
			EventMsg->last_x = App->w_mouse_x;
			EventMsg->last_y = App->w_mouse_y;
			EventMsg->x = LOWORD(lParam);
			EventMsg->y = HIWORD(lParam);
			EventArg.eventarg = EventMsg;
			EventArg.type = ApplicationEventType::APPLICATION_EVENT_MOUSE_MOVE;

			App->w_eventqueue.push(EventArg);
			App->w_mouse_x = LOWORD(lParam);
			App->w_mouse_y = HIWORD(lParam);
			return;
		}
	}

	void ApplicationHelper::ApplicationClose(HWND hWnd, WPARAM wParam, LPARAM lParam) {

		Application* App = AppManger.get(hWnd);
		App->Close();

	}

	void ApplicationHelper::ApplicationResize(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		Application* App = AppManger.get(hWnd);
	}

	void ApplicationHelper::ApplicationShow(HWND hWnd, WPARAM wParam, LPARAM lParam) {

		Application* App = AppManger.get(hWnd);

	}

	void ApplicationHelper::ApplicationHide(HWND hWnd, WPARAM wParam, LPARAM lParam) {

		Application* App = AppManger.get(hWnd);

	}

	void ApplicationHelper::ApplicationGetFocus(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		Application* App = AppManger.get(hWnd);
		App->w_is_focus = true;
	}

	void ApplicationHelper::ApplicationLostFocus(HWND hWnd, WPARAM wParam, LPARAM lParam) {
		Application* App = AppManger.get(hWnd);
		App->w_is_focus = false;
	}

	void ApplicationHelper::ApplicationCreateDefaultBrush(Application* &App) {
	}

	void ApplicationHelper::ApplicationCreateSwapChainAndRenderTarget(Application* &App) {
		ID3D11Device* Device = AppManger.getDevice();
		
		RECT rc;
		GetClientRect(App->w_hwnd, &rc);
		UINT width = rc.right - rc.left;
		UINT height = rc.bottom - rc.top;

		UINT MSAA4xQuality(0);
		Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &MSAA4xQuality);
		DXGI_SWAP_CHAIN_DESC sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.BufferCount = 1;
		sd.BufferDesc.Width = width;
		sd.BufferDesc.Height = height;
		sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 60;
		sd.BufferDesc.RefreshRate.Denominator = 1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.OutputWindow = App->w_hwnd;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.Windowed = TRUE;
		IDXGIDevice *DXGIDevice(NULL);
		Device->QueryInterface(__uuidof(IDXGIDevice), reinterpret_cast<void**>(&DXGIDevice));
		IDXGIAdapter *DXGIAdapter(NULL);
		DXGIDevice->GetParent(__uuidof(IDXGIAdapter), reinterpret_cast<void**>(&DXGIAdapter));
		IDXGIFactory *DXGIFactory(NULL);
		DXGIAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&DXGIFactory));
		DXGIFactory->CreateSwapChain(Device, &sd, &App->w_swapchain);

		DXGIFactory->Release();
		DXGIAdapter->Release();

		ID3D11Texture2D *BackBuffer(NULL);
		App->w_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&BackBuffer));
		Device->CreateRenderTargetView(BackBuffer, NULL, &App->w_rendertargetview);

		BackBuffer->Release();


		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory(&descDepth, sizeof(descDepth));
		descDepth.Width = width;
		descDepth.Height = height;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		ID3D11Texture2D *DepthStencilBuffer(NULL);


		Device->CreateTexture2D(&descDepth, NULL, &DepthStencilBuffer);
		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;

		Device->CreateDepthStencilView(DepthStencilBuffer, &descDSV, &App->w_depthstencilview);
		AppManger.a_DeviceContext->OMSetRenderTargets(1, &App->w_rendertargetview, App->w_depthstencilview);

		DepthStencilBuffer->Release();

		D3D11_VIEWPORT ViewPort = { 0 };
		ViewPort.Width = (FLOAT)(App->w_width);
		ViewPort.Height = (FLOAT)(height);
		ViewPort.MinDepth = 0.f;
		ViewPort.MaxDepth = 1.f;
		ViewPort.TopLeftX = 0.f;
		ViewPort.TopLeftY = 0.f;
		AppManger.a_DeviceContext->RSSetViewports(1, &ViewPort);



		IDXGISurface* Surface;
		App->w_swapchain->GetBuffer(0, IID_PPV_ARGS(&Surface));


		FLOAT dpiX;
		FLOAT dpiY;
		AppManger.a_Factory->GetDesktopDpi(&dpiX, &dpiY);

		D2D1_RENDER_TARGET_PROPERTIES Props =
			RenderTargetProperties(
				D2D1_RENDER_TARGET_TYPE_DEFAULT,
				PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED),
				dpiX,
				dpiY);

		AppManger.a_Factory->CreateDxgiSurfaceRenderTarget(Surface, &Props, &App->w_rendertarget);

		ApplicationHelper::ApplicationCreateDefaultBrush(App);
	}

	void ApplicationHelper::ApplicationCloseWindowsUI(Application* App) {
		long style;
		style = ::GetWindowLong(App->w_hwnd, GWL_STYLE);
		style = (style &~WS_SIZEBOX);
		//style = (style &~WS_CAPTION);
		style = (style &~WS_MAXIMIZEBOX);
		//style = (style &~WS_SYSMENU);
		::SetWindowLong(App->w_hwnd, GWL_STYLE, style);
	}


	






	LRESULT CALLBACK ApplicationWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
		case WM_KEYDOWN: {
			ApplicationHelper::ApplicationKeyDown(hWnd, wParam, lParam);
			break;
		}
		case WM_KEYUP: {
			ApplicationHelper::ApplicationKeyUp(hWnd, wParam, lParam);
			break;
		}
		case WM_LBUTTONDOWN: {
			ApplicationHelper::ApplicationMouseButtonDown(hWnd, wParam, lParam, LEFT);
			SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
			break;
		}
		case WM_MBUTTONDOWN: {
			ApplicationHelper::ApplicationMouseButtonDown(hWnd, wParam, lParam, MIDDLE);
			break;
		}
		case WM_RBUTTONDOWN: {
			ApplicationHelper::ApplicationMouseButtonDown(hWnd, wParam, lParam, RIGHT);
			break;
		}
		case WM_LBUTTONUP: {
			ApplicationHelper::ApplicationMouseButtonUp(hWnd, wParam, lParam, LEFT);
			break;
		}
		case WM_MBUTTONUP: {
			ApplicationHelper::ApplicationMouseButtonUp(hWnd, wParam, lParam, MIDDLE);
			break;
		}
		case WM_RBUTTONUP: {
			ApplicationHelper::ApplicationMouseButtonUp(hWnd, wParam, lParam, RIGHT);
			break;
		}
		case WM_MOUSEWHEEL: {
			ApplicationHelper::ApplicationMouseWheelMove(hWnd, wParam, lParam);
			break;
		}
		case WM_MOUSEMOVE: {
			ApplicationHelper::ApplicationMouseMove(hWnd, wParam, lParam);
			break;
		}
		case WM_DESTROY: {
			ApplicationHelper::ApplicationClose(hWnd, wParam, lParam);
			break;
		}
		case WM_SIZE: {
			ApplicationHelper::ApplicationResize(hWnd, wParam, lParam);
			break;
		}
		case WM_SHOWWINDOW: {
			if (wParam)
				ApplicationHelper::ApplicationShow(hWnd, wParam, lParam);
			else ApplicationHelper::ApplicationHide(hWnd, wParam, lParam);
			break;
		}
		case WM_SETFOCUS: {
			ApplicationHelper::ApplicationGetFocus(hWnd, wParam, lParam);
			break;
		}
		case WM_KILLFOCUS: {
			ApplicationHelper::ApplicationLostFocus(hWnd, wParam, lParam);
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}






	
	Application::Application() {
		w_ison = false;
		w_isvalid = false;
		w_mouse_x = 0;
		w_mouse_y = 0;

	}

	Application::Application(
		const int height,
		const int width,
		const wchar_t* title,
		const wchar_t* ico,
		const wchar_t* AppName) {

		w_height = height;
		w_width = width;
		w_title = title;
		w_ico = ico;
		w_AppName = AppName;
		w_isvalid = true;
		w_ison = false;
		w_mouse_x = 0;
		w_mouse_y = 0;

		

		if (AppName) {
			w_AppName = AppName;
		}
		else w_AppName = w_title;

		w_hinstance = ::GetModuleHandle(NULL);

		WNDCLASS WindowClass;

		WindowClass.style = CS_HREDRAW | CS_VREDRAW;
		WindowClass.lpfnWndProc = ApplicationWndProc;
		WindowClass.cbClsExtra = 0;
		WindowClass.cbWndExtra = 0;
		WindowClass.hInstance = w_hinstance;
		WindowClass.hIcon = (HICON)LoadImage(0, w_ico, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
		WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		WindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		WindowClass.lpszMenuName = NULL;
		WindowClass.lpszClassName = w_AppName;

		RegisterClass(&WindowClass);

		RECT rc;
		rc.top = 0;
		rc.left = 0;
		rc.right = width;
		rc.bottom = height;

		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		w_hwnd = CreateWindow(w_AppName, w_title, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, rc.right-rc.left, rc.bottom-rc.top, nullptr, nullptr, w_hinstance, nullptr);

		
		
		

		
		ApplicationHelper::ApplicationCloseWindowsUI(this);

		
		w_msg = { 0 };

		w_msg.hwnd = w_hwnd;

		AppManger.push_back(this);


		

	}

	Application::~Application() {

	}

	void Application::CreateSelf(
		const int height,
		const int width,
		const wchar_t* title,
		const wchar_t* ico,
		const wchar_t* AppName) {


		w_height = height;
		w_width = width;
		w_title = title;
		w_ico = ico;
		w_isvalid = true;
		w_ison = false;

		if (AppName) {
			w_AppName = AppName;
		}
		else w_AppName = w_title;

		w_hinstance = ::GetModuleHandle(NULL);

		WNDCLASS WindowClass;


		WindowClass.style = CS_HREDRAW | CS_VREDRAW;
		WindowClass.lpfnWndProc = ApplicationWndProc;
		WindowClass.cbClsExtra = 0;
		WindowClass.cbWndExtra = 0;
		WindowClass.hInstance = w_hinstance;
		WindowClass.hIcon = (HICON)LoadImage(0, w_ico, IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
		WindowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		WindowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
		WindowClass.lpszMenuName = NULL;
		WindowClass.lpszClassName = w_AppName;

		RegisterClass(&WindowClass);

		RECT rc;
		rc.top = 0;
		rc.left = 0;
		rc.right = width;
		rc.bottom = height;

		AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

		w_hwnd = CreateWindow(w_AppName, w_title, WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, 0, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, w_hinstance, nullptr);
		

		

	    ApplicationHelper::ApplicationCloseWindowsUI(this);

		w_msg = { 0 };

		w_msg.hwnd = w_hwnd;

		AppManger.push_back(this);



	}

	void Application::AddEvent(ApplicationEventType Type, ApplicationCallBack Function) {
		w_function[Type].push_back(Function);

	
	}

	void Application::DeleteEvent(ApplicationEventType Type) {
		w_function[Type].clear();

	}

	void Application::Push_Event(ApplicationEvent Event) {
		w_eventqueue.push(Event);
	}

	void Application::Resize(int width, int height) {
		SetWindowPos(w_hwnd, NULL, 0, 0, width, height, SWP_NOMOVE);

	}

	bool Application::Run() {
		bool re = false;
		POINT MousePos;
		GetCursorPos(&MousePos);
		ScreenToClient(w_hwnd, &MousePos);
		w_mouse_x = MousePos.x;
		w_mouse_y = MousePos.y;
		if (PeekMessage(&w_msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&w_msg);
			DispatchMessage(&w_msg);
		}
		else re = true;
		while (!w_eventqueue.empty()) {
			ApplicationEvent EventArg = w_eventqueue.front();
			int type = EventArg.type;
			int size = w_function[type].size();
			for (int i = 0; i <= size - 1; i++) {
				w_function[type][i](this, EventArg.eventarg);
			}
			delete EventArg.eventarg;
			w_eventqueue.pop();
		}
		return re;
	}

	void Application::Show() {
		w_ison = true;
		ShowWindow(w_hwnd, SW_SHOW);

	}

	void Application::Hide() {
		w_ison = false;
		ShowWindow(w_hwnd, SW_HIDE);

	}

	void Application::Close() {

		w_ison = false;
		w_isvalid = false;
		PostQuitMessage(0);

	}

	void Application::Present() {
		w_swapchain->Present(0, 0);
	}

	auto Application::GetHwnd()->HWND {
		return w_hwnd;
	}

	auto Application::GetHeight()->int {
		return w_height;
	}

	auto Application::GetWidth()->int {
		return w_width;
	}

	auto Application::IsShow()->bool {
		return w_ison;
	}

	auto Application::IsHide()->bool {
		return !w_ison;
	}

	auto Application::IsValid()->bool {
		return w_isvalid;
	}

	auto Application::GetKeyState(Keycode Code)->bool {
		if (KEYDOWN((int)Code)) return DOWN;
		else return UP;
	}


	auto GetDevice() -> ID3D11Device *{
		return AppManger.getDevice();
	}

	auto GetDeviceContext() -> ID3D11DeviceContext *{
		return AppManger.getDeviceContext();
	}

	auto GetFactory() -> ID2D1Factory *{
		return AppManger.getFactory();
	}

	auto GetIWICFactory() -> IWICImagingFactory *{
		return AppManger.getIwicFactory();
	}

}