#include "Converter.hpp"


/*	Steps to success
1) Get input colours from input palette
2) Load source image
3) Process image
4) Save new image
*/

int main()
{
	Converter *c = new Converter();
	int result;

	// Read input palette
	result = c->read_png("in.png", c->in_pal);

	if (result == 1)
		return 1;

	// Load source image
	c->read_png("test.png", c->src);

	if (result == 1)
		return 1;

	// Process image
	c->process_image();

	// Save new image
	c->write_png("out.png", c->src);

	delete c;
	return 0;
} 