#pragma once
//the header need riteme remade
#include<d3d11.h>
#include<d2d1_1.h>
#include<dwrite.h>
#include<wincodecsdk.h>
#include"Application.h"
#include"zy_type.h"
using namespace App;
namespace zy {
#define Windows 

	class Image {
	private:
		ID2D1Bitmap* bitmap;
		char* filename;
	public:
		Image(char* filename);
		~Image();
		auto GetWidth()->float; //get Width
		auto GetHeight()->float; //get height
#ifdef Windows
		operator ID2D1Bitmap*();
#endif // Windows
	};

	class Render {
	private:
		IDXGISwapChain*     g_swapchain;
		ID2D1Factory*       g_factory;
		ID2D1RenderTarget*  g_render;
		IWICImagingFactory* g_imagefac;


		float               g_angle;
		float               g_center_x;
		float				g_center_y;
		float               g_scale_x;
		float               g_scale_y;
		float               g_scale_center_x;
		float               g_scale_center_y;
		Matrix3x2F          g_scale;
		Matrix3x2F          g_rotate;

		void UpDataMatrix();
	public:
		void Bind(Application* App);
		void BeginDraw(float4 color = float4(1.f, 1.f, 1.f, 1.f));
		void EndDraw();
		void Present();

		//basic message
		void SetDrawAngle(float Angle);
		void SetDrawScale(float x, float y); //do not support drawimage
		void SetDrawScaleCenter(float x, float y); //do not support drawimage
		void SetDrawRotateCenter(float x, float y);

		//Draw
		void DrawImage(Image DrawImage, RECT ImageRect);//

		//Resource
		void CreateImage(Image* Image); //create iamge




	};
}