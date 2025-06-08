#pragma once
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
	void deleteCinema();
	void editCinema();
	void addMovie();
	void showList();

	void displayMessage(std::string message);
private:
	User* user;
};