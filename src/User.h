#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <unordered_set>

class User {
public:
    int id;
    std::string name;
    int comfortLevel;
    int preferredGroupSize;
    std::unordered_set<std::string> interests;
    std::unordered_set<std::string> availability;
    std::unordered_set<int> friends;
    std::unordered_set<std::string> events;

    User();
    User(int id, std::string name, int comfortLevel, int preferredGroupSize);
};

#endif
