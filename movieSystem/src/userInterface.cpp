#include "pch.h"
#include "userInterface.h"
#include "utilities.h"
#include "user.h"

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

void Ui::mainMenu() {
    //Clear console
    system("cls");
    Utilities::displayFile("../assets/graphic/glasses.txt");

    while (true) {

        char choice;
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
        case '1':
            registerUi();
            break;
        case '2':
            break;
        case '3':
            break;
        default:
            displayMessage("You've entered an invalid option. Please try again.");
            break;
        }
    }
}

void Ui::registerUi()
{
    const char UIFile[] = "../assets/graphic/register.txt";
    const char fileToSave[] = "../assets/users.json";

    std::string line, password, confirmPassword, email;

    system("cls");

    std::ifstream file(UIFile);

    if (!file.is_open() || Utilities::isFileEmpty(UIFile)) {
        std::cout << "Could not open file for reading!" << std::endl;
        return;
    }

    while (std::getline(file, line)) {
        if (line.find("Email") != std::string::npos)
        {
            std::cout << line;
            std::cin >> email;
            while (!user->checkEmail(email, fileToSave)) {
                std::cin >> email;
            }
        }
        else if (line.find("Username") != std::string::npos)
        {
            std::cout << line;
            std::string userName;
            std::cin >> userName;
            user->setUserName(userName);
        }
        else if (line.find("Password") != std::string::npos)
        {
            std::cout << line;
            std::cin >> password;
            while (!user->checkPassword(password)) {
                std::cin >> password;
            }
        }
        else if (line.find("Confirm Password") != std::string::npos)
        {
            std::cout << line;
            std::cin >> confirmPassword;
            while (password != confirmPassword) {
                std::cout << "     Wrong password try again";
                std::cin >> confirmPassword;
            }
        }
        else if (line.find("Registering as Admin? ") != std::string::npos)
        {
            char choise;
            std::cout << line;
            std::cin >> choise;

            if (choise == 'y' || choise == 'Y')
            {
                registerAsAdmin();
            }
        }
        else
        {
            std::cout << line << std::endl;
        }
    }

    Utilities::saveToFile(fileToSave, user->saveAsJson());
    file.close();
}


void Ui::registerAsAdmin()
{
    const char adminKey[] = "Test";

    std::cout << "             Enter key: ";

    std::string keyToEnter;
    std::cin >> keyToEnter;

    while (Utilities::sha256FromString(keyToEnter) != adminKey) {
        std::cout << "             Wrong key: ";
        std::cin >> keyToEnter;
    }

    user->setAdmin();
}