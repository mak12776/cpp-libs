#pragma once

#include <cstdint>

#include "math.h"
#include "scl.h"

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

		template <typename unit_type, typename data_type, typename color_type, color_type data_mask, color_type data_shift>
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

				scl::math::safe_mul<unit_type>(size.x, color_width, this->x_width);
				scl::math::safe_mul<unit_type>(size.y, this->x_width, this->y_width);
				scl::math::safe_mul<unit_type>(size.x, size.y, this->area);

				this->data = new data_type[y_width];
			}

			inline data_type* get_data() const
			{
				return this->data;
			}

			inline point<unit_type> get_size() const
			{
				return this->data;
			}

			inline unit_type get_area() const
			{
				return this->area;
			}

			inline unit_type get_color_width() const
			{
				return this->color_width;
			}

			inline unit_type get_x_width() const
			{
				return this->x_width;
			}

			inline unit_type get_y_width() const
			{
				return this->y_width;
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
					for (unit_type c_offset = this->color_width; c_offset > 0; )
					{
						c_offset -= 1;

						this->data[index + c_offset] = color_copy & data_mask;
						color_copy >>= data_shift;
					}
				}
			}

			template <typename cast_type>
			inline void map(color_type(*func_pntr) (point<unit_type> point))
			{
				cast_type *data = (cast_type *)this->data;

				for (unit_type y = 0, y_offset = 0; y < this->size.y; y += 1, y_offset += this->size.x)
				{
					for (unit_type x = 0; x < this->size.x; x += 1)
					{
						data[y_offset + x] = func_pntr({ x, y });
					}
				}
			}

			inline void map(color_type(*func_pntr) (point<unit_type> point))
			{
				color_type color;

				for (unit_type y = 0, y_offset = 0; y < this->size.y; y += 1, y_offset += this->x_width)
				{
					for (unit_type x = 0, x_offset = 0; x < this->size.x; x += 1, x_offset += this->color_width)
					{
						color = func_pntr({ x, y });

						for (unit_type c_offset = this->color_width; c_offset > 0; )
						{
							c_offset -= 1;

							data[y_offset + x_offset + c_offset] = color & data_mask;
							color >>= data_shift;
						}
					}
				}
			}

			template <typename cast_type>
			inline void effect(color_type(*func_pntr) (point<unit_type> point, color_type color))
			{
				cast_type *data = (cast_type *)this->data;

				for (unit_type y = 0, y_offset = 0; y < this->size.y; y += 1, y_offset += this->size.x)
				{
					for (unit_type x = 0; x < this->size.x; x += 1)
					{
						data[y_offset + x] = func_pntr({ x, y }, data[y_offset + x]);
					}
				}
			}

			inline void effect(color_type(*func_pntr) (point<unit_type> point, color_type color))
			{
				color_type color;

				for (unit_type y = 0, y_offsett = 0; y < this->size.y; y += 1, y_offsett += this->x_width)
				{
					for (unit_type x = 0, x_offset = 0; x < this->size.x; x += 1, x_offset += this->color_width)
					{
						color = 0;

						unit_type xy_offset = y_offsett + x_offset;

						for (unit_type c_offset = this->color_width; c_offset > 0; )
						{
							c_offset -= 1;

							color <<= data_shift;
							color |= this->data[xy_offset + c_offset];
						}

						color = func_pntr({ x, y }, color);

						for (unit_type c_offset = this->color_width; c_offset > 0; )
						{
							c_offset -= 1;

							this->data[xy_offset + c_offset] = color & data_mask;
							color >>= data_shift;
						}
					}
				}
			}

			template <typename cast_type>
			inline void draw_point(point<unit_type> point, color_type color)
			{
				cast_type *data = (cast_type *)this->data;
				data[(point.y * this->size.x) + point.x] = color;
			}

			inline void draw_point(point<unit_type> point, color_type color)
			{
				unit_type index = (point.y * this->x_width) + (point.x * this->color_width);
				for (unit_type c_offset = this->color_width; c_offset > 0; )
				{
					c_offset -= 1;

					this->data[index + c_offset] = color & data_mask;
					color >>= data_shift;
				}
			}
		};
	}

	// units

	constexpr bool IS_UNIT_SIGNED = false;

	typedef size_t unit_t;
	typedef double funit_t;

	const uint8_t unit_width = sizeof(unit_t);

	// mode type

	typedef uint8_t mode_t;

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
				scl::err::set(scl::err::INVALID_ARGUMENT);
				scl::err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
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
				scl::err::set(scl::err::INVALID_ARGUMENT);
				scl::err::push_file_info(__FILE__, __LINE__, __FUNCTION__);
				return nullptr;
			}
		}
	}

	// color unit

	typedef uint32_t color_t;

	namespace colors
	{
		const color_t RGB_WHITE =	0xFFFFFF;
		const color_t RGB_BLACK =	0x000000;

		const color_t RGB_RED =		0xFF0000;
		const color_t RGB_GREEN =	0x00FF00;
		const color_t RGB_BLUE =	0x0000FF;

		const color_t RGB_AQUA =	0x00FFFF;
		const color_t RGB_PURPLE =	0xFF00FF;
		const color_t RGB_YELLOW =	0xFFFF00;

		static inline color_t make_l(unit_t l)
		{
			return (color_t)l;
		}

		static inline color_t make_la(uint8_t l, uint8_t a)
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

	typedef core::point<unit_t> point;

	static const point ZERO = { 0, 0 };
	static const point RIGHT = { 1, 0 };
	static const point LEFT = { -1, 0 };
	static const point UP = { 0, 1 };
	static const point DOWN = { 0, -1 };

	class image
	{
	private:
		mode_t mode;
		core::image<unit_t, uint8_t, color_t, 0xFF, 8> *base;

	public:
		inline image(point size, mode_t mode)
		{
			unit_t color_width = mode::get_width(mode);
			base = new core::image<unit_t, uint8_t, color_t, 0xFF, 8>(size, color_width);
		}

		inline ~image()
		{
			delete base;
		}
	};

	class animation
	{
	private:
		image *image_data;
		void(*start) (image &image_data);
		void(*step) (image &image_data);

	public:
		animation(image *image_data, void(*start) (image &image_data), void(*step) (image &image_data))
			: image_data(image_data), start(start), step(step) { }
	};
}
