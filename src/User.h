#ifndef USER_H
#define USER_H

#include <string>
#include <unordered_set>

class User {
public:
    int id;
    std::string name;
    int comfortLevel;
    int preferredGroupSize;
    std::unordered_set<std::string> interests;
    std::unordered_set<std::string> availability;
    std::unordered_set<std::string> events;
    std::unordered_set<int> friends;

    User();
    User(int id, const std::string& name, int comfortLevel, int preferredGroupSize);
};

#endif
