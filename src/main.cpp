#include "EventMatcher.h"
#include "DataLoader.h"
#include <iostream>
#include <limits>
#include <string>

void showMenu() {
    std::cout << "\nEventMate - Event Buddy Matcher\n";
    std::cout << "1. Show stats\n";
    std::cout << "2. Show events\n";
    std::cout << "3. Find top buddy matches\n";
    std::cout << "4. Create event groups\n";
    std::cout << "5. Show user profile\n";
    std::cout << "6. Export matches to CSV\n";
    std::cout << "7. Export groups to CSV\n";
    std::cout << "8. Search users by name\n";
    std::cout << "9. Show event attendees\n";
    std::cout << "10. Recommend events for user\n";
    std::cout << "11. Export profiles to CSV\n";
    std::cout << "0. Exit\n";
    std::cout << "Choice: ";
}

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string readLine(const std::string& label) {
    std::string value;
    std::cout << label;
    std::getline(std::cin, value);
    return value;
}

int main() {
    EventMatcher matcher;

    bool loaded = DataLoader::loadFormResponses("data/form_responses.csv", matcher);
    if (!loaded) {
        loaded = DataLoader::loadUsers("data/users.csv", matcher);
        DataLoader::loadFriendships("data/friendships.csv", matcher);
    }

    if (!loaded) {
        std::cout << "No data loaded. Run the program from the project root folder.\n";
        return 1;
    }

    matcher.printStats();

    int choice;
    while (true) {
        showMenu();
        std::cin >> choice;

        if (std::cin.fail()) {
            clearInput();
            std::cout << "Invalid input.\n";
            continue;
        }

        clearInput();
        if (choice == 0) break;

        if (choice == 1) {
            matcher.printStats();
        } else if (choice == 2) {
            std::vector<std::string> events = matcher.getEvents();
            std::cout << "\nAvailable events:\n";
            for (const std::string& eventName : events) {
                std::cout << "- " << eventName << " (" << matcher.getAttendees(eventName).size() << " people)\n";
            }
        } else if (choice == 3) {
            int userId;
            int limit;
            std::cout << "User ID: ";
            std::cin >> userId;
            clearInput();
            std::string eventName = readLine("Event name: ");
            std::cout << "Number of matches: ";
            std::cin >> limit;
            clearInput();
            matcher.printTopMatches(userId, eventName, limit);
        } else if (choice == 4) {
            std::string eventName = readLine("Event name: ");
            double threshold;
            int maxGroupSize;
            std::cout << "Minimum compatibility threshold, example 0.45: ";
            std::cin >> threshold;
            std::cout << "Maximum group size: ";
            std::cin >> maxGroupSize;
            clearInput();
            matcher.printGroups(eventName, threshold, maxGroupSize);
        } else if (choice == 5) {
            int userId;
            std::cout << "User ID: ";
            std::cin >> userId;
            clearInput();
            matcher.printUser(userId);
        } else if (choice == 6) {
            std::string eventName = readLine("Event name: ");
            int limit;
            std::cout << "Number of matches per user: ";
            std::cin >> limit;
            clearInput();
            std::string filePath = "output/matches.csv";
            if (matcher.exportMatches(eventName, limit, filePath)) {
                std::cout << "Saved to " << filePath << "\n";
            } else {
                std::cout << "Could not export matches. Check event name.\n";
            }
        } else if (choice == 7) {
            std::string eventName = readLine("Event name: ");
            double threshold;
            int maxGroupSize;
            std::cout << "Minimum compatibility threshold, example 0.45: ";
            std::cin >> threshold;
            std::cout << "Maximum group size: ";
            std::cin >> maxGroupSize;
            clearInput();
            std::string filePath = "output/groups.csv";
            if (matcher.exportGroups(eventName, threshold, maxGroupSize, filePath)) {
                std::cout << "Saved to " << filePath << "\n";
            } else {
                std::cout << "Could not export groups. Check event name.\n";
            }
        } else if (choice == 8) {
            std::string keyword = readLine("Search name: ");
            matcher.printUserSearch(keyword);
        } else if (choice == 9) {
            std::string eventName = readLine("Event name: ");
            matcher.printEventAttendees(eventName);
        } else if (choice == 10) {
            int userId;
            int limit;
            std::cout << "User ID: ";
            std::cin >> userId;
            std::cout << "Number of recommendations: ";
            std::cin >> limit;
            clearInput();
            matcher.printEventRecommendations(userId, limit);
        } else if (choice == 11) {
            std::string filePath = "output/profiles.csv";
            if (matcher.exportProfiles(filePath)) {
                std::cout << "Saved to " << filePath << "\n";
            } else {
                std::cout << "Could not export profiles.\n";
            }
        } else {
            std::cout << "Invalid choice.\n";
        }
    }

    return 0;
}
