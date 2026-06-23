#include "DSU.h"
#include <numeric>
#include <algorithm>

DSU::DSU(int n) {
    parent.resize(n);
    rank.assign(n, 0);
    size.assign(n, 1);
    std::iota(parent.begin(), parent.end(), 0);
}

int DSU::find(int x) {
    if (parent[x] == x) return x;
    parent[x] = find(parent[x]);
    return parent[x];
}

bool DSU::unite(int a, int b) {
    int rootA = find(a);
    int rootB = find(b);

    if (rootA == rootB) return false;

    if (rank[rootA] < rank[rootB]) {
        std::swap(rootA, rootB);
    }

    parent[rootB] = rootA;
    size[rootA] += size[rootB];

    if (rank[rootA] == rank[rootB]) {
        rank[rootA]++;
    }

    return true;
}

int DSU::groupSize(int x) {
    return size[find(x)];
}
