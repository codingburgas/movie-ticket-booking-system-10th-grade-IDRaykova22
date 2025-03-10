#include <iostream>

void getInformationMenu()
{
    int choice;
    std::cin >> choice;
    if (choice == 1)
    {

    }
    else if (choice == 2)
    {

    }
    else {
        std::cout << "Please choose valid option.";
    }
}

void printMenu()
{
    std::cout << "  __  __ " << std::endl;
    std::cout << " |  \\/  | ___ _ __  _   _ " << std::endl;
    std::cout << " | |\\/| |/ _ \\ |_ \\| | | |" << std::endl;
    std::cout << " | |  | |  __/ | | | |_| |" << std::endl;
    std::cout << " |_|  |_|\\___|_| |_|\\__._|" << std::endl;

    std::cout << "\nChoose an option:" << std::endl;

    std::cout << "1. Login Page" << std::endl;
    std::cout << "2. Choose Movies" << std::endl;
    getInformationMenu();
}

int main()
{
    printMenu();
}
