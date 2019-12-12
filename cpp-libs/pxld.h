#pragma once

#include <cstdint>
#include <intrin.h>

#include "types.h"
#include "math.h"

#include "error.h"

namespace scl
{
	namespace pxld
	{
		namespace mode
		{
			typedef uint8_t mode_t;

			const mode_t L = 1;
			const mode_t LA = 2;
			const mode_t RGB = 3;
			const mode_t RGBA = 4;

			static inline size_t get_width(mode_t mode)
			{
				switch (mode)
				{
				case L: return 1;
				case LA: return 2;
				case RGB: return 3;
				case RGBA: return 4;
				default:
					error::set_error_bad_argument("get_width", "mode");
					return 0;
				}
			}

			static inline const char *get_string(mode_t mode)
			{
				switch (mode)
				{
				case L: return "L";
				case LA: return "LA";
				case RGB: return "RGB";
				case RGBA: return "RGBA";
				default:
					error::set_error_bad_argument("get_string", "mode");
					return 0;
				}
			}
		}
		namespace point
		{
			typedef struct
			{
				size_t x;
				size_t y;
			} point_t;
		}

		namespace image
		{
			typedef struct
			{
				byte *data;

				mode::mode_t mode;
				point::point_t size;

				size_t color_width;
				size_t x_width;
				size_t y_wdith;
			} image_t;

			static inline void initialize(image_t *image, point::point_t size, mode::mode_t mode)
			{
				size_t color_width;
				size_t x_width;
				size_t y_width;

				color_width = mode::get_width(mode);
				if (error::num) return;

				math::safe_mul_size(size.x, color_width, x_width);
				if (error::num) return;

				math::safe_mul_size(size.y, x_width, y_width);
				if (error::num) return;

#ifdef SCL_CATCH_EXCEPTIONS
				try
				{
					image->data = new byte[image->y_wdith];
				}
				catch (std::bad_alloc &exception)
				{
					error::set_error_bad_alloc_exception("byte", image->y_wdith);
					return;
				}
#endif
				
				image->mode = mode;
				image->color_width = color_width;
				image->x_width = x_width;
				image->y_wdith = y_width;
			}
		}
		
	}
}