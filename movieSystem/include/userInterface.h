#pragma once
#include "pch.h"

class Ui
{
public:
	Ui();
	~Ui();

	void mainMenu();
	void timeLineUi();
private:
	void startScreen();
	void displayMenuMsg(std::string msg);

	void registerUi();
	void registerAsAdmin();

	void logInUi();
	void adminTimeLine(const std::string& fileName, Timeline& timeline);

	User* user;
};