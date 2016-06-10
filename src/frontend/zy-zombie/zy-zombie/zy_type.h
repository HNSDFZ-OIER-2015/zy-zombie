#pragma once

namespace zy {
	struct RECT {
		float left, right, top, bottom;
		RECT() {

		}
		RECT(float left, float right, float top, float bottom) {
			this->left = left;
			this->right = right;
			this->top = top;
			this->bottom = bottom;
		}
	};


	struct float4 {
		float x, y, z, w;
		float4(float x, float y, float z, float w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}
		float4() {

		}
	};
}

