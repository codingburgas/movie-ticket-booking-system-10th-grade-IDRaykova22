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
	void loginUi();

	void displayMessage(std::string message);
private:
	User* user;
};