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

	// Read input palette
	c->read_png("in.png", c->in_pal);

	// Load source image
	c->read_png("test.png", c->src);

	// Process image
	c->process_image();

	// Save new image
	c->write_png("out.png");

	delete c;
	return 0;
} 