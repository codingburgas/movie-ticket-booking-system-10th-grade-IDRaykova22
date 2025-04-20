#include "pch.h"
#include "userInterface.h"

Ui::Ui()
{
    this->user = new User;
    startScreen();
}

//Destructor that cleans memory 
Ui::~Ui() {
    delete user;
}

//Displays start screen
void Ui::startScreen()
{
    Utilities::SetColor(32);

    Utilities::displayFile("../assets/graphic/cinema.txt");

    std::string choice;

    while (true)
    {
        std::getline(std::cin, choice);

        if (choice.empty())
        {
            //mainMenu();
            break;
        }
        else
        {
            std::cout << "You've entered an invalid option. Please try again." << std::endl;
        }
    }

    Utilities::resetColor();
}