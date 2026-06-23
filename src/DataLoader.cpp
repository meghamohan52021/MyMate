#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>

std::string DataLoader::trim(const std::string& text) {
    int left = 0;
    int right = static_cast<int>(text.size()) - 1;

    while (left <= right && std::isspace(static_cast<unsigned char>(text[left]))) left++;
    while (right >= left && std::isspace(static_cast<unsigned char>(text[right]))) right--;

    if (left > right) return "";
    return text.substr(left, right - left + 1);
}

std::unordered_set<std::string> DataLoader::splitSet(const std::string& text, char delimiter) {
    std::unordered_set<std::string> result;
    std::stringstream stream(text);
    std::string item;

    while (std::getline(stream, item, delimiter)) {
        item = trim(item);
        if (!item.empty()) result.insert(item);
    }

    return result;
}

bool DataLoader::loadUsers(const std::string& filePath, EventMatcher& matcher) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "Could not open users file: " << filePath << "\n";
        return false;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::vector<std::string> columns;
        std::stringstream stream(line);
        std::string value;

        while (std::getline(stream, value, ',')) {
            columns.push_back(trim(value));
        }

        if (columns.size() != 7) continue;

        int id = std::stoi(columns[0]);
        std::string name = columns[1];
        int comfortLevel = std::stoi(columns[2]);
        int preferredGroupSize = std::stoi(columns[3]);

        User user(id, name, comfortLevel, preferredGroupSize);
        user.interests = splitSet(columns[4], '|');
        user.availability = splitSet(columns[5], '|');
        user.events = splitSet(columns[6], '|');

        matcher.addUser(user);
    }

    matcher.buildEventIndex();
    return true;
}

bool DataLoader::loadFriendships(const std::string& filePath, EventMatcher& matcher) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cout << "Could not open friendships file: " << filePath << "\n";
        return false;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream stream(line);
        std::string first;
        std::string second;

        std::getline(stream, first, ',');
        std::getline(stream, second, ',');

        if (!first.empty() && !second.empty()) {
            matcher.addFriendship(std::stoi(trim(first)), std::stoi(trim(second)));
        }
    }

    return true;
}
