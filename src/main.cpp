#include "EventMatcher.h"
#include "DataLoader.h"
#include <iostream>
#include <limits>

void showMenu() {
    std::cout << "\nEventMate - Event Buddy Matcher\n";
    std::cout << "1. Show events\n";
    std::cout << "2. Find top buddy matches\n";
    std::cout << "3. Create event groups\n";
    std::cout << "4. Show user profile\n";
    std::cout << "0. Exit\n";
    std::cout << "Choice: ";
}

int main() {
    EventMatcher matcher;

    bool usersLoaded = DataLoader::loadUsers("data/users.csv", matcher);
    bool friendshipsLoaded = DataLoader::loadFriendships("data/friendships.csv", matcher);

    if (!usersLoaded || !friendshipsLoaded) {
        std::cout << "Run the program from the project root folder.\n";
        return 1;
    }

    int choice;

    while (true) {
        showMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input.\n";
            continue;
        }

        if (choice == 0) break;

        if (choice == 1) {
            std::vector<std::string> events = matcher.getEvents();
            std::cout << "\nAvailable events:\n";
            for (const std::string& eventName : events) {
                std::cout << "- " << eventName << "\n";
            }
        } else if (choice == 2) {
            int userId;
            int limit;
            std::string eventName;

            std::cout << "User ID: ";
            std::cin >> userId;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Event name: ";
            std::getline(std::cin, eventName);
            std::cout << "Number of matches: ";
            std::cin >> limit;

            matcher.printTopMatches(userId, eventName, limit);
        } else if (choice == 3) {
            std::string eventName;
            double threshold;
            int maxGroupSize;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Event name: ";
            std::getline(std::cin, eventName);
            std::cout << "Minimum compatibility threshold, example 0.45: ";
            std::cin >> threshold;
            std::cout << "Maximum group size: ";
            std::cin >> maxGroupSize;

            matcher.printGroups(eventName, threshold, maxGroupSize);
        } else if (choice == 4) {
            int userId;
            std::cout << "User ID: ";
            std::cin >> userId;
            matcher.printUser(userId);
        } else {
            std::cout << "Invalid choice.\n";
        }
    }

    return 0;
}
