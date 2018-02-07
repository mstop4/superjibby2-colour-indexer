#pragma once
#include "png.h"

struct PNGImage
{
	int width, height;
	png_byte colour_type;
	png_byte bit_depth;
	png_bytep *row_pointers;
};