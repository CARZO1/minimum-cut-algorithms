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
    // simple edge structure
    struct Edge {
        int u, v;
    };

    // function to compute the minimum cut using a fixed permutation of edges
    int minCutFixedPermutation(int n, const std::vector<Edge>& edges);
}

#endif