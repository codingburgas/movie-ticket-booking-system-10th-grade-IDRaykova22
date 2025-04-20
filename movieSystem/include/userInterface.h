#pragma once
#include "pch.h"

class Ui
{
public:
	Ui();
	~Ui();

	void mainMenu();
private:
	void startScreen();
	void displayMenuMsg(std::string msg);

	void registerUi();
	void registerAsAdmin();

	void logInUi()

	User* user;
};