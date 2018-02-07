#pragma once

#include <iostream>
#include <string>
#include <stdio.h>
#include <errno.h>
#include "png.h"

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

		int width, height;
		png_byte colour_type;
		png_byte bit_depth;
		png_bytep *row_pointers;

		Converter();

		int read_png();
		void process_image();
		int write_png();

		~Converter();
};

