# EventMate: Event Buddy Matcher in C++

EventMate is a console-based C++ project that helps students find suitable event buddies and small groups based on interests, availability, comfort level, and mutual friends.

## DSA Used

| Feature | Data Structure / Algorithm |
|---|---|
| User lookup | unordered_map |
| Interest matching | unordered_set intersection |
| Availability matching | unordered_set intersection |
| Friend connections | Graph using adjacency sets |
| Mutual friends | Set intersection |
| Top buddy ranking | priority_queue / heap |
| Group creation | Disjoint Set Union / Union-Find |

## Project Structure

```text
EventMateCpp/
├── data/
│   ├── users.csv
│   └── friendships.csv
├── src/
│   ├── main.cpp
│   ├── User.h
│   ├── User.cpp
│   ├── DSU.h
│   ├── DSU.cpp
│   ├── EventMatcher.h
│   ├── EventMatcher.cpp
│   ├── DataLoader.h
│   └── DataLoader.cpp
├── Makefile
└── README.md
```

## How to Run

From the project root folder:

```bash
make
./eventmate
```

Or compile manually:

```bash
g++ -std=c++17 -Wall -Wextra -O2 src/main.cpp src/User.cpp src/DSU.cpp src/EventMatcher.cpp src/DataLoader.cpp -o eventmate
./eventmate
```

## Sample Inputs

Use these during the menu:

```text
User ID: 1
Event name: AI Hackathon
Number of matches: 3
```

For group creation:

```text
Event name: AI Hackathon
Minimum compatibility threshold: 0.45
Maximum group size: 4
```

## Match Score Formula

```text
Final score =
0.40 * interest similarity +
0.30 * availability similarity +
0.20 * mutual friend score +
0.10 * comfort compatibility
```

## Complexity Analysis

Let:

```text
n = number of users
m = number of users attending one event
e = number of friendships
k = average number of interests per user
t = average number of availability slots per user
r = number of requested recommendations
```

### Building users

Time: `O(n)`

Space: `O(n)`

### Building friendship graph

Time: `O(e)`

Space: `O(n + e)`

### Comparing two users

Interest comparison: `O(k)`

Availability comparison: `O(t)`

Mutual friends comparison: `O(f)`, where `f` is the smaller friend list size

Overall pair comparison: `O(k + t + f)`

### Finding top matches for one user in one event

Time: `O(m log m)` using a max heap

Space: `O(m)`

### Creating groups for one event

Pair comparison time: `O(m² * (k + t + f))`

Sorting compatible pairs: `O(p log p)`, where `p` is the number of compatible pairs

Union-Find operations: `O(p * α(m))`, practically constant per operation

Space: `O(m + p)`

## What Makes It Practical

This is not just event registration. The project solves the actual problem of finding someone suitable to attend with. It uses scoring, ranking, social graph analysis, and automatic group formation.
