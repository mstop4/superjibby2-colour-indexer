#pragma once

#include <iostream>
#include <memory>
#include <stdio.h>
#include <errno.h>
#include "png.h"
#include "PngImage.hpp"

class Converter
{
	public:

		std::shared_ptr<PNGImage> src, in_pal;

		Converter();

		int read_png(char* filename, std::shared_ptr<PNGImage> img);
		int write_png(char *filename, std::shared_ptr<PNGImage> img);

		void dump_png_data(std::shared_ptr<PNGImage> img);
		void process_image();

		void free_png(std::shared_ptr<PNGImage> img);

		~Converter();
};

