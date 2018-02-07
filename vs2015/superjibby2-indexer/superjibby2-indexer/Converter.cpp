#include "Converter.h"

Converter::Converter()
{
	src = new PNGImage;
}

int Converter::read_png(PNGImage *img)
{
	std::string filename;
	FILE *file_pt;
	errno_t err;

	std::cout << "Hello World!" << std::endl << std::endl;

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

	// Jump back here if something goes wrong with libpng
	if (setjmp(png_jmpbuf(png)))
	{
		std::cerr << "Flagrant System Error" << std::endl;
		abort();
	}

	// Read PNG info
	png_init_io(png, file_pt);
	png_read_info(png, info);

	img->width = png_get_image_width(png, info);
	img->height = png_get_image_height(png, info);
	img->colour_type = png_get_color_type(png, info);
	img->bit_depth = png_get_bit_depth(png, info);

	std::cout << "\nWidth: " << img->width << std::endl
		<< "Height: " << img->height << std::endl
		<< "Colour Type: " << img->colour_type << std::endl
		<< "Bit Depth: " << img->bit_depth << std::endl;

	// Normalize PNG format
	if (img->bit_depth == 16)
	{
		std::cout << "Strip 16-bit channels to 8-bit." << std::endl;
		png_set_strip_16(png);
	}

	if (img->colour_type == PNG_COLOR_TYPE_PALETTE)
	{
		std::cout << "PNG is using a palette: convert to RGB." << std::endl;
		png_set_palette_to_rgb(png);
	}

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if (img->colour_type == PNG_COLOR_TYPE_GRAY && img->bit_depth < 8)
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
	if (img->colour_type == PNG_COLOR_TYPE_RGB ||
		img->colour_type == PNG_COLOR_TYPE_GRAY ||
		img->colour_type == PNG_COLOR_TYPE_PALETTE)
	{
		std::cout << "Pad with alpha channel" << std::endl;
		png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
	}

	if (img->colour_type == PNG_COLOR_TYPE_GRAY ||
		img->colour_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	{
		std::cout << "Covnert grayscale to RGB" << std::endl;
		png_set_gray_to_rgb(png);
	}

	png_read_update_info(png, info);

	// Prepare memory space for bitmap data
	std::cout << "Reading image" << std::endl;

	img->row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * img->height);
	for (int y = 0; y < img->height; y++)
	{
		img->row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png, info));
	}

	png_read_image(png, img->row_pointers);

	//Clean up
	fclose(file_pt);

	png_destroy_read_struct(&png, &info, NULL);
	png = NULL;
	info = NULL;

	return 0;
}

void Converter::process_image()
{
	for (int y = 0; y < src->height; y++)
	{
		png_bytep row = src->row_pointers[y];

		for (int x = 0; x < src->width; x++)
		{
			png_bytep px = &(row[x * 4]);

			px[0] = 255;

			// Do something awesome for each pixel here...
			printf("%4d, %4d = RGBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
		}
	}
}

int Converter::write_png()
{
	std::string filename;
	FILE *file_pt;
	errno_t err;

	// Try creating a new file for saving
	std::cout << "Enter a save:" << std::endl;
	std::cin >> filename;

	err = fopen_s(&file_pt, filename.c_str(), "wb");

	if (err == ENOENT)
	{
		std::cerr << "File cannot be created!" << std::endl;
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

	// Jump back here if something goes wrong with libpng
	if (setjmp(png_jmpbuf(png)))
	{
		std::cerr << "Flagrant System Error" << std::endl;
		abort();
	}

	// Write PNG info
	png_init_io(png, file_pt);

	png_set_IHDR(
		png,
		info,
		src->width, src->height,
		8,
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);

	png_write_info(png, info);

	// Write bitmap data
	png_write_image(png, src->row_pointers);
	png_write_end(png, NULL);

	// Clean up
	std::cout << "Freeing memory" << std::endl;

	for (int y = 0; y < src->height; y++)
	{
		free(src->row_pointers[y]);
	}

	free(src->row_pointers);

	fclose(file_pt);

	if (png && info)
	{
		png_destroy_write_struct(&png, &info);
	}

	return 0;
}

Converter::~Converter()
{
	delete src;
}