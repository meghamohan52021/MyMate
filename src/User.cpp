#include "User.h"

User::User() : id(0), comfortLevel(1), preferredGroupSize(2) {}

User::User(int id, std::string name, int comfortLevel, int preferredGroupSize)
    : id(id), name(name), comfortLevel(comfortLevel), preferredGroupSize(preferredGroupSize) {}
