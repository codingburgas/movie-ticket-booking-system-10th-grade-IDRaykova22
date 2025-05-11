#pragma once
#include "pch.h"
#include <users.h>
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
	void chooseMovie();
	void adminMenu();
	void createCinema();

	void displayMessage(std::string message);
private:
	User* user;
};