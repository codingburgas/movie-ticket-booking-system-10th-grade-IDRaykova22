#include "pch.h"
#include "userInterface.h"
#include "utilities.h"

Ui::Ui()
{
    //this->user = new User;
    startScreen();
}

//Destructor that cleans memory 
Ui::~Ui() {
    //delete user;
}

//Displays start screen
void Ui::startScreen()
{
    Utilities::SetColor(33);

    Utilities::displayFile("../assets/graphic/cinema.txt");

    std::string choice;

    while (true)
    {
        std::getline(std::cin, choice);

        if (choice.empty())
        {
            mainMenu();
            break;
        }
        else
        {
            std::cout << "You've entered an invalid option. Please try again." << std::endl;
        }
    }

    Utilities::resetColor();
}

void Ui::displayMessage(std::string message)
{
    //Clear console
    system("cls");

    std::cout << message << std::endl;
}

void Ui::mainMenu() {
    //Clear console
    system("cls");
    Utilities::displayFile("../assets/graphic/glasses.txt");

    while (true) {

        int choice;
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            break;
        case 2:
            break;
        case 4:
            break;
        case 3:
            break;
        default:
            displayMessage("You've entered an invalid option. Please try again.");
            break;
        }
    }
}