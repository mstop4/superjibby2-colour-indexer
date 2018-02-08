#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <stdio.h>
#include <errno.h>
#include "png.h"
#include "PngImage.hpp"

/*	Steps to success
1) Get input colours from input palette
2) Get output colours from output palette
3) Load source image
4) Process image
5) Save new image
*/

class Converter
{
	public:

		std::shared_ptr<PNGImage> src, in_pal, out_pal;

		Converter();

		int read_png(std::shared_ptr<PNGImage> img);
		int write_png();

		void dump_png_data(std::shared_ptr<PNGImage> img);
		void process_image();

		void free_png(std::shared_ptr<PNGImage> img);

		~Converter();
};

