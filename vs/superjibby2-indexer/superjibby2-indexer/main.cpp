#include <string>
#include "Converter.hpp"

/*	Steps to success
1) Get a list of all PNG files to be processed
2) Get input colours from input palette
3) Load source image
4) Process image
5) Save new image
*/

int main()
{
	Converter *c = new Converter();
	int result;

	// Get a list of all PNG files to be processed
	c->get_files("unindexed");

	// Read input palette
	result = c->read_png("in.png", c->in_pal);

	if (result == 1)
		return 1;

	for (auto& file: c->files)
	{
		// Load source image
		c->read_png(file.string().c_str(), c->src);

		if (result == 1)
			return 1;

		// Process image
		c->process_image();

		// Save new image
		std::string new_path = "indexed\\i_" + file.filename().string();

		c->write_png(new_path.c_str(), c->src);
	}

	delete c;
	return 0;
} 