#pragma once
#include "pch.h"

class Ui
{
public:
	Ui();
	~Ui();

	void mainMenu();
	void startScreen();
private:
	void displayMessage(std::string message);

	//User* user;
};