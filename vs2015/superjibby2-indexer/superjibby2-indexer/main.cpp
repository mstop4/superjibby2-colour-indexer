#include "Converter.h"

int main()
{
	Converter *c = new Converter();

	int result = c->read_png(c->src);
	if (result == 1)
		return 1;

	c->process_image();
	c->write_png();

	delete c;
	return 0;
} 