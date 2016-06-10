#pragma once
//the header need riteme remade
#include<wrl.h>
#include<queue>
#include<vector>
#include<d2d1.h>
#include<d3d11.h>
#include<dwrite.h>
#include<Windows.h>
#include<wincodec.h>
#include<functional>
#include<wrl\client.h>



#include"Application-Keycode.h"
#include"Application-Event.h"


using namespace std;
using namespace D2D1;
using namespace Microsoft::WRL;

using namespace App::Event;
class Game;
class Render;
namespace App {
#define DOWN 1
#define UP 0
#define LEFT 3
#define MIDDLE 4
#define RIGHT 5
#define Theme ColorF::ColorF(ColorF::Red)
#define ButtonFocus ColorF::ColorF(ColorF::Pink)


	class Application {
	public:
		MSG  w_msg; //the message of window
		HWND w_hwnd; //Hwnd
		int  w_height; //height
		int  w_width; //width
		int  w_mouse_x; //mouse x
		int  w_mouse_y; //mouse y	
		bool w_ison; //is open and show
		bool w_isvalid; //is create or close
		bool w_is_focus; //is get focus
		LPCWSTR w_ico;// the ico
		LPCWSTR w_title; //the application title
		LPCWSTR w_AppName; //the wndclass name
		HINSTANCE  w_hinstance; //Hinstance
		vector<ApplicationCallBack> w_function[6]; //the function to deal event
		queue<ApplicationEvent> w_eventqueue; //the queue for deal event and msg
		ComPtr<IDXGISwapChain> w_swapchain;// the swapchain about window 
		ID3D11DepthStencilView* w_depthstencilview; //depth view
		ID3D11RenderTargetView* w_rendertargetview; //render targetview
		ComPtr<ID2D1RenderTarget> w_rendertarget;//the 2d object render
		friend class ApplicationManger;
		friend class ApplicationHelper;
		friend class Game;
		friend class Render;
	public:
		Application();
		Application(
			const int height, //Height
			const int width,  //Width
			const wchar_t* title, //Title
			const wchar_t* ico, //Ico
			const wchar_t* AppName = NULL); //AppName,if not defined we use the title name

		~Application();
		//create it 
		void CreateSelf(
			const int height, //Height
			const int width,  //Width
			const wchar_t* title, //Title
			const wchar_t* ico, //Ico
			const wchar_t* AppName = NULL); //AppName

		//add a event ,we can add more than one event
		void AddEvent(ApplicationEventType Type, ApplicationCallBack Function);
		//delete a type all event 
		void DeleteEvent(ApplicationEventType Type);
		//push a event 
		void Push_Event(ApplicationEvent Event);
		//resize ,change the window's size
		void Resize(int width, int height);
		//run,we should repeat use it ,until the Applicaiton close 
		bool Run();
		//show  the Application
		void Show();
		//hide  the Applicaiton
		void Hide();
		//close the Application
		void Close();
		//Present 
		void Present();


		//get hwnd
		auto GetHwnd()->HWND;
		//you don't need finsih the funciton
		//get height
		auto GetHeight()->int;
		//get width
		auto GetWidth()->int;
		//is show
		auto IsShow()->bool;
		//is hide
		auto IsHide()->bool;
		//is valid
		auto IsValid()->bool;


		


		//get Keycode's state,it is not good(it is same as GetAsyncKeyState(Win API))
		static auto GetKeyState(Keycode Code)->bool;

	};


	auto GetDevice()->ID3D11Device*;

	auto GetDeviceContext()->ID3D11DeviceContext*;

	auto GetFactory()->ID2D1Factory*;

	auto GetIWICFactory()->IWICImagingFactory*;
}