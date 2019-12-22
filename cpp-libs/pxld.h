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
		namespace core
		{
			template <typename type>
			struct point
			{
				type x;
				type y;

				inline double distance(point<type> &other)
				{
					return std::sqrt(std::pow(this->x - other.x, 2.0) + std::pow(this->y - other.y, 2.0));
				}

				inline double inverse_distance(point<type> &other, double max)
				{
					return max - std::fmin(distance(other), max);
				}

				inline double inverse_distance_fraction(point<type> other, double max)
				{
					return inverse_distance(other, max) / max;
				}

				inline bool is_natural()
				{
					return (this->x > 0) && (this->y > 0);
				}
			};

			template <typename new_type>
			static inline point<new_type> random_point(point<new_type> min, point<new_type> max)
			{
				return point<new_type> {rand::next<new_type>(min.x, max.x), rand::next<new_type>(min.y, max.y)};
			}

			template <typename unit_type, typename data_type, typename color_type>
			class image
			{
			private:
				data_type* data;

				point<unit_type> size;
				unit_type area;

				unit_type color_width;
				unit_type x_width;
				unit_type y_width;

			public:
				inline image(point<unit_type> size, unit_type color_width)
				{
					this->size = size;
					this->color_width = color_width;

					math::safe_mul<unit_type>(size.x, color_width, this.x_width);
					math::safe_mul<unit_type>(size.y, this.x_width, this.y_width);
					math::safe_mul<unit_type>(size.x, size.y, this.area);

					this->data = new data_type[y_width];
				}

				~image()
				{
					delete[] this->data;
				}

				template <typename cast_type>
				inline void fill(color_type color)
				{
					cast_type *data = (cast_type *)this->data;

					for (unit_type index = 0; index < this->area; index += 1)
					{
						data[index] = color;
					}
				}

				inline void fill(color_type color)
				{
					color_type color_copy;
					for (unit_type index = 0; index < this->y_width; index += this->color_width)
					{
						color_copy = color;
						for (unit_type c_offset = this->color_width; c_offset > 0;)
						{
							c_offset -= 1;
							this->data[index + c_offset] = color_copy & 0xFF;
							temp_color >>= 8;
						}
					}
				}
			};

			template <typename new_type>
			static const point<new_type> ZERO = { 0, 0 };

			template <typename new_type>
			static const point<new_type> RIGHT = { 1, 0 };

			template <typename new_type>
			static const point<new_type> LEFT = { -1, 0 };

			template <typename new_type>
			static const point<new_type> UP = { 0, 1 };

			template <typename new_type>
			static const point<new_type> DOWN = { 0, -1 };
		}

		typedef size_t unit_t;
		typedef double funit_t;
		typedef uint32_t color_t;

#define SCL_PXLD_IS_UNIT_SIGNED 0

		const uint8_t unit_width = sizeof(unit_t);

		typedef uint8_t mode_t;

		namespace mode
		{
			const mode_t W = 0;
			const mode_t WA = 1;
			const mode_t RGB = 2;
			const mode_t RGBA = 3;

			static inline bool is_valid(mode_t mode)
			{
				switch (mode)
				{
				case W:
				case WA:
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
				case W: return 1;
				case WA: return 2;
				case RGB: return 3;
				case RGBA: return 4;
				default:
					throw new_invalid_argument("get_width", "mode");
					return 0;
				}
			}

			static inline const char *get_string(mode_t mode)
			{
				switch (mode)
				{
				case W: return "L";
				case WA: return "LA";
				case RGB: return "RGB";
				case RGBA: return "RGBA";
				default:
					throw new_invalid_argument("get_width", "mode");
					return 0;
				}
			}
		}

		namespace colors
		{
			const color_t RGB_WHITE =	0xFFFFFF;
			const color_t RGB_BLACK =	0x000000;

			const color_t RGB_RED =		0xFF0000;
			const color_t RGB_GREEN =	0x00FF00;
			const color_t RGB_BLUE =	0x0000FF;

			static inline color_t make_w(unit_t l)
			{
				return (color_t)l;
			}

			static inline color_t make_wa(uint8_t l, uint8_t a)
			{
				return ((color_t)l << 8) | ((color_t)a);
			}

			static inline color_t make_rgb(uint8_t r, uint8_t g, uint8_t b)
			{
				return ((color_t)r << 16) | ((color_t)g << 8) | ((color_t)b);
			}

			static inline color_t make_rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
			{
				return ((color_t)r << 24) | ((color_t)g << 16) | ((color_t)b << 8) | ((color_t)a);
			}
		}

		typedef struct core::point<unit_t> point;

		static const point ZERO = { 0, 0 };
		static const point RIGHT = { 1, 0 };
		static const point LEFT = { -1, 0 };
		static const point UP = { 0, 1 };
		static const point DOWN = { 0, -1 };

		class image
		{
		private:
			mode_t mode;
			core::image<unit_t, byte, color_t> *base;

		public:
			inline image(point size, mode_t mode)
			{
				unit_t color_width = mode::get_width(mode);
				base = new core::image<unit_t, byte, color_t>(size, color_width);
			}

			inline ~image()
			{
				delete base;
			}

			inline void fill(color_t color)
			{

			}
		};

		namespace _core
		{
			template <typename type>
			static inline void draw_point(image_t *image, point point, color_t color)
			{
				type *data = (type *)image->data;

				data[(point.y * image->size.x) + point.y] = color;
			}

			static inline void draw_point(image_t *image, point point, color_t color)
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

			template <typename type>
			static inline void draw(image_t *image, color_t(*func) (point<unit_t>))
			{
				type *data = (type *)image->data;

				for (unit_t y = 0, y_offset = 0; y < image->size.y; y += 1, y_offset += image->size.x)
				{
					for (unit_t x = 0; x < image->size.x; x += 1)
					{
						data[y_offset + x] = func(point<unit_t>{ x, y });
					}
				}
			}

			static inline void draw(image_t *image, color_t(*func) (point<unit_t>))
			{
				color_t color;

				for (unit_t y_offset = 0, y = 0; y_offset < image->y_width; y_offset += image->x_width, y += 1)
				{
					for (unit_t x_offset = 0, x = 0; x_offset < image->x_width; x_offset += image->color_width, x += 1)
					{
						color = func(point<unit_t>{ x, y });

						for (unit_t c_offset = image->color_width; c_offset > 0;)
						{
							c_offset -= 1;

							image->data[y_offset + x_offset + c_offset] = color & 0xFF;
							color <<= 8;
						}
					}
				}
			}

			template <typename type>
			static inline void map(image_t *image, color_t(*func) (point<unit_t>, color_t))
			{
				type *data = (type *)image->data;

				for (unit_t y = 0, y_offset = 0; y < image->size.y; y += 1, y_offset += image->size.x)
				{
					for (unit_t x = 0; x < image->size.x; x += 1)
					{
						data[y_offset + x] = func(point<unit_t>{ x, y }, (color_t)data[y_offset + x]);
					}
				}
			}

			static inline void map(image_t *image, color_t(*func) (point<unit_t>, color_t))
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

						color = func(point<unit_t>{ x, y }, color);

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

		typedef struct
		{
			byte *data;

			mode_t mode;
			
			size_t area;

			unit_t color_width;
			unit_t x_width;
			unit_t y_width;
		} image_t;

		namespace image
		{
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

			static inline void draw_point(image_t *image, point<unit_t> point, color_t color)
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

			static inline void draw(image_t *image, color_t(*func) (point<unit_t>))
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

			static inline void map(image_t *image, color_t(*func) (point<unit_t>, color_t))
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