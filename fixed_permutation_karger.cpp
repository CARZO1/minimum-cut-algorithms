#include "karger.hpp"
#include <random>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
/* Dom C
I have chosen to implement the Deterministic Karger – Fixed Permutation algorithm.

This algorithm is a derandomised version of the classic Karger's random contraction algorithm, for finding the minimunm cut of
an underirected graph. The algorithm works by contracting edges in a fixed order, rather than randomly selecting edges to contract.

I selected this algorithm because it:
- clearly demonstrates how randomness in algorithms can be removed through deterministic ordering
- retains the simplictiy of the original Karger algorithm (edge contradtion)
- is straightforward to implement and test using disjoint sets (union-find data structure)
- allows a direct comparison with the randomised version of Karger's algorithm

My though process for the implementation is as follows:
1) Represent the graph as a list of undirected edges
2) Sort these edges deterministically based on vertex IDs (this replaces randomness)
3) Repeatedly contract edges in that fixed order until only two supernodes remain
4) Count how many edges still connect the two remaining components - this is the cut size
*/

namespace karger {
int minCutFixedPermutation(int n, const std::vector<Edge>& edges) {
        if (n <= 1) return 0; // no cut possible
        if (edges.empty()) return 0; // no edges, no cut

        // Step 1 - create disjoint set for all vertices
        std::vector<int> parent(n);
        std::vector<int> rank(n, 0);
        for (int i = 0; i < n; ++i) parent[i] = i; // each vertex is its own parent initially

        // Step 2 - create a fixed (input-derived) perumutation, and sort edges by (min(u,v), max(u,v)) to remove randomness
        std::vector<Edge> sortedEdges = edges;
        std::sort(sortedEdges.begin(), sortedEdges.end(), [](const Edge& a, const Edge& b) {
            int au = std::min(a.u, a.v);
            int av = std::max(a.u, a.v);
            int bu = std::min(b.u, b.v);
            int bv = std::max(b.u, b.v);
            if (au == bu) return av < bv;
            return au < bu;
        });

        // Step 3 - contract edges following the fixed order
        int vertices = n;
        for (const Edge& edge : sortedEdges) {
            if (vertices <= 2) break; // stop when only two supernodes remain
            int a = findParent(parent, edge.u);
            int b = findParent(parent, edge.v);
            if (a != b) {
                unionSets(parent, rank, a, b);
                vertices--;
            }
        }

        // Step 4 - determine which supernodes remain
        std::vector<int> supernodes;
        for (int i = 0; i < n; ++i) {
            int p = findParent(parent, i);
            if (std::find(supernodes.begin(), supernodes.end(), p) == supernodes.end()) {
                supernodes.push_back(p);
            }
        }

        if (supernodes.size() < 2) return 0; // no cut possible

        int supernodeA = supernodes[0];
        int supernodeB = supernodes[1];

        // Step 5 - count crossing edges between the two remaining supernodes
        int cutSize = 0;
        for (const auto& e : edges) {
            int a = findParent(parent, e.u);
            int b = findParent(parent, e.v);
            if (a == supernodeA && b == supernodeB) cutSize++;
            else if (a == supernodeB && b == supernodeA) cutSize++;
        }

        std::cout << "Final cut size (deterministic Karger - fixed permutation): " << cutSize << "\n";
        return cutSize;
    }
}
