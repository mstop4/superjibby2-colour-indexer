#include <iostream>
#include <string>
#include <stdio.h>
#include <errno.h>
#include "png.h"

#define PNGSIGSIZE 8

int width, height;
png_byte colour_type;
png_byte bit_depth;
png_bytep *row_pointers;

int png_read()
{
	std::string filename;
	FILE *file_pt;
	errno_t err;

	std::cout << "Hello World!" << std::endl;

	// Try loading a file
	std::cout << "Enter a filename:" << std::endl;
	std::cin >> filename;

	err = fopen_s(&file_pt, filename.c_str(), "rb");

	if (err == ENOENT)
	{
		std::cerr << "File not found!" << std::endl;
		return 1;
	}

	std::cout << filename << " is totally legit." << std::endl;

	// Init structs
	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png)
	{
		std::cerr << "Error: Could not initialize PNG read struct" << std::endl;
		abort();
	}

	png_infop info = png_create_info_struct(png);
	if (!info)
	{
		std::cerr << "Error: Could not initialize PNG info struct" << std::endl;
		abort();
	}

	// Error handling
	if (setjmp(png_jmpbuf(png)))
	{
		std::cerr << "Flagrant System Error" << std::endl;
		abort();
	}

	png_init_io(png, file_pt);
	png_read_info(png, info);

	width = png_get_image_width(png, info);
	height = png_get_image_height(png, info);
	colour_type = png_get_color_type(png, info);
	bit_depth = png_get_bit_depth(png, info);

	std::cout << "\nWidth: " << width << std::endl
		<< "Height: " << height << std::endl
		<< "Colour Type: " << colour_type << std::endl
		<< "Bit Depth: " << bit_depth << std::endl;

	if (bit_depth == 16)
	{
		std::cout << "Strip 16-bit channels to 8-bit." << std::endl;
		png_set_strip_16(png);
	}

	if (colour_type == PNG_COLOR_TYPE_PALETTE)
	{
		std::cout << "PNG is using a palette: convert to RGB." << std::endl;
		png_set_palette_to_rgb(png);
	}

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if (colour_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
	{
		std::cout << "Expand grayscale to 8-bit" << std::endl;
		png_set_expand_gray_1_2_4_to_8(png);
	}

	if (png_get_valid(png, info, PNG_INFO_tRNS))
	{
		std::cout << "Valid tRNS" << std::endl;
		png_set_tRNS_to_alpha(png);
	}

	// These color_type don't have an alpha channel then fill it with 0xff.
	if (colour_type == PNG_COLOR_TYPE_RGB ||
		colour_type == PNG_COLOR_TYPE_GRAY ||
		colour_type == PNG_COLOR_TYPE_PALETTE)
	{
		std::cout << "Pad with alpha channel" << std::endl;
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
	}

	if (colour_type == PNG_COLOR_TYPE_GRAY ||
		colour_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		std::cout << "Covnert grayscale to RGB" << std::endl;
		png_set_gray_to_rgb(png);
	}

	png_read_update_info(png, info);

	row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
	for (int y = 0; y < height; y++)
	{
		row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
	}

	std::cout << "Reading image" << std::endl;
	png_read_image(png, row_pointers);

	fclose(file_pt);

	png_destroy_read_struct(&png, &info, NULL);
	png = NULL;
	info = NULL;

	return 0;
}

int png_write()
{
	std::string filename;
	FILE *file_pt;
	errno_t err;

	// Try saving a file
	std::cout << "Enter a save:" << std::endl;
	std::cin >> filename;

	err = fopen_s(&file_pt, filename.c_str(), "wb");

	if (err == ENOENT)
	{
		std::cerr << "File not found!" << std::endl;
		return 1;
	}

	std::cout << filename << " is totally legit." << std::endl;

	// Init structs
	png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png)
	{
		std::cerr << "Error: Could not initialize PNG read struct" << std::endl;
		abort();
	}

	png_infop info = png_create_info_struct(png);
	if (!info)
	{
		std::cerr << "Error: Could not initialize PNG info struct" << std::endl;
		abort();
	}

	// Error handling
	if (setjmp(png_jmpbuf(png)))
	{
		std::cerr << "Flagrant System Error" << std::endl;
		abort();
	}

	png_init_io(png, file_pt);

	png_set_IHDR(
		png,
		info,
		width, height,
		8,
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);

	png_write_info(png, info);

	png_write_image(png, row_pointers);
	png_write_end(png, NULL);

	std::cout << "Freeing memory" << std::endl;

	for (int y = 0; y < height; y++)
	{
		free(row_pointers[y]);
	}

	free(row_pointers);

	fclose(file_pt);

	if (png && info)
	{
		png_destroy_write_struct(&png, &info);
	}

	return 0;
}

void process_png_file()
{
	for (int y = 0; y < height; y++)
	{
		png_bytep row = row_pointers[y];

		for (int x = 0; x < width; x++)
		{
			png_bytep px = &(row[x * 4]);

			px[0] = 255;

			// Do something awesome for each pixel here...
			printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
		}
	}
}

int main()
{
	int result = png_read();

	if (result == 1)
		return 1;

	process_png_file();

	png_write();

	return 0;
} 