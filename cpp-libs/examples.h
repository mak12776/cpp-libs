#pragma once

#include <random>
#include "pxld.h"

namespace examples
{
	using namespace scl::pxld;
	using namespace scl::error;

	image_t *picture_1(point_t size)
	{
		image_t *image;
		point_t point;

		image = image::new_image(size, mode::RGB);
		if (num) return nullptr;

		image::draw_color(image, 0x000000);
		if (num) return nullptr;

		for (size_t i = 0; i < 1000; i += 1)
		{
			image::random_point(image, &point);
			if (num) return nullptr;

			image::draw_point(image, point, 0xFFFFFF);
			if (num) return nullptr;
		}

		return image;
	}
}
