#pragma once
#include "pch.h"
#include <user.h>
#include "utilities.h"

class Ui
{
public:
	Ui();
	~Ui();

	void mainMenu();
	void startScreen();
	void registerUi();
	void registerAsAdmin();
private:
	void displayMessage(std::string message);

	//void registerUi();
	//void registerAsAdmin();

	User* user;
};