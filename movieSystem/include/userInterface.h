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
	void bookMovie();
	void registerAsAdmin();
	void loginUi();
	void chooseMovie();
	void adminMenu();
	void createCinema();
	void deleteCinema();
	void editCinema();
	void addMovie();
	void deleteMovie();
	void editMovie();
	void showList();

	void displayMessage(std::string message);

	bool loggedIn = false;
private:
	User* user;
};