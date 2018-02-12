#include <string>
#include "Converter.hpp"

/*	Steps to success
1) Get a list of all PNG files to be processed
2) Get input colours from input palette
3) Get output colours from output palette
4) Load source image
5) Process image
6) Save new image
*/

int main(int argc, char* argv[])
{
	if (argc != 5)
	{
		std::cout << "Usage: sj2i <in_palette> <out_palette> <src_dir> <dest_dir>" << std::endl;
		return 1;
	}

	Converter *c = new Converter();
	int result;
	std::string out_path = argv[4];

	// Get a list of all PNG files to be processed
	c->get_files(argv[3]);

	// Read input palette
	result = c->read_png(argv[1], c->in_pal);

	if (result == 1)
		return 1;

	// Read output palette
	result = c->read_png(argv[2], c->out_pal);

	if (result == 1)
		return 1;

	fs::path root_dir = fs::path(argv[3]).parent_path();

	for (auto& file: c->files)
	{
		// Load source image
		result = c->read_png(file.string().c_str(), c->src);

		if (result == 1)
			return 1;

		// Process image
		c->process_image();

		// Save new image
		std::string new_path = out_path + "\\" + c->strip_path(file,root_dir).string();

		result = c->write_png(new_path.c_str(), c->src);

		if (result == 1)
			return 1;
	}

	delete c;
	return 0;
} 