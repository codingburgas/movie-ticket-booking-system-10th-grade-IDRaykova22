#include "pch.h"

namespace Utilities
{
	void SetColor(int textColor);
	void resetColor();

	void displayFile(const std::string& fileName);
	bool isFileEmpty(const std::string& fileName);

	void saveToFile(const std::string& fileName, const nlohmann::json& data);
	std::string sha256FromString(const std::string& input);
	nlohmann::json loadFile(const std::string& fileName);
};