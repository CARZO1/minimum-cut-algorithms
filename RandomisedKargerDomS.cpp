/* Dom S
I have chosen to create and implement a Randomistic Karger Algorithm. - Randomised Karger Min Cut
This algorithm contracts randomly chosen edges repeatedly until it identifies only 2 supernodes. 
The edges crossing the final supernodes signifies the minimum cut.
I chose this because it’s simple to implement, clearly shows how randomisation is able to speed up graph algorithms, and allows direct
comparison with deterministic variants. It also aligns with the course focus on randomised algorithms in the second half.
*/

#include "karger.hpp"
#include <random>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>

/* Dom S - Algorithm 1 - Randomised Karger Min Cut */

int karger::minCutRandomised(int n, const std::vector<Edge>& edges, std::uint64_t seed) {
    if (n <= 1 || edges.empty()) return 0;

    std::vector<int> parent(n), rank(n, 0);
    for (int i = 0; i < n; ++i) parent[i] = i;

    auto findParent = [&](int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    };

    auto unionSets = [&](int a, int b) {
        a = findParent(a);
        b = findParent(b);
        if (a == b) return false;
        if (rank[a] < rank[b]) std::swap(a, b);
        parent[b] = a;
        if (rank[a] == rank[b]) rank[a]++;
        return true;
    };

    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<std::size_t> pick(0, edges.size() - 1);

    int supernodes = n;
    while (supernodes > 2) {
        const auto& e = edges[pick(rng)];
        int a = findParent(e.u);
        int b = findParent(e.v);
        if (a == b) continue;
        unionSets(a, b);
        --supernodes;
    }

    int repA = -1, repB = -1;
    for (int i = 0; i < n; ++i) {
        int r = findParent(i);
        if (repA == -1) repA = r;
        else if (r != repA) { repB = r; break; }
    }
    if (repB == -1) return 0;

    int cutSize = 0;
    for (const auto& e : edges) {
        int a = findParent(e.u);
        int b = findParent(e.v);
        if ((a == repA && b == repB) || (a == repB && b == repA))
            ++cutSize;
    }

    std::cout << "Final cut size (Randomised Karger, seed " << seed << "): "
              << cutSize << "\n";
    return cutSize;
