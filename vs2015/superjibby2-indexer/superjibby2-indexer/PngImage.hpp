#pragma once
#include "png.h"

struct PNGImage
{
	int width = 0, height = 0;
	png_byte colour_type;
	png_byte bit_depth;
	png_bytep *row_pointers;
};