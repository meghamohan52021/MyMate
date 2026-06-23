#include "DataLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cctype>
#include <unordered_map>

std::string DataLoader::trim(const std::string& text) {
    int left = 0;
    int right = static_cast<int>(text.size()) - 1;
    while (left <= right && std::isspace(static_cast<unsigned char>(text[left]))) left++;
    while (right >= left && std::isspace(static_cast<unsigned char>(text[right]))) right--;
    if (left > right) return "";
    return text.substr(left, right - left + 1);
}

std::string DataLoader::normalizeHeader(const std::string& text) {
    std::string result;
    for (char ch : text) {
        if (std::isalnum(static_cast<unsigned char>(ch))) {
            result += static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
        } else if (ch == '_' || ch == ' ') {
            result += '_';
        }
    }
    while (result.find("__") != std::string::npos) {
        result.replace(result.find("__"), 2, "_");
    }
    if (!result.empty() && result.front() == '_') result.erase(result.begin());
    if (!result.empty() && result.back() == '_') result.pop_back();
    return result;
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

std::vector<std::string> DataLoader::parseCsvLine(const std::string& line) {
    std::vector<std::string> columns;
    std::string current;
    bool inQuotes = false;

    for (int i = 0; i < static_cast<int>(line.size()); i++) {
        char ch = line[i];
        if (ch == '"') {
            if (inQuotes && i + 1 < static_cast<int>(line.size()) && line[i + 1] == '"') {
                current += '"';
                i++;
            } else {
                inQuotes = !inQuotes;
            }
        } else if (ch == ',' && !inQuotes) {
            columns.push_back(trim(current));
            current.clear();
        } else {
            current += ch;
        }
    }
    columns.push_back(trim(current));
    return columns;
}

int DataLoader::toIntOrDefault(const std::string& text, int fallback) {
    try {
        return std::stoi(trim(text));
    } catch (...) {
        return fallback;
    }
}

bool DataLoader::loadFormResponses(const std::string& filePath, EventMatcher& matcher) {
    std::ifstream file(filePath);
    if (!file.is_open()) return false;

    std::string line;
    if (!std::getline(file, line)) return false;

    std::vector<std::string> headers = parseCsvLine(line);
    std::unordered_map<std::string, int> index;
    for (int i = 0; i < static_cast<int>(headers.size()); i++) {
        index[normalizeHeader(headers[i])] = i;
    }

    auto valueAt = [](const std::vector<std::string>& row, int pos) {
        if (pos < 0 || pos >= static_cast<int>(row.size())) return std::string("");
        return row[pos];
    };

    auto getIndex = [&](const std::vector<std::string>& names) {
        for (const std::string& name : names) {
            auto it = index.find(name);
            if (it != index.end()) return it->second;
        }
        return -1;
    };

    int nameCol = getIndex({"name", "full_name", "student_name"});
    int comfortCol = getIndex({"comfort_level", "comfort"});
    int groupCol = getIndex({"preferred_group_size", "group_size", "preferred_groupsize"});
    int interestCol = getIndex({"interests", "interest"});
    int availabilityCol = getIndex({"availability", "available_times", "available_time_slots"});
    int eventCol = getIndex({"events", "event_choices", "event"});
    int friendsCol = getIndex({"friends", "friend_names", "mutual_friends"});

    if (nameCol == -1 || interestCol == -1 || availabilityCol == -1 || eventCol == -1) {
        std::cout << "Form CSV needs Name, Interests, Availability, and Events columns.\n";
        return false;
    }

    std::vector<std::pair<std::string, std::unordered_set<std::string>>> pendingFriends;
    int id = 1;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::vector<std::string> row = parseCsvLine(line);

        std::string name = valueAt(row, nameCol);
        if (name.empty()) continue;

        int comfort = toIntOrDefault(valueAt(row, comfortCol), 3);
        int groupSize = toIntOrDefault(valueAt(row, groupCol), 3);
        comfort = std::max(1, std::min(5, comfort));
        groupSize = std::max(2, std::min(8, groupSize));

        User user(id, name, comfort, groupSize);
        user.interests = splitSet(valueAt(row, interestCol), '|');
        user.availability = splitSet(valueAt(row, availabilityCol), '|');
        user.events = splitSet(valueAt(row, eventCol), '|');

        matcher.addUser(user);
        if (friendsCol != -1) {
            pendingFriends.push_back({name, splitSet(valueAt(row, friendsCol), '|')});
        }
        id++;
    }

    for (const auto& item : pendingFriends) {
        for (const std::string& friendName : item.second) {
            matcher.addFriendshipByName(item.first, friendName);
        }
    }

    matcher.buildEventIndex();
    return true;
}

bool DataLoader::loadUsers(const std::string& filePath, EventMatcher& matcher) {
    std::ifstream file(filePath);
    if (!file.is_open()) return false;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::vector<std::string> columns = parseCsvLine(line);
        if (columns.size() < 7) continue;

        int id = toIntOrDefault(columns[0], -1);
        if (id == -1) continue;

        User user(id, columns[1], toIntOrDefault(columns[2], 3), toIntOrDefault(columns[3], 3));
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
    if (!file.is_open()) return false;

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::vector<std::string> columns = parseCsvLine(line);
        if (columns.size() < 2) continue;
        matcher.addFriendship(toIntOrDefault(columns[0], -1), toIntOrDefault(columns[1], -1));
    }

    return true;
}
