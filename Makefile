CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
SRC = src/main.cpp src/User.cpp src/DSU.cpp src/EventMatcher.cpp src/DataLoader.cpp
OUT = eventmate

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	rm -f $(OUT)
	rm -f output/*.csv
