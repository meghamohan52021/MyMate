#ifndef DSU_H
#define DSU_H

#include <vector>

class DSU {
private:
    std::vector<int> parent;
    std::vector<int> rank;
    std::vector<int> size;

public:
    DSU(int n);
    int find(int x);
    bool unite(int a, int b);
    int groupSize(int x);
};

#endif
