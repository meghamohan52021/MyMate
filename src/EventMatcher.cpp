#include "EventMatcher.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <map>
#include <iomanip>
#include <sstream>
#include <cmath>

void EventMatcher::addUser(const User& user) {
    users[user.id] = user;
    nameToId[user.name] = user.id;
}

void EventMatcher::addFriendship(int userA, int userB) {
    if (!hasUser(userA) || !hasUser(userB) || userA == userB) return;
    users[userA].friends.insert(userB);
    users[userB].friends.insert(userA);
}

bool EventMatcher::addFriendshipByName(const std::string& nameA, const std::string& nameB) {
    int idA = getUserIdByName(nameA);
    int idB = getUserIdByName(nameB);
    if (idA == -1 || idB == -1 || idA == idB) return false;
    addFriendship(idA, idB);
    return true;
}

bool EventMatcher::hasUser(int userId) const {
    return users.find(userId) != users.end();
}

bool EventMatcher::hasEvent(const std::string& eventName) const {
    return eventUsers.find(eventName) != eventUsers.end();
}

int EventMatcher::userCount() const {
    return static_cast<int>(users.size());
}

int EventMatcher::eventCount() const {
    return static_cast<int>(eventUsers.size());
}

int EventMatcher::getUserIdByName(const std::string& name) const {
    auto it = nameToId.find(name);
    if (it == nameToId.end()) return -1;
    return it->second;
}

std::vector<std::string> EventMatcher::getEvents() const {
    std::vector<std::string> result;
    for (const auto& item : eventUsers) result.push_back(item.first);
    std::sort(result.begin(), result.end());
    return result;
}

std::vector<int> EventMatcher::getAttendees(const std::string& eventName) const {
    if (!hasEvent(eventName)) return {};
    return eventUsers.at(eventName);
}

void EventMatcher::buildEventIndex() {
    eventUsers.clear();
    for (const auto& item : users) {
        const User& user = item.second;
        for (const std::string& eventName : user.events) {
            eventUsers[eventName].push_back(user.id);
        }
    }
    for (auto& item : eventUsers) {
        std::sort(item.second.begin(), item.second.end());
    }
}

int EventMatcher::intersectionSize(const std::unordered_set<std::string>& a,
                                   const std::unordered_set<std::string>& b) const {
    const auto& smaller = a.size() < b.size() ? a : b;
    const auto& larger = a.size() < b.size() ? b : a;
    int count = 0;
    for (const auto& value : smaller) {
        if (larger.find(value) != larger.end()) count++;
    }
    return count;
}

int EventMatcher::intersectionSize(const std::unordered_set<int>& a,
                                   const std::unordered_set<int>& b) const {
    const auto& smaller = a.size() < b.size() ? a : b;
    const auto& larger = a.size() < b.size() ? b : a;
    int count = 0;
    for (int value : smaller) {
        if (larger.find(value) != larger.end()) count++;
    }
    return count;
}

double EventMatcher::setSimilarity(const std::unordered_set<std::string>& a,
                                   const std::unordered_set<std::string>& b) const {
    if (a.empty() && b.empty()) return 1.0;
    int common = intersectionSize(a, b);
    int total = static_cast<int>(a.size() + b.size() - common);
    if (total == 0) return 0.0;
    return static_cast<double>(common) / total;
}

double EventMatcher::mutualFriendScore(const User& a, const User& b) const {
    int mutual = intersectionSize(a.friends, b.friends);
    int total = static_cast<int>(a.friends.size() + b.friends.size() - mutual);
    if (total == 0) return 0.0;
    return static_cast<double>(mutual) / total;
}

double EventMatcher::comfortScore(const User& a, const User& b) const {
    int difference = std::abs(a.comfortLevel - b.comfortLevel);
    if (difference == 0) return 1.0;
    if (difference == 1) return 0.75;
    if (difference == 2) return 0.45;
    return 0.20;
}

MatchResult EventMatcher::compareUsers(int idA, int idB) const {
    const User& a = users.at(idA);
    const User& b = users.at(idB);

    double interest = setSimilarity(a.interests, b.interests);
    double availability = setSimilarity(a.availability, b.availability);
    double mutual = mutualFriendScore(a, b);
    double comfort = comfortScore(a, b);

    double finalScore = (0.40 * interest) +
                        (0.30 * availability) +
                        (0.20 * mutual) +
                        (0.10 * comfort);

    return {idA, idB, finalScore,
            intersectionSize(a.interests, b.interests),
            intersectionSize(a.availability, b.availability),
            intersectionSize(a.friends, b.friends)};
}

std::vector<MatchResult> EventMatcher::getTopMatches(int userId, const std::string& eventName, int limit) const {
    if (!hasUser(userId) || !hasEvent(eventName) || limit <= 0) return {};

    std::priority_queue<MatchResult, std::vector<MatchResult>, MatchCompareMin> heap;

    for (int otherId : eventUsers.at(eventName)) {
        if (otherId == userId) continue;
        MatchResult match = compareUsers(userId, otherId);
        heap.push(match);
        if (static_cast<int>(heap.size()) > limit) heap.pop();
    }

    std::vector<MatchResult> result;
    while (!heap.empty()) {
        result.push_back(heap.top());
        heap.pop();
    }

    std::sort(result.begin(), result.end(), [](const MatchResult& a, const MatchResult& b) {
        return a.score > b.score;
    });

    return result;
}

std::vector<std::vector<int>> EventMatcher::createGroups(const std::string& eventName, double threshold, int maxGroupSize) const {
    if (!hasEvent(eventName) || maxGroupSize < 2) return {};

    const std::vector<int>& attendees = eventUsers.at(eventName);
    std::unordered_map<int, int> idToIndex;
    std::unordered_map<int, int> indexToId;

    for (int i = 0; i < static_cast<int>(attendees.size()); i++) {
        idToIndex[attendees[i]] = i;
        indexToId[i] = attendees[i];
    }

    std::vector<MatchResult> pairs;
    for (int i = 0; i < static_cast<int>(attendees.size()); i++) {
        for (int j = i + 1; j < static_cast<int>(attendees.size()); j++) {
            MatchResult match = compareUsers(attendees[i], attendees[j]);
            if (match.score >= threshold) pairs.push_back(match);
        }
    }

    std::sort(pairs.begin(), pairs.end(), [](const MatchResult& a, const MatchResult& b) {
        return a.score > b.score;
    });

    DSU dsu(static_cast<int>(attendees.size()));

    for (const MatchResult& match : pairs) {
        int indexA = idToIndex[match.userA];
        int indexB = idToIndex[match.userB];
        int rootA = dsu.find(indexA);
        int rootB = dsu.find(indexB);

        if (rootA != rootB && dsu.groupSize(rootA) + dsu.groupSize(rootB) <= maxGroupSize) {
            dsu.unite(rootA, rootB);
        }
    }

    std::map<int, std::vector<int>> grouped;
    for (int i = 0; i < static_cast<int>(attendees.size()); i++) {
        grouped[dsu.find(i)].push_back(indexToId[i]);
    }

    std::vector<std::vector<int>> groups;
    for (auto& item : grouped) {
        if (item.second.size() > 1) {
            std::sort(item.second.begin(), item.second.end());
            groups.push_back(item.second);
        }
    }

    std::sort(groups.begin(), groups.end(), [](const std::vector<int>& a, const std::vector<int>& b) {
        if (a.size() != b.size()) return a.size() > b.size();
        return a < b;
    });

    return groups;
}

void EventMatcher::printUser(int userId) const {
    if (!hasUser(userId)) {
        std::cout << "User not found.\n";
        return;
    }

    const User& user = users.at(userId);
    std::cout << user.id << ". " << user.name << "\n";
    std::cout << "Comfort level: " << user.comfortLevel << "\n";
    std::cout << "Preferred group size: " << user.preferredGroupSize << "\n";
    std::cout << "Friends: " << user.friends.size() << "\n";
}

void EventMatcher::printTopMatches(int userId, const std::string& eventName, int limit) const {
    if (!hasUser(userId)) {
        std::cout << "User not found.\n";
        return;
    }
    if (!hasEvent(eventName)) {
        std::cout << "Event not found.\n";
        return;
    }

    std::vector<MatchResult> matches = getTopMatches(userId, eventName, limit);
    const User& user = users.at(userId);

    std::cout << "\nTop matches for " << user.name << " at " << eventName << ":\n";
    if (matches.empty()) {
        std::cout << "No matches available.\n";
        return;
    }

    for (int i = 0; i < static_cast<int>(matches.size()); i++) {
        const MatchResult& match = matches[i];
        const User& other = users.at(match.userB);
        std::cout << i + 1 << ". " << other.name
                  << " | Score: " << std::fixed << std::setprecision(1) << match.score * 100 << "%"
                  << " | Interests: " << match.commonInterests
                  << " | Availability: " << match.commonAvailability
                  << " | Mutual friends: " << match.mutualFriends << "\n";
    }
}

void EventMatcher::printGroups(const std::string& eventName, double threshold, int maxGroupSize) const {
    if (!hasEvent(eventName)) {
        std::cout << "Event not found.\n";
        return;
    }

    std::vector<std::vector<int>> groups = createGroups(eventName, threshold, maxGroupSize);
    std::cout << "\nSuggested groups for " << eventName << ":\n";

    if (groups.empty()) {
        std::cout << "No groups created with the current threshold.\n";
        return;
    }

    for (int i = 0; i < static_cast<int>(groups.size()); i++) {
        std::cout << "Group " << i + 1 << ": ";
        for (int j = 0; j < static_cast<int>(groups[i].size()); j++) {
            std::cout << users.at(groups[i][j]).name;
            if (j + 1 < static_cast<int>(groups[i].size())) std::cout << ", ";
        }
        std::cout << "\n";
    }
}

void EventMatcher::printStats() const {
    std::cout << "\nLoaded users: " << userCount() << "\n";
    std::cout << "Loaded events: " << eventCount() << "\n";
}

std::string EventMatcher::csvEscape(const std::string& text) const {
    bool needsQuotes = text.find(',') != std::string::npos || text.find('"') != std::string::npos || text.find('\n') != std::string::npos;
    if (!needsQuotes) return text;
    std::string result = "\"";
    for (char ch : text) {
        if (ch == '"') result += "\"\"";
        else result += ch;
    }
    result += "\"";
    return result;
}

bool EventMatcher::exportMatches(const std::string& eventName, int limit, const std::string& filePath) const {
    if (!hasEvent(eventName)) return false;

    std::ofstream file(filePath);
    if (!file.is_open()) return false;

    file << "event,user_id,user_name,rank,match_id,match_name,score_percent,common_interests,common_availability,mutual_friends\n";

    for (int userId : eventUsers.at(eventName)) {
        std::vector<MatchResult> matches = getTopMatches(userId, eventName, limit);
        for (int i = 0; i < static_cast<int>(matches.size()); i++) {
            const MatchResult& match = matches[i];
            file << csvEscape(eventName) << ","
                 << userId << ","
                 << csvEscape(users.at(userId).name) << ","
                 << i + 1 << ","
                 << match.userB << ","
                 << csvEscape(users.at(match.userB).name) << ","
                 << std::fixed << std::setprecision(1) << match.score * 100 << ","
                 << match.commonInterests << ","
                 << match.commonAvailability << ","
                 << match.mutualFriends << "\n";
        }
    }

    return true;
}

bool EventMatcher::exportGroups(const std::string& eventName, double threshold, int maxGroupSize, const std::string& filePath) const {
    if (!hasEvent(eventName)) return false;

    std::ofstream file(filePath);
    if (!file.is_open()) return false;

    std::vector<std::vector<int>> groups = createGroups(eventName, threshold, maxGroupSize);
    file << "event,group_number,user_id,user_name,group_size\n";

    for (int i = 0; i < static_cast<int>(groups.size()); i++) {
        for (int userId : groups[i]) {
            file << csvEscape(eventName) << ","
                 << i + 1 << ","
                 << userId << ","
                 << csvEscape(users.at(userId).name) << ","
                 << groups[i].size() << "\n";
        }
    }

    return true;
}
