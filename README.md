# EventMate: Event Buddy Matcher

EventMate is a C++ console-based matching system that helps students find suitable buddies or small groups for events. It is designed for real use with form-exported CSV data, such as responses from Google Forms or Microsoft Forms.

The project is not just an event management app. It uses data structures and algorithms to solve a real matching problem using interests, availability, comfort level, event choices, and friend connections.

---

## Problem Statement

Many students want to attend university events, workshops, hackathons, career talks, sports meetups, or society activities, but they may not know who to go with.

This can be a bigger issue for students who are new, shy, or uncomfortable attending events alone. EventMate solves this by recommending compatible event buddies and automatically creating small event groups.

---

## Main Features

- Load users from a Google Forms-style CSV file
- Match students based on interests, availability, comfort level, and mutual friends
- Show available events and number of attendees
- Find top buddy recommendations for a selected user and event
- Create event groups using Union-Find
- Export buddy matches to CSV
- Export created groups to CSV
- Use graph-based friend connections
- Use priority queues for ranking best matches

---

## Technologies Used

- C++17
- Standard Template Library
- File handling
- CSV parsing
- Hash maps
- Hash sets
- Priority queue
- Graph representation
- Union-Find

---

## Project Structure

```text
EventMateCpp/
│
├── data/
│   ├── form_responses.csv
│   ├── users.csv
│   └── friendships.csv
│
├── output/
│   ├── matches.csv
│   └── groups.csv
│
├── src/
│   ├── main.cpp
│   ├── User.h
│   ├── User.cpp
│   ├── DataLoader.h
│   ├── DataLoader.cpp
│   ├── EventMatcher.h
│   ├── EventMatcher.cpp
│   ├── DSU.h
│   └── DSU.cpp
│
├── Makefile
└── README.md
```

---

## How to Run

### Option 1: Windows PowerShell with g++

From the project root folder:

```powershell
g++ src/*.cpp -o eventmate.exe
.\eventmate.exe
```

### Option 2: Linux, macOS, or Git Bash with Make

```bash
make
./eventmate
```

or:

```bash
make run
```

### Option 3: Clean compiled files

```bash
make clean
```

---

## Input Data Format

The main input file is:

```text
data/form_responses.csv
```

It follows a form-style format:

```text
Timestamp,Name,Comfort Level,Preferred Group Size,Interests,Availability,Events,Friends
```

Example row:

```text
2026-01-01 10:00,Kavya Mohan,3,4,AI|Hackathons|Music,Friday Evening|Saturday Afternoon,AI Hackathon|Career Talk,Hannah Hall|Priya Nair
```

### Column Explanation

| Column | Meaning |
|---|---|
| Timestamp | Time when the form was submitted |
| Name | Student name |
| Comfort Level | Number from 1 to 5 |
| Preferred Group Size | Ideal group size for the student |
| Interests | Interests separated by `|` |
| Availability | Available time slots separated by `|` |
| Events | Events the student wants to attend, separated by `|` |
| Friends | Friend names separated by `|` |

---

## Menu Options

When the program runs, it shows this menu:

```text
EventMate - Event Buddy Matcher
1. Show stats
2. Show events
3. Find top buddy matches
4. Create event groups
5. Show user profile
6. Export matches to CSV
7. Export groups to CSV
0. Exit
```

### 1. Show stats

Shows how many users and events were loaded.

Example:

```text
Loaded users: 100
Loaded events: 8
```

### 2. Show events

Displays all available events and how many students selected each event.

Example:

```text
Available events:
- AI Hackathon (24 people)
- Career Talk (31 people)
- Coding Workshop (27 people)
```

### 3. Find top buddy matches

Finds the best buddy matches for a selected user at a selected event.

The program asks for:

```text
User ID
Event name
Number of matches
```

Example:

```text
User ID: 7
Event name: AI Hackathon
Number of matches: 3
```

Output example:

```text
Top matches for Kavya Mohan at AI Hackathon:
1. Hannah Hall | Score: 43.5% | Interests: 3 | Availability: 2 | Mutual friends: 0
```

### 4. Create event groups

Creates small compatible groups for an event.

The program asks for:

```text
Event name
Minimum compatibility threshold
Maximum group size
```

Example:

```text
Event name: AI Hackathon
Minimum compatibility threshold, example 0.45: 0.45
Maximum group size: 4
```

Students are joined into groups only if their compatibility score is high enough.

### 5. Show user profile

Displays basic information about a user.

Example:

```text
User ID: 7
```

Output:

```text
7. Kavya Mohan
Comfort level: 3
Preferred group size: 4
Friends: 2
```

### 6. Export matches to CSV

Exports buddy recommendations for every user attending a selected event.

Output file:

```text
output/matches.csv
```

The exported file contains:

```text
event,user_id,user_name,rank,match_id,match_name,score_percent,common_interests,common_availability,mutual_friends
```

### 7. Export groups to CSV

Exports generated event groups to a CSV file.

Output file:

```text
output/groups.csv
```

The exported file contains:

```text
event,group_number,user_id,user_name,group_size
```

### 0. Exit

Closes the program.

---

## Matching Logic

Each pair of students is compared using four factors:

```text
Final score =
40% interest similarity
+ 30% availability similarity
+ 20% mutual friend score
+ 10% comfort level compatibility
```

### Interest Similarity

Compares two students' interests using set intersection and union.

```text
Interest similarity = common interests / total unique interests
```

### Availability Similarity

Compares available time slots using set intersection and union.

```text
Availability similarity = common availability / total unique availability
```

### Mutual Friend Score

Compares friend lists and checks how many friends two students have in common.

```text
Mutual friend score = common friends / total unique friends
```

### Comfort Score

Compares comfort levels from 1 to 5.

| Difference | Score |
|---|---|
| 0 | 1.00 |
| 1 | 0.75 |
| 2 | 0.45 |
| 3 or more | 0.20 |

---

## Data Structures and Algorithms Used

| Feature | Data Structure / Algorithm | Purpose |
|---|---|---|
| Store users | `unordered_map<int, User>` | Fast user lookup by ID |
| Store names | `unordered_map<string, int>` | Convert friend names to user IDs |
| Store interests | `unordered_set<string>` | Fast interest comparison |
| Store availability | `unordered_set<string>` | Fast availability comparison |
| Store events | `unordered_map<string, vector<int>>` | Find attendees for each event |
| Friend network | Graph using adjacency sets | Represent user connections |
| Top matches | Priority queue / heap | Keep best matches efficiently |
| Group creation | Union-Find | Merge compatible students into groups |
| CSV loading | File parsing | Read form responses |
| CSV export | File writing | Save matches and groups |

---

## Time and Space Complexity

Let:

```text
n = total number of users
m = number of users attending one event
k = average number of interests per user
t = average number of availability slots per user
e = number of friendship connections
r = number of requested matches
```

### Loading users

```text
Time: O(n)
Space: O(n)
```

### Building event index

```text
Time: O(n × average events per user)
Space: O(n)
```

### Comparing two users

```text
Time: O(k + t + friend count)
Space: O(1) extra
```

### Finding top matches for one user

```text
Time: O(m × (k + t + friend count) + m log r)
Space: O(r)
```

### Creating groups for one event

```text
Time: O(m² × (k + t + friend count) + m² log m)
Space: O(m²)
```

### Union-Find operations

```text
Find: O(α(n))
Union: O(α(n))
```

`α(n)` is the inverse Ackermann function, which is practically constant.

---

## Function Guide

This section explains the main functions in the project.

---

## main.cpp

### `showMenu()`

Prints the main menu options.

### `clearInput()`

Clears invalid or leftover input from the console.

This prevents input errors when switching between numbers and text.

### `readLine(const string& label)`

Prints a label and reads a full line of text.

Used for event names because event names can contain spaces, such as `AI Hackathon` or `Career Talk`.

### `main()`

Starts the program.

It:

1. Creates an `EventMatcher` object
2. Loads data from `form_responses.csv`
3. Falls back to `users.csv` and `friendships.csv` if needed
4. Shows the menu
5. Runs the selected option
6. Calls the correct matching, grouping, printing, or export function

---

## User Class

### `User()`

Default constructor.

Creates an empty user with:

```text
id = 0
comfortLevel = 3
preferredGroupSize = 3
```

### `User(int id, const string& name, int comfortLevel, int preferredGroupSize)`

Creates a user with a real ID, name, comfort level, and preferred group size.

Each user also stores:

- interests
- availability
- events
- friends

---

## DataLoader Class

The `DataLoader` class reads CSV files and converts them into `User` objects and friendship connections.

### `trim(const string& text)`

Removes extra spaces from the start and end of a string.

Example:

```text
"  AI Hackathon  " becomes "AI Hackathon"
```

### `normalizeHeader(const string& text)`

Converts CSV column names into a simple format.

Example:

```text
"Comfort Level" becomes "comfort_level"
```

This makes the loader more flexible with form column names.

### `splitSet(const string& text, char delimiter)`

Splits values such as interests, events, availability, and friends into a set.

Example:

```text
"AI|Music|Hackathons" becomes {AI, Music, Hackathons}
```

A set is used so duplicate values are automatically removed.

### `parseCsvLine(const string& line)`

Parses one line of a CSV file.

It handles:

- commas
- quoted text
- double quotes inside quoted text

### `toIntOrDefault(const string& text, int fallback)`

Converts text into an integer.

If conversion fails, it returns the fallback value.

Example:

```text
"4" becomes 4
"abc" becomes fallback value
```

### `loadFormResponses(const string& filePath, EventMatcher& matcher)`

Loads the main Google Forms-style CSV file.

It reads:

- name
- comfort level
- preferred group size
- interests
- availability
- events
- friend names

Then it:

1. Creates users
2. Adds users to the matcher
3. Stores friend names temporarily
4. Resolves friend names into user IDs
5. Builds the event index

Returns `true` if loading succeeds.

### `loadUsers(const string& filePath, EventMatcher& matcher)`

Loads users from the older `users.csv` format.

This is used as a backup if `form_responses.csv` is not available.

### `loadFriendships(const string& filePath, EventMatcher& matcher)`

Loads friendships from `friendships.csv`.

This is also part of the backup loading method.

---

## EventMatcher Class

The `EventMatcher` class contains the main matching and grouping logic.

### `addUser(const User& user)`

Adds a user to the system.

It stores the user by ID and also stores their name so friendships can be resolved by name.

### `addFriendship(int userA, int userB)`

Adds a two-way friendship between two users.

If user 1 is friends with user 2, user 2 is also friends with user 1.

### `addFriendshipByName(const string& nameA, const string& nameB)`

Adds a friendship using names instead of IDs.

This is useful because form responses usually contain friend names, not user IDs.

### `buildEventIndex()`

Builds a map of events to attendees.

Example:

```text
AI Hackathon -> [1, 7, 13, 25]
Career Talk -> [2, 5, 9]
```

This makes event-based matching faster because the program only compares users attending the same event.

### `hasUser(int userId)`

Checks whether a user exists.

### `hasEvent(const string& eventName)`

Checks whether an event exists.

### `userCount()`

Returns the number of loaded users.

### `eventCount()`

Returns the number of loaded events.

### `getUserIdByName(const string& name)`

Finds a user's ID from their name.

Returns `-1` if the name is not found.

### `getEvents()`

Returns a sorted list of all event names.

Used by menu option 2.

### `getAttendees(const string& eventName)`

Returns the user IDs of students attending a selected event.

### `intersectionSize(const unordered_set<string>& a, const unordered_set<string>& b)`

Counts how many text values two sets have in common.

Used for:

- common interests
- common availability

### `intersectionSize(const unordered_set<int>& a, const unordered_set<int>& b)`

Counts how many integer values two sets have in common.

Used for mutual friends.

### `setSimilarity(const unordered_set<string>& a, const unordered_set<string>& b)`

Calculates Jaccard similarity between two sets.

Formula:

```text
common values / total unique values
```

Used for interests and availability.

### `mutualFriendScore(const User& a, const User& b)`

Calculates how many mutual friends two users have compared with their total unique friends.

### `comfortScore(const User& a, const User& b)`

Calculates compatibility between two users based on their comfort levels.

Similar comfort levels get a higher score.

### `compareUsers(int idA, int idB)`

Compares two users and returns a `MatchResult`.

It calculates:

- interest similarity
- availability similarity
- mutual friend score
- comfort score
- final weighted score
- number of common interests
- number of common availability slots
- number of mutual friends

### `getTopMatches(int userId, const string& eventName, int limit)`

Finds the top matching buddies for one user at one event.

It only compares the user with people attending the same event.

It uses a priority queue to keep the best matches.

### `createGroups(const string& eventName, double threshold, int maxGroupSize)`

Creates small groups for an event.

Steps:

1. Compare every pair of attendees
2. Keep pairs above the threshold
3. Sort compatible pairs by score
4. Use Union-Find to merge students into groups
5. Stop groups from becoming larger than the maximum group size

### `printUser(int userId)`

Prints a user's basic profile.

### `printTopMatches(int userId, const string& eventName, int limit)`

Prints the top buddy matches in a readable format.

### `printGroups(const string& eventName, double threshold, int maxGroupSize)`

Prints suggested groups for an event.

### `printStats()`

Prints the number of loaded users and events.

### `csvEscape(const string& text)`

Prepares text safely for CSV output.

It adds quotes when a value contains commas, quotes, or new lines.

### `exportMatches(const string& eventName, int limit, const string& filePath)`

Exports top buddy matches for all users attending an event.

Output file:

```text
output/matches.csv
```

### `exportGroups(const string& eventName, double threshold, int maxGroupSize, const string& filePath)`

Exports created groups for a selected event.

Output file:

```text
output/groups.csv
```

---

## DSU Class

The `DSU` class implements Disjoint Set Union, also called Union-Find.

It is used to create compatible groups.

### `DSU(int n)`

Creates `n` separate groups.

At the start, every student is in their own group.

### `find(int x)`

Finds the leader of the group that `x` belongs to.

Uses path compression to make future searches faster.

### `unite(int a, int b)`

Merges two groups if they are not already connected.

Uses union by rank to keep the structure efficient.

### `groupSize(int x)`

Returns the size of the group that `x` belongs to.

Used to prevent groups from becoming larger than the selected maximum group size.

---

## MatchResult Struct

Stores the result of comparing two users.

Fields:

| Field | Meaning |
|---|---|
| `userA` | First user ID |
| `userB` | Matched user ID |
| `score` | Final compatibility score |
| `commonInterests` | Number of shared interests |
| `commonAvailability` | Number of shared availability slots |
| `mutualFriends` | Number of mutual friends |

---

## MatchCompareMin Struct

Used by the priority queue when finding top matches.

It keeps the lowest score at the top so the program can remove weak matches when the heap becomes larger than the requested limit.

---

## MatchCompareMax Struct

Defines comparison for higher-score-first ordering.

It is available for max-heap style match ranking.

---

## Example Use Case

A university society wants to organize an AI Hackathon.

Students fill a form with:

- name
- interests
- availability
- preferred events
- comfort level
- friends

The organiser exports the form responses as CSV and places it in:

```text
data/form_responses.csv
```

Then they run EventMate and choose:

```text
3. Find top buddy matches
```

or:

```text
4. Create event groups
```

The system recommends who should attend together.

---

## Why This Project Is Strong

This project demonstrates:

- Practical problem-solving
- Graph-based modelling
- Hash-based matching
- Ranking with heaps
- Group creation using Union-Find
- CSV file handling
- Clean C++ class design
- Time and space complexity analysis
- Real use with form responses

It is more advanced than a basic timetable, budget tracker, or CRUD app because the main value comes from the algorithmic matching logic.

---

## Future Plan: Social Event Platform

The long-term plan for EventMate is to grow from a console-based matching system into a social event platform similar to Instagram, but focused on student events, buddy matching, and small group discovery.

In the future version, every student will have their own profile where they can show their interests, upcoming events, comfort level, availability preferences, friends, and previous event participation. Instead of only using CSV files, users will be able to sign up, update their profile, follow or connect with friends, discover events, and receive buddy or group recommendations directly inside the platform.

The idea is not to copy Instagram exactly, but to take the useful social features such as profiles, connections, event posts, recommendations, and activity feeds, then apply them to a real student problem: helping people find others to attend events with.

### Planned Social Features

- User profiles with name, bio, interests, events, and comfort preferences
- Profile pictures and optional public/private profile settings
- Friend requests or follow-style connections
- Event pages where organisers can post event details
- Event discovery feed based on interests and friends
- Buddy recommendations shown on each event page
- Automatic group suggestions for hackathons, workshops, trips, and society events
- Chat or group invite system for matched students
- Saved events and attended-event history
- Privacy controls so users can choose what information is visible
- Organiser dashboard for managing attendees and groups

### Future Technical Upgrades

- Replace CSV storage with SQLite or PostgreSQL
- Add a backend API for user accounts, events, matches, and groups
- Build a web frontend using React, Vue, or another frontend framework
- Keep the C++ matching engine as the algorithmic core or rewrite the matching logic as a backend service
- Connect directly to Google Forms or Google Sheets for easy event registration
- Add authentication and user login
- Add better group balancing based on group size, comfort level, and mutual friends
- Add QR code event check-in
- Add email or notification-based group invitations
- Add admin tools for university societies and event organisers


