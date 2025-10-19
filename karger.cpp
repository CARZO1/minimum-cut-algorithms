/*
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

#include "karger.hpp"

namespace karger {
    // find function for disjoint set
    int findParent(std::vector<int>& parent, int x) {
        if (parent[x] == x) return x; // if vertex is its own parent, its the root
        return parent[x] = findParent(parent, parent[x]);
    }

    // union function for disjoint set
    int unionSets(std::vector<int>& parent, std::vector<int>& rank, int a, int b) {
        int a = findParent(parent, a);
        int b = findParent(parent, b);
        if (a == b) return 0; // already in the same set

        if (rank[a] < rank[b]) std::swap(a, b); // attach smaller rank tree under root of higher rank tree
        parent[b] = a; // make a the parent of b
        if (rank[a] == rank[b]) rank[a]++; // if both trees had same rank, increment rank of new root
    }

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

        // still need to implement rest of code
    }
}