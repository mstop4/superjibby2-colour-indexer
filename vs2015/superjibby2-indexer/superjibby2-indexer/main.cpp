#include "png.h"
#include <iostream>
#include <string>
#include <stdio.h>
#include <errno.h>

#define PNGSIGSIZE 8

int main()
{
	std::cout << "Hello World!" << std::endl;

	// Try loading a file
	std::string filename;
	FILE *file_pt;
	errno_t err;

	std::cout << "Enter a filename:" << std::endl;
	std::cin >> filename;

	err = fopen_s(&file_pt, filename.c_str(), "rb");

	if (err == ENOENT)
	{
		std::cerr << "File not found!" << std::endl;
		return 0;
	}

	else
	{
		std::cout << filename << " is totally legit." << std::endl;
		fclose(file_pt);
	}

	return 1;

} 