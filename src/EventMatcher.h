#ifndef EVENT_MATCHER_H
#define EVENT_MATCHER_H

#include "User.h"
#include "DSU.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct MatchResult {
    int userA;
    int userB;
    double score;
    int commonInterests;
    int commonAvailability;
    int mutualFriends;
};

struct MatchCompare {
    bool operator()(const MatchResult& a, const MatchResult& b) const {
        return a.score < b.score;
    }
};

class EventMatcher {
private:
    std::unordered_map<int, User> users;
    std::unordered_map<std::string, std::vector<int>> eventUsers;

    double setSimilarity(const std::unordered_set<std::string>& a,
                         const std::unordered_set<std::string>& b) const;

    int intersectionSize(const std::unordered_set<int>& a,
                         const std::unordered_set<int>& b) const;

    int intersectionSize(const std::unordered_set<std::string>& a,
                         const std::unordered_set<std::string>& b) const;

    double mutualFriendScore(const User& a, const User& b) const;
    double comfortScore(const User& a, const User& b) const;
    MatchResult compareUsers(int idA, int idB) const;

public:
    void addUser(const User& user);
    void addFriendship(int userA, int userB);
    void buildEventIndex();
    std::vector<MatchResult> getTopMatches(int userId, const std::string& eventName, int limit) const;
    std::vector<std::vector<int>> createGroups(const std::string& eventName, double threshold, int maxGroupSize) const;
    void printUser(int userId) const;
    void printTopMatches(int userId, const std::string& eventName, int limit) const;
    void printGroups(const std::string& eventName, double threshold, int maxGroupSize) const;
    bool hasUser(int userId) const;
    bool hasEvent(const std::string& eventName) const;
    std::vector<std::string> getEvents() const;
};

#endif
