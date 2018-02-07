#include "Converter.hpp"

int main()
{
	Converter *c = new Converter();

	int result = c->read_png(c->src);
	if (result == 1)
		return 1;

	c->dump_png_data(c->src);
	c->process_image();
	c->dump_png_data(c->src);

	delete c;
	return 0;
} 