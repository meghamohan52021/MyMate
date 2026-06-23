#ifndef EVENT_MATCHER_H
#define EVENT_MATCHER_H

#include "User.h"
#include "DSU.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

struct MatchResult {
    int userA;
    int userB;
    double score;
    int commonInterests;
    int commonAvailability;
    int mutualFriends;
};

struct MatchCompareMin {
    bool operator()(const MatchResult& a, const MatchResult& b) const {
        return a.score > b.score;
    }
};

struct MatchCompareMax {
    bool operator()(const MatchResult& a, const MatchResult& b) const {
        return a.score < b.score;
    }
};

class EventMatcher {
private:
    std::unordered_map<int, User> users;
    std::unordered_map<std::string, std::vector<int>> eventUsers;
    std::unordered_map<std::string, int> nameToId;

    double setSimilarity(const std::unordered_set<std::string>& a,
                         const std::unordered_set<std::string>& b) const;

    int intersectionSize(const std::unordered_set<int>& a,
                         const std::unordered_set<int>& b) const;

    int intersectionSize(const std::unordered_set<std::string>& a,
                         const std::unordered_set<std::string>& b) const;

    double mutualFriendScore(const User& a, const User& b) const;
    double comfortScore(const User& a, const User& b) const;
    MatchResult compareUsers(int idA, int idB) const;
    std::string csvEscape(const std::string& text) const;
    std::string toLower(const std::string& text) const;
    std::string joinStrings(const std::unordered_set<std::string>& values) const;
    std::string joinFriendNames(const std::unordered_set<int>& friendIds) const;

public:
    void addUser(const User& user);
    void addFriendship(int userA, int userB);
    bool addFriendshipByName(const std::string& nameA, const std::string& nameB);
    void buildEventIndex();

    bool hasUser(int userId) const;
    bool hasEvent(const std::string& eventName) const;
    int userCount() const;
    int eventCount() const;
    int getUserIdByName(const std::string& name) const;
    std::vector<std::string> getEvents() const;
    std::vector<int> getAttendees(const std::string& eventName) const;
    std::vector<int> searchUsersByName(const std::string& keyword) const;
    std::vector<std::pair<std::string, double>> recommendEventsForUser(int userId, int limit) const;

    std::vector<MatchResult> getTopMatches(int userId, const std::string& eventName, int limit) const;
    std::vector<std::vector<int>> createGroups(const std::string& eventName, double threshold, int maxGroupSize) const;

    void printUser(int userId) const;
    void printUserSearch(const std::string& keyword) const;
    void printEventAttendees(const std::string& eventName) const;
    void printEventRecommendations(int userId, int limit) const;
    void printTopMatches(int userId, const std::string& eventName, int limit) const;
    void printGroups(const std::string& eventName, double threshold, int maxGroupSize) const;
    void printStats() const;

    bool exportMatches(const std::string& eventName, int limit, const std::string& filePath) const;
    bool exportGroups(const std::string& eventName, double threshold, int maxGroupSize, const std::string& filePath) const;
    bool exportProfiles(const std::string& filePath) const;
};

#endif
