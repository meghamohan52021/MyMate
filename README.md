# EventMate: C++ Event Buddy Matcher

EventMate matches students for events using real data structures and algorithms instead of simple CRUD logic.

## What is upgraded

This version supports a Google Forms-style CSV file:

```text
Timestamp,Name,Comfort Level,Preferred Group Size,Interests,Availability,Events,Friends
```

Your friends can fill a form, you can export the responses as CSV, rename the file to `form_responses.csv`, and place it inside the `data` folder.

The sample `form_responses.csv` included here has 100 users.

## CSV format

Use `|` between multiple choices.

Example:

```csv
Timestamp,Name,Comfort Level,Preferred Group Size,Interests,Availability,Events,Friends
2026-06-23 10:00:00,Megha,3,4,AI|Hackathons|Dance,Friday Evening|Saturday Afternoon,AI Hackathon|Tech Talk,Aisha|Priya
```

Recommended Google Form questions:

1. Name
2. Comfort Level, from 1 to 5
3. Preferred Group Size
4. Interests
5. Availability
6. Events
7. Friends

For checkbox questions, export them using `|` between choices before running, or keep the included CSV format.

## DSA used

| Feature | DSA |
|---|---|
| Student connections | Graph with adjacency lists |
| Interest comparison | Hash sets and Jaccard similarity |
| Availability comparison | Hash sets |
| Best buddy ranking | Priority queue / heap |
| Group creation | Disjoint Set Union / Union-Find |
| Event filtering | Hash map from event name to user IDs |
| Name-to-ID resolving | Hash map |

## Match score

```text
final score =
40% interest similarity
+ 30% availability similarity
+ 20% mutual friend similarity
+ 10% comfort-level compatibility
```

## Complexity

Let:

```text
n = total users
m = users attending one event
k = average number of interests
 t = average number of availability slots
E = friendships
r = number of top matches requested
```

Building user and event index:

```text
Time: O(n)
Space: O(n)
```

Building friendship graph:

```text
Time: O(E)
Space: O(n + E)
```

Comparing all users for one event:

```text
Time: O(m^2 * (k + t))
Space: O(m^2) if all candidate pairs are stored
```

Top matches per user using a fixed-size heap:

```text
Time: O(m log r)
Space: O(r)
```

Group creation using Union-Find:

```text
Time: O(m^2 * α(m)) after pair scoring and sorting
Space: O(m)
```

`α(m)` is practically constant.

## How to run

From the project root:

```bash
make
./eventmate
```

Or:

```bash
make run
```

On Windows with MinGW:

```bash
g++ -std=c++17 -Wall -Wextra -O2 src/main.cpp src/User.cpp src/DSU.cpp src/EventMatcher.cpp src/DataLoader.cpp -o eventmate.exe
eventmate.exe
```

## Menu options

```text
1. Show stats
2. Show events
3. Find top buddy matches
4. Create event groups
5. Show user profile
6. Export matches to CSV
7. Export groups to CSV
0. Exit
```

Exported files are saved inside the `output` folder.

## Best way to use with friends

1. Create a Google Form with the columns listed above.
2. Ask friends to fill it.
3. Download/export responses as CSV.
4. Rename it to `form_responses.csv`.
5. Put it inside `data/`.
6. Run the program.
7. Export matches or groups.

## Files

```text
src/User.h
src/User.cpp
src/DSU.h
src/DSU.cpp
src/EventMatcher.h
src/EventMatcher.cpp
src/DataLoader.h
src/DataLoader.cpp
src/main.cpp
data/form_responses.csv
output/
Makefile
README.md
```
