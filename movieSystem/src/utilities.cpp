#include "pch.h"

namespace Utilities
{
	//Set color to text
	void SetColor(int textColor)
	{
		std::cout << "\033[" << textColor << "m";
	}

	//Reset color to default
	void resetColor()
	{
		std::cout << "\033[0m";
	}

	//Displays the contents of a txt file
	void displayFile(const std::string& fileName) {
		std::ifstream file(fileName);

		if (file.is_open() || !isFileEmpty(fileName)) {
			std::cout << file.rdbuf() << std::endl;
			file.close();
		}
		else {
			std::cerr << "Could not open file for reading!" << std::endl;
		}
	}
}