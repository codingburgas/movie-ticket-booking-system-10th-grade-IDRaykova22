#include "userInterface.h"
#include "utilities.h"
#include "users.h"
#include <set>
#include <algorithm>

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

std::vector<std::string> splitLanguages(const std::string& languages) {
    std::vector<std::string> result;
    std::stringstream ss(languages);
    std::string lang;

    while (std::getline(ss, lang, ',')) {
        lang.erase(0, lang.find_first_not_of(" \t"));
        result.push_back(lang);
    }

    return result;
}

void Ui::bookMovie()
{
    system("cls");

    std::string movieName;
    std::cout << "Enter the movie name you want to book: ";
    std::getline(std::cin, movieName);

    nlohmann::json data = Utilities::loadFile("../assets/movies.json");
    nlohmann::json selectedMovie;
    bool movieFound = false;

    for (auto& item : data) {
        if (item.contains("type") && item["type"] == "movie" &&
            item["name"].get<std::string>() == movieName) {
            selectedMovie = item;
            movieFound = true;
            break;
        }
    }

    if (!movieFound) {
        std::cout << "Movie not found. Press any key to continue...";
        std::cin.get();
        return;
    }

    std::cout << "Booking for: " << selectedMovie["name"].get<std::string>() << std::endl;
    std::cout << "Description: " << selectedMovie["description"].get<std::string>() << std::endl;
    std::cout << "Cinema: " << selectedMovie["cinema"].get<std::string>() << std::endl;
    std::cout << "Hall: " << selectedMovie["hall"].get<std::string>() << std::endl;
    std::cout << "Show time: " << selectedMovie["times"].get<std::string>() << std::endl;
    std::cout << "Duration: " << selectedMovie["duration"].get<int>() << " minutes" << std::endl;
    std::cout << std::endl;

    int children = 0, elderly = 0, regularTickets = 0;

    std::cout << "Are there any children under 10? (y/n): ";
    char hasChildren;
    std::cin >> hasChildren;

    if (hasChildren == 'y' || hasChildren == 'Y') {
        std::cout << "How many children under 10? ";
        std::cin >> children;
    }

    std::cout << "Are there any elderly people over 60? (y/n): ";
    char hasElderly;
    std::cin >> hasElderly;

    if (hasElderly == 'y' || hasElderly == 'Y') {
        std::cout << "How many elderly people over 60? ";
        std::cin >> elderly;
    }

    std::cout << "How many regular tickets (paying customers)? ";
    std::cin >> regularTickets;

    int totalSeats = children + elderly + regularTickets;
    std::cout << "Total seats needed: " << totalSeats << std::endl;

    system("cls");
    Utilities::displayFile("../assets/graphic/seatsChart.txt");

    std::string bookingFileName = "../assets/bookings_" + selectedMovie["name"].get<std::string>() +
        "_" + selectedMovie["cinema"].get<std::string>() +
        "_" + selectedMovie["hall"].get<std::string>() + ".json";

    nlohmann::json bookings;
    std::ifstream bookingFile(bookingFileName);
    if (bookingFile.is_open()) {
        bookingFile >> bookings;
        bookingFile.close();
    }
    else {
        bookings = nlohmann::json::array();
    }

    std::set<int> takenSeats;
    for (auto& booking : bookings) {
        if (booking.contains("seats")) {
            for (int seat : booking["seats"]) {
                takenSeats.insert(seat);
            }
        }
    }

    if (!takenSeats.empty()) {
        std::cout << "\nTaken seats: ";
        for (int seat : takenSeats) {
            std::cout << "[" << seat << "] ";
        }
        std::cout << std::endl;
    }

    //User select seats
    std::vector<int> selectedSeats;
    std::cout << "\nSelect " << totalSeats << " seats (enter seat numbers one by one):" << std::endl;

    for (int i = 0; i < totalSeats; i++) {
        int seatNumber;
        bool validSeat = false;

        while (!validSeat) {
            std::cout << "Seat " << (i + 1) << ": ";
            std::cin >> seatNumber;

            if (seatNumber < 1 || seatNumber > 54) {
                std::cout << "Invalid seat number. Please choose between 1-54." << std::endl;
                continue;
            }

            if (takenSeats.find(seatNumber) != takenSeats.end()) {
                std::cout << "Seat " << seatNumber << " is already taken. Please choose another." << std::endl;
                continue;
            }

            if (std::find(selectedSeats.begin(), selectedSeats.end(), seatNumber) != selectedSeats.end()) {
                std::cout << "You already selected seat " << seatNumber << ". Please choose another." << std::endl;
                continue;
            }

            selectedSeats.push_back(seatNumber);
            validSeat = true;
        }
    }

    double totalPrice = 0.0;
    std::cout << "\nSelected seats and pricing:" << std::endl;

    for (int seat : selectedSeats) {
        double seatPrice = 0.0;
        std::string section;

        if (seat >= 1 && seat <= 9) {
            section = "A (Platinum)";
            seatPrice = 30.0;
        }
        else if (seat >= 10 && seat <= 27) {
            section = "B/C/D (Gold)";
            seatPrice = 20.0;
        }
        else if (seat >= 28 && seat <= 54) {
            section = "E/F/G (Silver)";
            seatPrice = 10.0;
        }

        std::cout << "Seat " << seat << " - Section " << section << " - $" << seatPrice << std::endl;
        totalPrice += seatPrice;
    }

    int freeTickets = children + elderly;
    if (freeTickets > 0 && regularTickets > 0) {
        double pricePerTicket = totalPrice / totalSeats;
        totalPrice = pricePerTicket * regularTickets;

        std::cout << "\nDiscount applied:" << std::endl;
        std::cout << "Free tickets (children + elderly): " << freeTickets << std::endl;
        std::cout << "Paying tickets: " << regularTickets << std::endl;
    }

    std::cout << "\nTotal price: $" << std::fixed << std::setprecision(2) << totalPrice << std::endl;

    std::cout << "Do you want to pay online or walk-in?\n[1] Online\n[2] Walk-in\n";
    char paymentMethod;
    std::cin >> paymentMethod;
    std::cin.ignore();

    std::string customerName, cardNumber, expiryDate, cvv, billingAddress, phoneNumber;

    switch (paymentMethod)
    {
    case '1':
        std::cout << "\n--- Customer Information ---" << std::endl;
        std::cout << "Full Name: ";
        std::getline(std::cin, customerName);

        std::cout << "Phone Number: ";
        std::getline(std::cin, phoneNumber);

        std::cout << "Billing Address: ";
        std::getline(std::cin, billingAddress);

        std::cout << "\n--- Payment Information ---" << std::endl;
        std::cout << "Card Number (16 digits): ";
        std::getline(std::cin, cardNumber);

        std::cout << "Expiry Date (MM/YY): ";
        std::getline(std::cin, expiryDate);

        std::cout << "CVV (3 digits): ";
        std::getline(std::cin, cvv);

        if (customerName.empty() || cardNumber.length() != 16 || cvv.length() != 3) {
            std::cout << "\nInvalid information provided. Booking cancelled." << std::endl;
            std::cout << "Press N to go back";
            return;
        }
        break;
    case '2':
        std::cout << "\n--- Customer Information ---" << std::endl;
        std::cout << "Full Name: ";
        std::getline(std::cin, customerName);

        std::cout << "Phone Number: ";
        std::getline(std::cin, phoneNumber);

        cardNumber = "WALK-IN";
        billingAddress = "WALK-IN";

        std::cout << "See you at the reception.\n";
        break;
    }

    nlohmann::json newBooking;
    newBooking["customerName"] = customerName;
    newBooking["phoneNumber"] = phoneNumber;
    newBooking["billingAddress"] = billingAddress;
    if (paymentMethod == '1') {
        newBooking["cardNumber"] = cardNumber.substr(0, 4) + "****" + cardNumber.substr(12);
    }
    else {
        newBooking["cardNumber"] = cardNumber;
    }
    newBooking["movieName"] = selectedMovie["name"];
    newBooking["cinema"] = selectedMovie["cinema"];
    newBooking["hall"] = selectedMovie["hall"];
    newBooking["showTime"] = selectedMovie["times"];
    newBooking["seats"] = selectedSeats;
    newBooking["totalPrice"] = totalPrice;
    newBooking["regularTickets"] = regularTickets;
    newBooking["children"] = children;
    newBooking["elderly"] = elderly;
    newBooking["bookingDate"] = "2025-07-02";

    bookings.push_back(newBooking);

    std::ofstream outFile(bookingFileName);
    outFile << bookings.dump(4);
    outFile.close();

    system("cls");
    std::cout << "  The purchase was made successfully!\n" << std::endl;
    std::cout << "Booking Summary:" << std::endl;
    std::cout << "Movie: " << selectedMovie["name"].get<std::string>() << std::endl;
    std::cout << "Cinema: " << selectedMovie["cinema"].get<std::string>() << std::endl;
    std::cout << "Hall: " << selectedMovie["hall"].get<std::string>() << std::endl;
    std::cout << "Show Time: " << selectedMovie["times"].get<std::string>() << std::endl;
    std::cout << "Customer: " << customerName << std::endl;
    std::cout << "Seats: ";
    for (int seat : selectedSeats) {
        std::cout << "[" << seat << "] ";
    }
    std::cout << std::endl;
    std::cout << "Total Paid: $" << std::fixed << std::setprecision(2) << totalPrice << std::endl;
    if (paymentMethod == '2') {
        std::cout << "\nPlease pay at the cinema reception before the show." << std::endl;
    }
    std::cout << std::endl;
    std::cout << "Thank you for your purchase!" << std::endl;
    std::cout << "Press any key to continue and then N to go back\n";
    std::cin.get();
}


void Ui::chooseMovie()
{
    //Clear console
    system("cls");

    if (user->getIsAdmin())
    {
        adminMenu();
    }
    else if (user->getIsAdmin() == false)
    {
        nlohmann::json data = Utilities::loadFile("../assets/movies.json");

        std::cout << "Hello, would you like to see all available movies in all cinemas or a specific cinema's schedule\n[1] All cinemas \n[2] Specific cinema\n";
        char choiceSchedules;
        bool madeChoice = false;
        std::string cinemaName;
        std::cin >> choiceSchedules;
        std::cin.ignore();
        switch (choiceSchedules)
        {
        case '1':
            std::cout << "Listing all available movies:\n\n";

            for (auto& item : data) {
                if (item.contains("type") && item["type"] == "movie") {
                    std::cout << "- " << item["name"].get<std::string>() << " |Genre: " << item["genre"].get<std::string>() << "| |Show times: " << item["times"].get<std::string>() << "| |Release year: " << item["releaseDate"] << "| |Duration: " << item["duration"] << " minutes| " << " | Languages : " << item["languages"].get<std::string>() << "| |Cinema : " << item["cinema"].get<std::string>() << '|' << " | Hall: " << item["hall"].get<std::string>() << "|\n" << std::endl;
                }
            }
            madeChoice = true;
            break;
        case '2':
            std::cout << "Available Cinemas:\n";

            for (auto& item : data) {
                if (item.contains("type") && item["type"] == "cinema") {
                    std::cout << "- " << item["name"].get<std::string>() << "\n";
                }
            }
            std::cout << "\nEnter cinema: \n";
            std::getline(std::cin, cinemaName);
            std::cout << "Listing all available movies:\n\n";

            for (auto& item : data) {
                if ((item.contains("type") && item["type"] == "movie") && item["cinema"].get<std::string>() == cinemaName) {
                    std::cout << "- " << item["name"].get<std::string>() << " |Genre: " << item["genre"].get<std::string>() << "| |Show times: " << item["times"].get<std::string>() << "| |Release year: " << item["releaseDate"] << "| |Duration: " << item["duration"] << " minutes| " << " | Languages : " << item["languages"].get<std::string>() << "| |Cinema : " << item["cinema"].get<std::string>() << '|' << " | Hall: " << item["hall"].get<std::string>() << "|\n" << std::endl;
                }
            }
            madeChoice = true;
            break;
        default:
            std::cout << "Invalid choice. Press N to go back\n";
            break;
        }
        
        if (madeChoice)
        {
            std::cout << "[B] Book a movie\nFilter the search by: [1] Title | [2] Language | [3] Genre | [4] Release year | [5] Showtime\n";
            char choice;
            std::cin >> choice;
            std::cin.ignore();

            switch (choice)
            {
            case '1':
            {
                std::cout << "[1] Specific name [2] From A to Z\n";
                int choice;
                std::cin >> choice;
                std::cin.ignore();
                switch (choice)
                {
                case 1:
                {
                    std::cout << "Enter movie name: ";
                    std::string movieName;

                    while (true)
                    {
                        std::getline(std::cin, movieName);

                        if (movieName == "N" || movieName == "n") mainMenu();

                        bool found = false;

                        for (auto& item : data)
                        {
                            if (item.contains("type") && item["type"] == "movie")
                            {
                                if (item["name"].get<std::string>() == movieName)
                                {
                                    std::cout << "- " << item["name"].get<std::string>() << " |Genre: " << item["genre"].get<std::string>() << "| |Show times: " << item["times"].get<std::string>() << "| |Release year: " << item["releaseDate"] << "| |Duration: " << item["duration"] << " minutes| |Languages: " << item["languages"].get<std::string>() << "| |Cinema: " << item["cinema"].get<std::string>() << "| |Hall: " << item["hall"].get<std::string>() << "|" << std::endl;
                                    found = true;
                                }
                            }
                        }

                        if (!found)
                        {
                            std::cout << "We couldn't find a movie with that name.\n";
                        }

                        std::cout << "Press N to go back or enter another movie name\n";
                    }
                }
                case 2:
                {
                    std::vector<nlohmann::json> movies;

                    for (auto& item : data)
                    {
                        if (item.contains("type") && item["type"] == "movie")
                        {
                            movies.push_back(item);
                        }
                    }

                    std::sort(movies.begin(), movies.end());

                    std::cout << "\nMovies from A to Z:\n\n";

                    for (const auto& item : movies)
                    {
                        std::cout << "- " << item["name"].get<std::string>() << " |Genre: " << item["genre"].get<std::string>() << "| |Show times: " << item["times"].get<std::string>() << "| |Release year: " << item["releaseDate"] << "| |Duration: " << item["duration"] << " minutes| |Languages: " << item["languages"].get<std::string>() << "| |Cinema: " << item["cinema"].get<std::string>() << "| |Hall: " << item["hall"].get<std::string>() << "|" << std::endl;
                    }

                    std::cout << "\nPress N to go back.\n";
                    std::cin.get();
                    mainMenu();
                    break;
                }
                default:
                    std::cout << "Invalid choice.\n";
                    break;
                }
            }
            case '2':
            {
                std::cout << "Enter language(s): ";
                std::string language;

                while (true)
                {
                    std::getline(std::cin, language);

                    if (language == "N" || language == "n") mainMenu();

                    bool found = false;

                    for (auto& item : data)
                    {
                        if (item.contains("type") && item["type"] == "movie")
                        {
                            auto langs = splitLanguages(item["languages"].get<std::string>());

                            if (std::find(langs.begin(), langs.end(), language) != langs.end())
                            {
                                std::cout << "- " << item["name"].get<std::string>() << " |Genre: " << item["genre"].get<std::string>() << "| |Show times: " << item["times"].get<std::string>() << "| |Release year: " << item["releaseDate"] << "| |Duration: " << item["duration"] << " minutes| |Languages: " << item["languages"].get<std::string>() << "| |Cinema: " << item["cinema"].get<std::string>() << "| |Hall: " << item["hall"].get<std::string>() << "|" << std::endl;
                                found = true;
                            }
                        }
                    }

                    if (!found)
                    {
                        std::cout << "We couldn't find a movie with that language.\n";
                    }

                    std::cout << "Press N to go back or enter another movie language\n";
                }
            }
            case '3':
            {
                std::cout << "Enter genre: ";
                std::string genre;

                while (true)
                {
                    std::getline(std::cin, genre);

                    if (genre == "N" || genre == "n") mainMenu();

                    bool found = false;

                    for (auto& item : data)
                    {
                        if (item.contains("type") && item["type"] == "movie")
                        {
                            if (item["genre"].get<std::string>() == genre)
                            {
                                std::cout << "- " << item["name"].get<std::string>() << " |Genre: " << item["genre"].get<std::string>() << "| |Show times: " << item["times"].get<std::string>() << "| |Release year: " << item["releaseDate"] << "| |Duration: " << item["duration"] << " minutes| |Languages: " << item["languages"].get<std::string>() << "| |Cinema: " << item["cinema"].get<std::string>() << "| |Hall: " << item["hall"].get<std::string>() << "|" << std::endl;
                                found = true;
                            }
                        }
                    }

                    if (!found)
                    {
                        std::cout << "We couldn't find a movie with that genre.\n";
                    }

                    std::cout << "Press N to go back or enter another movie genre\n";
                }
            }
            case '4':
            {
                std::cout << "Enter release year: ";
                std::string releaseYear;

                while (true)
                {
                    std::getline(std::cin, releaseYear);
                    if (releaseYear == "N" || releaseYear == "n") mainMenu();
                    int releaseYearInt = std::stoi(releaseYear);

                    bool found = false;

                    for (auto& item : data)
                    {
                        if (item.contains("type") && item["type"] == "movie")
                        {
                            if (item["releaseDate"] == releaseYearInt)
                            {
                                std::cout << "- " << item["name"].get<std::string>() << " |Genre: " << item["genre"].get<std::string>() << "| |Show times: " << item["times"].get<std::string>() << "| |Release year: " << item["releaseDate"] << "| |Duration: " << item["duration"] << " minutes| |Languages: " << item["languages"].get<std::string>() << "| |Cinema: " << item["cinema"].get<std::string>() << "| |Hall: " << item["hall"].get<std::string>() << "|" << std::endl;
                                found = true;
                            }
                        }
                    }

                    if (!found)
                    {
                        std::cout << "We couldn't find a movie with that release year.\n";
                    }

                    std::cout << "Press N to go back or enter another movie release year\n";
                }
                break;
            }
            case '5':
            {
                std::cout << "Enter showtime (Example: 12:20) : ";
                std::string showtime;

                while (true)
                {
                    std::getline(std::cin, showtime);

                    if (showtime == "N" || showtime == "n") mainMenu();

                    bool found = false;

                    for (auto& item : data)
                    {
                        if (item.contains("type") && item["type"] == "movie")
                        {
                            auto langs = splitLanguages(item["times"].get<std::string>());

                            if (std::find(langs.begin(), langs.end(), showtime) != langs.end())
                            {
                                std::cout << "- " << item["name"].get<std::string>() << " |Genre: " << item["genre"].get<std::string>() << "| |Show times: " << item["times"].get<std::string>() << "| |Release year: " << item["releaseDate"] << "| |Duration: " << item["duration"] << " minutes| |Languages: " << item["languages"].get<std::string>() << "| |Cinema: " << item["cinema"].get<std::string>() << "| |Hall: " << item["hall"].get<std::string>() << "|" << std::endl;
                                found = true;
                            }
                        }
                    }

                    if (!found)
                    {
                        std::cout << "We couldn't find a movie that'll show at this exact hour.\n";
                    }

                    std::cout << "Press N to go back or enter another showtime\n";
                }
            }
            case 'B': case 'b' :
                Ui::bookMovie();
                break;
            }
        }
        }
    
    else
    {
        displayMessage("You have to log in to look at the available movies");
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
                editCinema();
                break;
            case '3':
                deleteCinema();
                break;
            case '4':
                addMovie();
                break;
            case '5':
                editMovie();
                break;
            case '6':
                    deleteMovie();
                    break;
            case '7':
                showList();
                break;
            case 'n' : case 'N':
                mainMenu();
                break;
            default:
                displayMessage("Invalid option. Please try again.");
                break;
        }
    }
}

void Ui::createCinema()
{
    system("cls");
    std::cout << "Create New Cinema\n\n";

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

void Ui::deleteCinema()
{
    system("cls");
    std::cout << "Delete Cinema\n\n";

    nlohmann::json data = Utilities::loadFile("../assets/movies.json");
   
    // Show available cinemas
    std::cout << "Available Cinemas:\n";

    for (auto& item : data) {
        if (item.contains("type") && item["type"] == "cinema") {
            std::cout << "- " << item["name"].get<std::string>() << "\n";
        }
    }

    std::cout << "\nEnter cinema name to delete: ";
    std::string cinemaName;
    std::cin.ignore();
    std::getline(std::cin, cinemaName);

    // Remove cinema and movies
    auto it = data.begin();
    bool found = false;

    while (it != data.end()) {
        if (it->contains("type") &&
            (((*it)["type"] == "cinema" && (*it)["name"] == cinemaName) ||
                ((*it)["type"] == "movie" && (*it)["cinema"] == cinemaName))) {
            it = data.erase(it);
            found = true;
        }
        else {
            ++it;
        }
    }

    if (found) {
        std::ofstream file("../assets/movies.json");
        file << data.dump(1);
        file.close();
        std::cout << "Cinema deleted successfully!";
    }
    else {
        std::cout <<  "Cinema not found.";
    }

    char choice;
    std::cout << "\nPress [N] to go back\n";
    std::cin >> choice;
}

void Ui::editCinema()
{
    system("cls");
    std::cout << "Edit Cinema\n\n";

    nlohmann::json data = Utilities::loadFile("../assets/movies.json");

    //Show availables movies
    for (auto& item : data)
    {
        if (item.contains("type") && item["type"] == "cinema")
        {
            std::cout << "- " << item["name"].get<std::string>() << "\n";
        }
    }

    std::cout << "Enter cinema to edit: ";
    std::string cinemaName;
    std::cin.ignore();
    std::getline(std::cin, cinemaName);

    bool found = false;
    for (auto& item : data)
    {
        if (item.contains("type") && item["type"] == "cinema")
        {
            if (item["name"].get<std::string>() == cinemaName)
            {
                found = true;

                std::cout << "Edit name: ";
                std::string newName;
                std::getline(std::cin, newName);
                
                std::cout << "Edit location: ";
                std::string newLocation;
                std::getline(std::cin, newLocation);

                std::cout << "New number halls: ";
                int newHalls;
                std::cin >> newHalls;

                item["name"] = newName;
                item["location"] = newLocation;
                item["numHalls"] = newHalls;
            }
        }
    }

    if (found)
    {
        std::ofstream file("../assets/movies.json");
        file << data.dump(1);
        file.close();
        std::cout << "Cinema edited";
    }
    else std::cout << "Cinema not found";

    char choice;
    std::cout << "\nPress [N] to go back\n";
    std::cin >> choice;
}

void Ui::addMovie()
{
    system("cls");
    std::cout << "Add New Movie\n\n";

    std::string name, genre, languages, description, cinema, hall, times;
    int duration, releaseDate;

    std::cin.ignore();
    std::cout << "Enter movie name: ";
    std::getline(std::cin, name);

    std::cout << "Enter genre: ";
    std::getline(std::cin, genre);

    std::cout << "Enter release year: ";
    std::cin >> releaseDate;
    std::cin.ignore();

    std::cout << "Enter duration (minutes): ";
    std::cin >> duration;
    std::cin.ignore();

    std::cout << "Enter languages (comma separated): ";
    std::getline(std::cin, languages);

    std::cout << "Enter description: ";
    std::getline(std::cin, description);

    // Load available cinemas
    nlohmann::json data = Utilities::loadFile("../assets/movies.json");
    std::cout << "\nAvailable Cinemas:\n";
    for (auto& item : data) {
        if (item.contains("type") && item["type"] == "cinema") {
            std::cout << "- " << item["name"].get<std::string>() << "\n";
        }
    }

    std::cout << "\nEnter cinema name: ";
    std::getline(std::cin, cinema);

    std::cout << "Enter hall number: ";
    std::getline(std::cin, hall);

    std::cout << "Enter show times (comma separated): ";
    std::getline(std::cin, times);

    nlohmann::json movie;
    movie["name"] = name;
    movie["genre"] = genre;
    movie["duration"] = duration;
    movie["languages"] = languages;
    movie["description"] = description;
    movie["cinema"] = cinema;
    movie["hall"] = hall;
    movie["times"] = times;
    movie["releaseDate"] = releaseDate;
    movie["type"] = "movie";

    Utilities::saveToFile("../assets/movies.json", movie);
    displayMessage("Movie added successfully!");
}

void Ui::editMovie()
{
    system("cls");
    std::cout << "Edit Movie\n\n";

    nlohmann::json data = Utilities::loadFile("../assets/movies.json");

    //Show availables movies
    for (auto& item : data)
    {
        if (item.contains("type") && item["type"] == "movie")
        {
            std::cout << "- " << item["name"].get<std::string>() << "\n";
        }
    }

    std::cout << "Enter movie to edit: ";
    std::string movieName;
    std::cin.ignore();
    std::getline(std::cin, movieName);

    bool found = false;
    for (auto& item : data)
    {
        if (item.contains("type") && item["type"] == "movie")
        {
            if (item["name"].get<std::string>() == movieName)
            {
                found = true;

                std::cout << "Edit name: ";
                std::string newName;
                std::getline(std::cin, newName);

                std::cout << "Edit genre: ";
                std::string newGenre;
                std::getline(std::cin, newGenre);

                std::cout << "Edit release year: ";
                int newReleaseDate;
                std::cin >> newReleaseDate;

                std::cout << "Edit duration: ";
                int newDuration;
                std::cin >> newDuration;

                std::cin.ignore();
                std::cout << "Edit languages: ";
                std::string newLanguages;
                std::getline(std::cin, newLanguages);


                std::cout << "Edit description: ";
                std::string newDescription;
                std::getline(std::cin, newDescription);

                std::cout << "\nAvailable cinemas:\n";

                for (auto& item : data) {
                    if (item.contains("type") && item["type"] == "cinema") {
                        std::cout << "- " << item["name"].get<std::string>() << std::endl;
                        std::cout << "-- Number of halls: " << item["numHalls"].get<int>() << std::endl;
                    }
                }

                std::cout << "Edit cinema: ";
                std::string newCinema;
                std::getline(std::cin, newCinema);

                std::cout << "Edit hall (which hall to play the movie in): ";
                std::string newHall;
                std::getline(std::cin, newHall);

                std::cout << "Edit times (to play) : ";
                std::string newTimes;
                std::getline(std::cin, newTimes);

                item["name"] = newName;
                item["genre"] = newGenre;
                item["duration"] = newDuration;
                item["languages"] = newLanguages;
                item["description"] = newDescription;
                item["cinema"] = newCinema;
                item["hall"] = newHall;
                item["times"] = newTimes;
                item["releaseDate"] = newReleaseDate;
                
            }
        }
    }

    if (found)
    {
        std::ofstream file("../assets/movies.json");
        file << data.dump(1);
        file.close();
        std::cout << "Movie edited";
    }
    else std::cout << "Movie not found";

    char choice;
    std::cout << "\nPress [N] to go back\n";
    std::cin >> choice;
}

void Ui::deleteMovie()
{
    system("cls");
    std::cout << "Delete movie\n\n";

    nlohmann::json data = Utilities::loadFile("../assets/movies.json");

    // Show available cinemas
    std::cout << "Available movies:\n";

    for (auto& item : data) {
        if (item.contains("type") && item["type"] == "movie") {
            std::cout << "- " << item["name"].get<std::string>() << "\n";
        }
    }

    std::cout << "\nEnter movie name to delete: ";
    std::string movieName;
    std::cin.ignore();
    std::getline(std::cin, movieName);

    auto it = data.begin();
    bool found = false;

    while (it != data.end()) {
        if (it->contains("type") &&
            (((*it)["type"] == "movie" && (*it)["name"] == movieName))) {
            it = data.erase(it);
            found = true;
        }
        else {
            ++it;
        }
    }

    if (found) {
        std::ofstream file("../assets/movies.json");
        file << data.dump(1);
        file.close();
        std::cout << "Movie deleted successfully!";
    }
    else {
        std::cout << "Movie not found.";
    }

    char choice;
    std::cout << "\nPress [N] to go back\n";
    std::cin >> choice;
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

void Ui::showList()
{
    system("cls");
    std::cout << "Cinemas and Movies List\n\n";

    nlohmann::json data = Utilities::loadFile("../assets/movies.json");
    
    std::cout << "CINEMAS:\n";
    std::cout << "---------\n\n";
    bool hasCinemas = false;
    
    for (auto& item : data) {
        if (item.contains("type") && item["type"] == "cinema") {
            hasCinemas = true;
            std::cout << "Name: " << item["name"].get<std::string>() << "\n";
            std::cout << "Location: " << item["location"].get<std::string>() << "\n";
            std::cout << "Number of Halls: " << item["numHalls"].get<int>() << "\n";
            std::cout << "------------------------\n";
        }
    }

    if (!hasCinemas) {
        std::cout << "No cinemas added yet.\n\n";
    }

    std::cout << "\nMOVIES:\n";
    std::cout << "---------\n\n";
    bool hasMovies = false;

    for (auto& item : data) {
        if (item.contains("type") && item["type"] == "movie") {
            hasMovies = true;
            std::cout << "Name: " << item["name"].get<std::string>() << "\n";
            std::cout << "Genre: " << item["genre"].get<std::string>() << "\n";
            std::cout << "Duration: " << item["duration"].get<int>() << " minutes\n";
            std::cout << "Languages: " << item["languages"].get<std::string>() << "\n";
            std::cout << "Description: " << item["description"].get<std::string>() << "\n";
            std::cout << "Cinema: " << item["cinema"].get<std::string>() << "\n";
            std::cout << "Hall: " << item["hall"].get<std::string>() << "\n";
            std::cout << "Show Times: " << item["times"].get<std::string>() << "\n";
            std::cout << "------------------------\n";
        }
    }

    if (!hasMovies) {
        std::cout << "No movies added yet.\n";
    }

    char choice;
    std::cout << "\nPress N to go back\n";
    std::cin >> choice;
}