#ifndef DATA_LOADER_H
#define DATA_LOADER_H

#include "EventMatcher.h"
#include <string>

class DataLoader {
private:
    static std::unordered_set<std::string> splitSet(const std::string& text, char delimiter);
    static std::string trim(const std::string& text);

public:
    static bool loadUsers(const std::string& filePath, EventMatcher& matcher);
    static bool loadFriendships(const std::string& filePath, EventMatcher& matcher);
};

#endif
