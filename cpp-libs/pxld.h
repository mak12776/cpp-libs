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
		typedef size_t unit_t;
		typedef uint32_t color_t;

		const bool is_unit_signed = false;

		typedef enum
		{
			L, LA, RGB, RGBA
		} mode_t;

		namespace mode
		{
			static inline unit_t get_width(mode_t mode)
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

		typedef struct
		{
			unit_t x;
			unit_t y;
		} point_t;

		namespace point
		{
			static inline point_t *new_point(unit_t x, unit_t y)
			{
				point_t *result = new point_t;
				result->x = x;
				result->y = y;
				return result;
			}

			static inline void delete_point(point_t *point)
			{
				delete point;
			}
		}

		typedef struct
		{
			byte *data;

			mode_t mode;
			point_t size;

			unit_t color_width;
			unit_t x_width;
			unit_t y_width;
		} image_t;

		namespace image
		{
			static inline void initialize(image_t *image, point_t size, mode_t mode)
			{
				unit_t color_width;
				unit_t x_width;
				unit_t y_width;

				color_width = mode::get_width(mode);
				if (error::num) return;

				math::safe_mul_size(size.x, color_width, x_width);
				if (error::num) return;

				math::safe_mul_size(size.y, x_width, y_width);
				if (error::num) return;


				image->data = memory::new_array<byte>(y_width);

#ifdef SCL_CATCH_EXCEPTIONS
				if (image->data == nullptr) return;
#endif
				image->mode = mode;
				image->size = size;
				
				image->color_width = color_width;
				image->x_width = x_width;
				image->y_width = y_width;
			}

			static inline void finalize(image_t *image)
			{
				delete[] image->data;
			}

			static inline void draw(image_t *image, color_t(*func) (point_t))
			{
				color_t color;
				for (unit_t y_offset = 0, y = 0; y_offset < image->y_width; y_offset += image->x_width, y += 1)
				{
					for (unit_t x_offset = 0, x = 0; x_offset < image->x_width; x_offset += image->color_width, x += 1)
					{
						color = func(point_t{ x, y });

						unit_t c_offset = image->color_width;
						while (c_offset > 0)
						{
							c_offset -= 1;

							image->data[y_offset + x_offset + c_offset] = (byte)(color & 0xFF);
							color >>= 8;
						}
					}
				}
			}

			static inline void map(image_t *image, color_t(*func) (point_t, color_t))
			{
				color_t color;
				for (unit_t y_offset = 0, y = 0; y < image->size.y; y_offset += image->x_width, y += 1)
				{
					for (unit_t x_offset = 0, x = 0; x < image->size.x; x_offset += image->color_width, x += 1)
					{
						for (unit_t c_offset = image->color_width - 1; c_offset >= 0; c_offset -= 1)
						{
							image->data[y_offset + x_offset + c_offset] = color & 0xFF;
							color >>= 8;
						}
					}
				}
			}
		}
	}
}