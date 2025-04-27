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

    Utilities::displayFile("../assets/graphic/glasses.txt");
    std::cout << message << std::endl;
}

void registerUi()
{
    const char UIFile[] = "../assets/graphic/register.txt";

    std::string line, password, confirmPassword, email;

    //Clear console
    system("cls");

    std::ifstream file(UIFile);

    if (!file.is_open() || Utilities::isFileEmpty(UIFile)) {
        std::cout << "Could not open file for reading!" << std::endl;
        return;
    }
}

void Ui::mainMenu() {
    //Clear console
    system("cls");
    Utilities::displayFile("../assets/graphic/glasses.txt");

    while (true) {

        char choice;
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            registerUi();
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