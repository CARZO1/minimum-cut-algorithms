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
#include <vector>
#include <string>

/* Dom S - Algorithm 1 - Randomised Karger Min Cut */

int karger::minCutRandomised(int n, const std::vector<Edge>& edges, std::uint64_t seed) {
    if (n <= 1 || edges.empty()) return 0;

    // initialise disjoint set
    std::vector<int> parent(n), rank(n, 0);
    for (int i = 0; i < n; ++i) parent[i] = i;

    // rng setup
    std::mt19937_64 rng(seed);
    std::uniform_int_distribution<std::size_t> pick(0, edges.size() - 1);

    // contraction loop
    int supernodes = n;
    while (supernodes > 2) {
        const auto& e = edges[pick(rng)];
        int a = findParent(parent, e.u);
        int b = findParent(parent, e.v);
        if (a == b) continue;
        unionSets(parent, rank, a, b);
        --supernodes;
    }

    // identify remaining supernodes
    int repA = -1, repB = -1;
    for (int i = 0; i < n; ++i) {
        int r = findParent(parent, i);
        if (repA == -1) repA = r;
        else if (r != repA) { repB = r; break; }
    }
    if (repB == -1) return 0;

    // count crossing edges
    int cutSize = 0;
    for (const auto& e : edges) {
        int a = findParent(parent, e.u);
        int b = findParent(parent, e.v);
        if ((a == repA && b == repB) || (a == repB && b == repA)) ++cutSize;
    }

    std::cout << "Final cut size (Randomised Karger, seed " << seed << "): " << cutSize << "\n";
    return cutSize;
}

// Dom S Test Cases - Randomised Karger
int main() {
    std::cout << "Dom S - Randomised Karger Tests";

    struct TestCase { 
        std::string name; 
        int n; 
        std::vector<karger::Edge> edges; 
    };

    std::vector<TestCase> domSTests = {
        {"simple 4-node", 4, {{0,1},{0,2},{1,2},{1,3},{2,3}}},
        {"triangle min cut 2", 3, {{0,1},{1,2},{0,2}}},
        {"two cliques bridge", 6, {{0,1},{1,2},{0,2},{3,4},{4,5},{3,5},{2,3}}},
        {"square cycle min cut 2", 4, {{0,1},{1,2},{2,3},{3,0}}},
        {"star graph min cut 1", 5, {{0,1},{0,2},{0,3},{0,4}}},
        {"complete k4 cut 3", 4, {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}}},
        {"disconnected cut 0", 4, {{0,1}}}
    };

    for (const auto& test : domSTests) {
        std::cout << "test: " << test.name << "\n";
        karger::minCutRandomised(test.n, test.edges, 123);
        std::cout << "";
    }

    return 0;
}
