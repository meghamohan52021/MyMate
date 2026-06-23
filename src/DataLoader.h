#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include "EventMatcher.h"
#include <string>
#include <unordered_set>
#include <vector>

class DataLoader {
private:
    static std::string trim(const std::string& text);
    static std::unordered_set<std::string> splitSet(const std::string& text, char delimiter);
    static std::vector<std::string> parseCsvLine(const std::string& line);
    static int toIntOrDefault(const std::string& text, int fallback);
    static std::string normalizeHeader(const std::string& text);

public:
    static bool loadFormResponses(const std::string& filePath, EventMatcher& matcher);
    static bool loadUsers(const std::string& filePath, EventMatcher& matcher);
    static bool loadFriendships(const std::string& filePath, EventMatcher& matcher);
};

#endif
