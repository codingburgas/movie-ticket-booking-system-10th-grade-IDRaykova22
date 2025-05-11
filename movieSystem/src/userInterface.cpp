#include "pch.h"
#include "userInterface.h"
#include "utilities.h"
#include "users.h"

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
            loginUi();
            break;
        case '3':
            chooseMovie();
            break;
        case '4':
            displayMessage("You've logged out from: " + user->getUserName() + "\n");
            user->eraseUser();
            break;
        default:
            displayMessage("You've entered an invalid option. Please try again.");
            break;
        }
    }
}

void Ui::chooseMovie()
{
    //Clear console
    system("cls");

    if (user->getIsAdmin())
    {
        adminMenu();
    }
}

void Ui::adminMenu()
{
    while (true) {
        system("cls");
        Utilities::displayFile("../assets/graphic/adminMenu.txt");

        char choice;
        std::cout << "Choice: ";
        std::cin >> choice;

        switch (choice) {
            case '1':
                createCinema();
                break;
            case '2':
                addMovie();
                break;
            case '3':
                return; // Return to main menu
            default:
                displayMessage("Invalid option. Please try again.");
                break;
        }
    }
}

void Ui::createCinema()
{
    system("cls");
    std::cout << "=== Create New Cinema ===\n\n";

    std::string name, location;
    int numHalls;

    std::cin.ignore();
    std::cout << "Enter cinema name: ";
    std::getline(std::cin, name);

    std::cout << "Enter location: ";
    std::getline(std::cin, location);

    std::cout << "Enter number of halls: ";
    std::cin >> numHalls;

    nlohmann::json cinema;
    cinema["name"] = name;
    cinema["location"] = location;
    cinema["numHalls"] = numHalls;
    cinema["type"] = "cinema";

    Utilities::saveToFile("../assets/movies.json", cinema);
    displayMessage("Cinema created successfully!");
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
    const char adminKey[] = "f2b1ba409ac3670c296150ed002f28d302415705cfa4c689f5d2d8101b98d616"; //Admin24@

    std::cout << "             Enter key: ";

    std::string keyToEnter;
    std::cin >> keyToEnter;

    while (Utilities::sha256FromString(keyToEnter) != adminKey) {
        std::cout << "             Wrong key: ";
        std::cin >> keyToEnter;
    }

    user->setAdmin();
}

void Ui::loginUi()
{
    const char UIFile[] = "../assets/graphic/login.txt";
    const char fileToSave[] = "../assets/users.json";

    //Clear console
    system("cls");

    std::string line, email, password;

    std::ifstream file(UIFile);

    if (!file.is_open() || Utilities::isFileEmpty(UIFile)) {
        std::cout << "Could not open file for reading!" << std::endl;
        return;
    }

    while (std::getline(file, line))
    {

        if (line.find("Email") != std::string::npos)
        {
            std::cout << line;
            std::cin >> email;

            while (!user->loadFromFile(fileToSave, email)) {
                std::cin >> email;
            }
        }
        else if (line.find("Password") != std::string::npos)
        {
            std::cout << line;

            std::cin >> password;
            while (user->getPassword() != Utilities::sha256FromString(password)) {
                std::cout << "Wrong password\nPassword: ";
                std::cin >> password;
            }
        }
        else
        {
            std::cout << line << std::endl;
        }
    }

    file.close();
}