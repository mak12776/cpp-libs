#pragma once

#include <cstdint>

#include "types.h"
#include "math.h"
#include "rand.h"
#include "error.h"

namespace scl
{
	namespace pxld
	{
		typedef size_t unit_t;			/* this can also be signed */
		typedef double funit_t;			/* type of fpoint_t members */
		typedef uint32_t color_t;		/* type of image_t data pointer is unsigned */

#define SCL_PXLD_IS_UNIT_SIGNED 0

		const bool is_unit_signed = false;
		const uint8_t unit_width = sizeof(unit_t);

		typedef uint8_t mode_t;

		namespace colors
		{
			const color_t RGB_WHITE = 0xFFFFFF;
			const color_t RGB_BLACK = 0x000000;

			static inline color_t make_L(unit_t l)
			{
				return (color_t)l;
			}

			static inline color_t make_LA(uint8_t l, uint8_t a)
			{
				return ((color_t)l << 8) | ((color_t)a);
			}

			static inline color_t make_RGB(uint8_t r, uint8_t g, uint8_t b)
			{
				return ((color_t)r << 16) | ((color_t)g << 8) | ((color_t)b);
			}

			static inline color_t make_RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			{
				return ((color_t)r << 24) | ((color_t)g << 16) | ((color_t)b << 8) | ((color_t)a);
			}
		}

		namespace mode
		{
			const mode_t L = 0;
			const mode_t LA = 1;
			const mode_t RGB = 2;
			const mode_t RGBA = 3;

			static inline bool is_valid(mode_t mode)
			{
				switch (mode)
				{
				case L:
				case LA:
				case RGB:
				case RGBA:
					return true;
				default:
					return false;
				}
			}
			
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

		typedef struct
		{
			funit_t x;
			funit_t y;
		} fpoint_t;

		namespace point
		{
			const point_t ZERO = { 0, 0 };

			static inline void random(point_t *point, point_t min, point_t max)
			{
				point->x = rand::next_int<unit_t>(min.x, max.x);
				point->y = rand::next_int<unit_t>(min.y, max.y);
			}

			static inline double distance(point_t a, point_t b)
			{
				return std::sqrt(std::pow(a.x - b.x, 2.0) + std::pow(a.y - b.y, 2.0));
			}

			static inline double inverse_distance(point_t a, point_t b, double max)
			{
				return max - std::fmin(distance(a, b), max);
			}

			static inline double inverse_distance_fraction(point_t a, point_t b, double max)
			{
				return inverse_distance(a, b, max) / max;
			}

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
			size_t area;

			unit_t color_width;
			unit_t x_width;
			unit_t y_width;
		} image_t;

		namespace _core
		{
			template <typename T>
			static inline void draw_color(image_t *image, color_t color)
			{
				T *data = (T *)image->data;

				for (unit_t index = 0; index < image->area; index += 1)
				{
					data[index] = color;
				}
			}

			static inline void draw_color(image_t *image, color_t color)
			{
				color_t temp_color;
				for (unit_t index = 0; index < image->y_width; index += image->color_width)
				{
					temp_color = color;

					for (unit_t c_offset = image->color_width; c_offset > 0;)
					{
						c_offset -= 1;

						image->data[index + c_offset] = temp_color & 0xFF;
						temp_color >>= 8;
					}
				}
			}

			template <typename T>
			static inline void draw_point(image_t *image, point_t point, color_t color)
			{
				T *data = (T *)image->data;

				data[(point.y * image->size.x) + point.y] = color;
			}

			static inline void draw_point(image_t *image, point_t point, color_t color)
			{
				unit_t y_offset = point.y * image->x_width;
				unit_t x_offset = point.x * image->color_width;

				for (unit_t c_offset = image->color_width; c_offset > 0; )
				{
					c_offset -= 1;

					image->data[y_offset + x_offset + c_offset] = color & 0xFF;
					color >>= 8;
				}
			}

			template <typename T>
			static inline void draw(image_t *image, color_t(*func) (point_t))
			{
				T *data = (T *)image->data;

				for (unit_t y = 0, y_offset = 0; y < image->size.y; y += 1, y_offset += image->size.x)
				{
					for (unit_t x = 0; x < image->size.x; x += 1)
					{
						data[y_offset + x] = func(point_t{ x, y });
					}
				}
			}

			static inline void draw(image_t *image, color_t(*func) (point_t))
			{
				color_t color;

				for (unit_t y_offset = 0, y = 0; y_offset < image->y_width; y_offset += image->x_width, y += 1)
				{
					for (unit_t x_offset = 0, x = 0; x_offset < image->x_width; x_offset += image->color_width, x += 1)
					{
						color = func(point_t{ x, y });

						for (unit_t c_offset = image->color_width; c_offset > 0;)
						{
							c_offset -= 1;

							image->data[y_offset + x_offset + c_offset] = color & 0xFF;
							color <<= 8;
						}
					}
				}
			}

			template <typename T>
			static inline void map(image_t *image, color_t(*func) (point_t, color_t))
			{
				T *data = (T *)image->data;

				for (unit_t y = 0, y_offset = 0; y < image->size.y; y += 1, y_offset += image->size.x)
				{
					for (unit_t x = 0; x < image->size.x; x += 1)
					{
						data[y_offset + x] = func(point_t{ x, y }, (color_t)data[y_offset + x]);
					}
				}
			}

			static inline void map(image_t *image, color_t(*func) (point_t, color_t))
			{
				color_t color;
				for (unit_t y_offset = 0, y = 0; y_offset < image->x_width; y_offset += image->x_width, y += 1)
				{
					for (unit_t x_offset = 0, x = 0; x_offset < image->color_width; x_offset += image->color_width, x += 1)
					{
						color = 0;

						for (unit_t c_offset = image->color_width; c_offset > 0;)
						{
							c_offset -= 1;

							color <<= 8;
							color |= image->data[y_offset + x_offset + c_offset];
						}

						color = func(point_t{ x, y }, color);

						for (unit_t c_offset = image->color_width; c_offset > 0;)
						{
							c_offset -= 1;

							image->data[y_offset + x_offset + c_offset] = color & 0xFF;
							color >>= 8;
						}
					}
				}
			}
		}

		namespace image
		{
			static inline void random_point(image_t *image, point_t *point)
			{
				point::random(point, point::ZERO, image->size);
			}

			static inline void init(image_t *image, point_t size, mode_t mode)
			{
				unit_t area;
				unit_t color_width;
				unit_t x_width;
				unit_t y_width;

				color_width = mode::get_width(mode);
				if (error::num) return;

				math::safe_mul_size(size.x, color_width, x_width);
				if (error::num) return;

				math::safe_mul_size(size.y, x_width, y_width);
				if (error::num) return;

				math::safe_mul_size(size.x, size.y, area);
				if (error::num) return;

				image->data = memory::new_array<byte>(y_width);
#ifdef SCL_CATCH_EXCEPTIONS
				if (image->data == nullptr) return;
#endif

				image->mode = mode;
				image->size = size;
				image->area = area;

				image->color_width = color_width;
				image->x_width = x_width;
				image->y_width = y_width;
			}

			static inline void end(image_t *image)
			{
				delete[] image->data;
			}

			static inline image_t *new_image(point_t size, mode_t mode)
			{
				image_t *result;

				result = new image_t;
				init(result, size, mode);
				if (error::num)
					delete result;

				return result;
			}

			static inline void delete_image(image_t *image)
			{
				end(image);
				delete image;
			}

			static inline void draw_color(image_t *image, color_t color)
			{
				switch (image->color_width)
				{
				case 1: _core::draw_color<uint8_t>(image, color); break;
				case 2: _core::draw_color<uint16_t>(image, color); break;
				case 3: _core::draw_color(image, color); break;
				case 4: _core::draw_color<uint32_t>(image, color); break;
				default: error::set_error_bad_argument("draw_color", "image->color_width"); break;
				}
			}

			static inline void draw_point(image_t *image, point_t point, color_t color)
			{
				switch (image->color_width)
				{
				case 1: _core::draw_point<uint8_t>(image, point, color); break;
				case 2: _core::draw_point<uint16_t>(image, point, color); break;
				case 3: _core::draw_point(image, point, color); break;
				case 4: _core::draw_point<uint32_t>(image, point, color); break;
				default: error::set_error_bad_argument("draw_point", "image->color_width"); break;
				}
			}

			static inline void draw(image_t *image, color_t(*func) (point_t))
			{
				switch (image->color_width)
				{
				case 1: _core::draw<uint8_t>(image, func); break;
				case 2: _core::draw<uint16_t>(image, func); break;
				case 3: _core::draw(image, func); break;
				case 4: _core::draw<uint32_t>(image, func); break;
				default: error::set_error_bad_argument("draw", "image->color_width"); break;
				}
			}

			static inline void map(image_t *image, color_t(*func) (point_t, color_t))
			{
				switch (image->color_width)
				{
				case 1: _core::map<uint8_t>(image, func); break;
				case 2: _core::map<uint16_t>(image, func); break;
				case 3: _core::map(image, func); break;
				case 4: _core::map<uint32_t>(image, func); break;
				default: error::set_error_bad_argument("map", "image->color_width"); break;
				}
			}

			static inline size_t save(image_t *image, FILE *file, const bool safe_result = false)
			{
				size_t byte_number;

				byte_number = io::safe_write((void *)"img.", 4, file);
				if (error::num) return byte_number;

				byte_number += io::safe_write((void *)&unit_width, 1, file);
				if (error::num) return byte_number;

				byte_number += io::safe_write(&(image->color_width), sizeof(unit_t), file);
				if (error::num) return byte_number;

				byte_number += io::safe_write(&(image->size.x), sizeof(unit_t), file);
				if (error::num) return byte_number;

				byte_number += io::safe_write(&(image->size.y), sizeof(unit_t), file);
				if (error::num) return byte_number;

				byte_number += io::safe_write(image->data, image->y_width, file);
				if (error::num) return byte_number;

				return byte_number;
			}

			static inline size_t save(image_t *image, const char *name)
			{
				FILE *file;
				size_t byte_number;

				file = scl::io::safe_fopen(name, "wb");
				if (error::num) return 0;

				byte_number = save(image, file);
				fclose(file);

				return byte_number;
			}
		}

		typedef struct animation
		{
			image_t *image;
			void(*start) (image_t *image);
			void(*step) (image_t *image);
		} animation_t;
	}
}