#pragma once

#include <errno.h>
#include <experimental/filesystem> // C++-standard header file name  
#include <filesystem> // Microsoft-specific implementation header file name  
#include <iostream>
#include <iomanip>
#include <memory>
#include <stdio.h>
#include <vector>

#include "png.h"
#include "PngImage.hpp"

namespace fs = std::experimental::filesystem::v1;

class Converter
{
	public:

		std::shared_ptr<PNGImage> src, in_pal;
		std::vector<fs::path> files;

		Converter();

		void Converter::get_files(char* directory);

		int read_png(char* filename, std::shared_ptr<PNGImage> img);
		int write_png(char *filename, std::shared_ptr<PNGImage> img);

		void dump_png_data(std::shared_ptr<PNGImage> img);
		void process_image();

		void free_png(std::shared_ptr<PNGImage> img);

		~Converter();
};

