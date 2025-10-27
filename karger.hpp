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

    // Dominic S
    int minCutRandomised(int n, const std::vector<Edge>& edges, std::uint64_t seed);

    // Dominic C
    int minCutFixedPermutation(int n, const std::vector<Edge>& edges);
}

#endif
