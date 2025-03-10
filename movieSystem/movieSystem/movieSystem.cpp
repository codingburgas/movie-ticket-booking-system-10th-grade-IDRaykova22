#include <iostream>
#include <fstream>

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
    std::ifstream f("menu.txt");

    if (f.is_open())
        std::cout << f.rdbuf();

    std::cout << "\nChoose an option:" << std::endl;

    std::cout << "1. Login Page" << std::endl;
    std::cout << "2. Choose Movies" << std::endl;
    getInformationMenu();
}

int main()
{
    printMenu();
}
