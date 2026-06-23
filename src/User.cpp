#include "User.h"

User::User() : id(0), comfortLevel(3), preferredGroupSize(3) {}

User::User(int id, const std::string& name, int comfortLevel, int preferredGroupSize)
    : id(id), name(name), comfortLevel(comfortLevel), preferredGroupSize(preferredGroupSize) {}
