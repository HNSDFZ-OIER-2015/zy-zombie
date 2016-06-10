#include "zy_render.h"
#include "zy_type.h"
#include "Application.h"
using namespace App;
using namespace zy;

template<typename T>
void release(T Interface) {
	if (Interface)
		Interface->Release();
}

namespace zy {
	void zy::Render::UpDataMatrix()
	{
		g_render->SetTransform(g_rotate*g_scale);
	}

	void zy::Render::Bind(Application * App)
	{
		g_factory = App::GetFactory();
		g_imagefac = App::GetIWICFactory();
		g_swapchain = App->w_swapchain.Get();
		g_render = App->w_rendertarget.Get();
	}

	void zy::Render::BeginDraw(float4 color)
	{
		g_render->BeginDraw();
		g_render->Clear(D2D1::ColorF::ColorF(color.x, color.y, color.z, color.w));
	}

	void zy::Render::EndDraw()
	{
		g_render->EndDraw();
	}

	void zy::Render::Present()
	{
		g_swapchain->Present(0, 0);
	}

	void zy::Render::SetDrawAngle(float Angle)
	{
		g_angle = Angle;
		g_rotate = Matrix3x2F::Rotation(g_angle, D2D1::Point2F(g_center_x, g_center_y));
	}

	void zy::Render::SetDrawScale(float x, float y)
	{
		g_scale_x = x;
		g_scale_y = y;
		g_scale = Matrix3x2F::Scale(D2D1::SizeF(g_scale_x, g_scale_y),
			D2D1::Point2F(g_scale_center_x, g_scale_center_y));

	}

	void zy::Render::SetDrawScaleCenter(float x, float y)
	{
		g_scale_center_x = x;
		g_scale_center_y = y;
		g_scale = Matrix3x2F::Scale(D2D1::SizeF(g_scale_x, g_scale_y),
			D2D1::Point2F(g_scale_center_x, g_scale_center_y));
	}

	void zy::Render::SetDrawRotateCenter(float x, float y)
	{
		g_center_x = x;
		g_center_y = y;
		g_rotate = Matrix3x2F::Rotation(g_angle, D2D1::Point2F(g_center_x, g_center_y));
	}

	void zy::Render::DrawImage(Image DrawImage, RECT ImageRect)
	{
		ID2D1Bitmap* bitmap = DrawImage;
		D2D_SIZE_F Size = bitmap->GetSize();
		
		g_render->SetTransform(g_rotate);
		g_render->DrawBitmap(bitmap,
			D2D1::RectF(
			(float)0,
			(float)0,
			(float)0 + Size.width,
			(float)0 + Size.height));
	}


	zy::Image::Image(char * filename)
	{
		this->filename = filename;
	}

	zy::Image::~Image()
	{
		release(this->bitmap);
	}

	auto zy::Image::GetWidth() -> float
	{
		return bitmap->GetSize().width;
	}

	auto zy::Image::GetHeight() -> float
	{
		return bitmap->GetSize().height;
	}

	Image::operator ID2D1Bitmap*()
	{
		return bitmap;
	}
}