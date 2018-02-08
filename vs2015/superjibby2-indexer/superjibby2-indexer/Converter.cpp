#include "Converter.hpp"

Converter::Converter()
{
	src = std::make_shared<PNGImage>();
	in_pal = std::make_shared<PNGImage>();
}

int Converter::read_png(char* filename, std::shared_ptr<PNGImage> img)
{
	FILE *file_pt;
	errno_t err;

	err = fopen_s(&file_pt, filename, "rb");

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

void Converter::dump_png_data(std::shared_ptr<PNGImage> img)
{
	for (int y = 0; y < img->height; y++)
	{
		png_bytep row = img->row_pointers[y];

		for (int x = 0; x < img->width; x++)
		{
			png_bytep px = &(row[x * 4]);

			std::cout << x << ", " << y << "-> (" << (int)px[0] << ", " << (int)px[1] << ", " << (int)px[2] << ", " << (int)px[3] << ")" << std::endl;
		}
	}
}

void Converter::process_image()
{
	std::cout << "Processing image" << std::endl;
	int total_pixels = src->width * src->height;
	float progress;

	for (int y = 0; y < src->height; y++)
	{
		png_bytep src_row = src->row_pointers[y];

		for (int x = 0; x < src->width; x++)
		{
			progress = ((float)(x + y * src->width) / (float)total_pixels) * 100.0f;
			std::cout << "\r" << std::fixed << std::setprecision(2) << progress << "%";
			png_bytep src_px = &(src_row[x * 4]);

			bool done = false;

			for (int v = 0; v < in_pal->height && !done; v++)
			{
				png_bytep in_row = in_pal->row_pointers[v];

				for (int u = 0; u < in_pal->width && !done; u++)
				{
					png_bytep in_px = &(in_row[u * 4]);
					if (src_px[0] == in_px[0] &&
						src_px[1] == in_px[1] &&
						src_px[2] == in_px[2])
					{
						src_px[0] = v;
						src_px[1] = u;
						src_px[2] = 0;
						src_px[3] = 255;
						done = true;
					}
				}
			}

			// No match was found, make pixel fuschia
			if (!done)
			{
				src_px[0] = 255;
				src_px[1] = 0;
				src_px[2] = 255;
				src_px[3] = 255;
			}
		}
	}
}

int Converter::write_png(char *filename, std::shared_ptr<PNGImage> img)
{
	FILE *file_pt;
	errno_t err;

	err = fopen_s(&file_pt, filename, "wb");

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
		img->width, img->height,
		8,
		PNG_COLOR_TYPE_RGBA,
		PNG_INTERLACE_NONE,
		PNG_COMPRESSION_TYPE_DEFAULT,
		PNG_FILTER_TYPE_DEFAULT
	);

	png_write_info(png, info);

	// Write bitmap data
	png_write_image(png, img->row_pointers);
	png_write_end(png, NULL);

	// Clean up
	fclose(file_pt);

	if (png && info)
	{
		png_destroy_write_struct(&png, &info);
	}

	return 0;
}

void Converter::free_png(std::shared_ptr<PNGImage> img)
{
	// Clean up
	std::cout << "Freeing memory" << std::endl;

	for (int y = 0; y < img->height; y++)
	{
		free(img->row_pointers[y]);
	}

	free(img->row_pointers);
}

Converter::~Converter()
{
	free_png(src);
	free_png(in_pal);
}
