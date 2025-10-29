#ifndef KARGER_HPP
#define KARGER_HPP

#include <vector>
#include <utility>
#include <string>
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <unordered_map>

namespace karger {
    struct Edge { int u, v; };

    // find function for disjoint set
    inline int findParent(std::vector<int>& parent, int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]]; // move up two levels at a time
            x = parent[x];
        }
        return x;
    }

    // union function for disjoint set
    inline bool unionSets(std::vector<int>& parent, std::vector<int>& rank, int a, int b) {
        a = findParent(parent, a);
        b = findParent(parent, b);
        if (a == b) return false;

        // attach smaller tree under larger one
        if (rank[a] < rank[b] || (rank[a] == rank[b] && a > b))
            std::swap(a, b);

        parent[b] = a;
        if (rank[a] == rank[b]) rank[a]++;
        return true;
    }

    // Dominic S
    int minCutRandomised(int n, const std::vector<Edge>& edges, std::uint64_t seed);

    // Domenic C
    int minCutFixedPermutation(int n, const std::vector<Edge>& edges);
}

#endif
