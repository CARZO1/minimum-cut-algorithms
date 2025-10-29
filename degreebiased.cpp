/* Deterministic Degree-Biased Karger's Contraction Algorithm

Karger's algorithm finds a cut by repeatedly contracting edges until only two supernodes remain. 
The edges between these final supernodes represent the cut value.
 
This deterministic variant selects edges to contract based on:
1. Minimize deg(u) * deg(v) (degree product heuristic)
2. Break ties lexicographically on (min(u,v), max(u,v))
3. Always contract larger ID into smaller ID

This heuristic tends to merge dense regions first, leaving weaker connections (potential min-cut edges) for last.

COMPLEXITY:
Baseline (scan-based): O(n·m) time, O(n+m) space
- Each of O(n) contractions scans O(m) edge endpoints
- Degree updates are O(degree) per contraction
*/

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <tuple>
#include <queue>
#include <string>

using namespace std;

// Graph representation: adjacency list with multiplicities
// adj[u][v] = number of edges between u and v
using Graph = vector<unordered_map<int, int>>;

//Compute degree of a vertex (sum of all edge multiplicities)
int compute_degree(const Graph& adj, int u) {
    int deg = 0;
    for (const auto& [v, mult] : adj[u]) {
        deg += mult;
    }
    return deg;
}

/*
Contract edge (u,v): merge v into u
Precondition: u < v
 */
void contract_edge(Graph& adj, vector<bool>& active, int u, int v) {
    // Merge all of v's neighbors into u
    for (const auto& [w, mult] : adj[v]) {
        if (w == u) continue; // Skip self-loop
        
        adj[u][w] += mult;
        adj[w][u] += mult;
        adj[w].erase(v);
    }
    
    // Remove v from graph
    adj[v].clear();
    active[v] = false;
    
    // Remove any self-loop on u
    adj[u].erase(v);
}

/*
Deterministic Degree-Biased Karger's Algorithm (Baseline)

Time: O(n·m), Space: O(n+m)
*/
int deterministic_degree_biased_karger(int n, const vector<pair<int,int>>& edges) {
    if (n <= 1) return 0;
    
    // Build adjacency list with multiplicities
    Graph adj(n);
    for (const auto& [u, v] : edges) {
        if (u != v) { // Ignore self-loops in input
            adj[u][v]++;
            adj[v][u]++;
        }
    }
    
    // Track active supernodes
    vector<bool> active(n, true);
    int num_active = n;
    
    // Contract until two supernodes remain
    while (num_active > 2) {
        // Find edge with minimum deg(u)*deg(v), breaking ties by (u,v)
        int best_u = -1, best_v = -1;
        long long best_score = LLONG_MAX;
        
        for (int u = 0; u < n; u++) {
            if (!active[u]) continue;
            
            int deg_u = compute_degree(adj, u);
            
            for (const auto& [v, mult] : adj[u]) {
                if (v <= u || !active[v]) continue; // Only consider u < v
                
                int deg_v = compute_degree(adj, v);
                long long score = (long long)deg_u * deg_v;
                
                // Lexicographic comparison: (score, u, v)
                if (score < best_score || 
                    (score == best_score && (u < best_u || (u == best_u && v < best_v)))) {
                    best_score = score;
                    best_u = u;
                    best_v = v;
                }
            }
        }
        
        if (best_u == -1) {
            // No edges left (disconnected graph)
            break;
        }
        
        // Contract edge: merge best_v into best_u
        contract_edge(adj, active, best_u, best_v);
        num_active--;
    }
    
    // Find the two remaining supernodes and compute cut value
    if (num_active < 2) return 0;
    
    vector<int> remaining;
    for (int i = 0; i < n; i++) {
        if (active[i]) remaining.push_back(i);
    }
    
    return adj[remaining[0]][remaining[1]];
}

//CLI: Read graph from stdin and output cut value
void run_cli() {
    int n, m;
    cin >> n >> m;
    
    vector<pair<int,int>> edges;
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        edges.push_back({u, v});
    }
    
    int result = deterministic_degree_biased_karger(n, edges);
    cout << result << endl;
}

//Unit tests
void run_tests() {
    cout << "Running tests\n" << endl;
    
    struct TestCase {
        string name;
        int n;
        vector<pair<int,int>> edges;
        int expected;
    };
    
    vector<TestCase> tests = {
        {
            "Two triangles with bridge",
            6,
            {{0,1},{1,2},{2,0},{3,4},{4,5},{5,3},{2,3}},
            1
        },
        {
            "Square with diagonal",
            4,
            {{0,1},{1,2},{2,3},{3,0},{0,2}},
            2
        },
        {
            "Triangle",
            3,
            {{0,1},{1,2},{0,2}},
            2
        },
        {
            "Parallel edges (multiplicity 3)",
            2,
            {{0,1},{0,1},{0,1}},
            3
        },
        {
            "Disconnected graph",
            3,
            {},
            0
        },
        {
            "Barbell - single bridge",
            6,
            {{0,1},{1,2},{2,0}, {3,4},{4,5},{5,3}, {2,3}},
            1
        },
        {
            "Barbell - double bridge",
            6,
            {{0,1},{1,2},{2,0}, {3,4},{4,5},{5,3}, {2,3},{2,3}},
            2
        },
        {
            "Lollipop - K3 + path",
            5,
            {{0,1},{1,2},{2,0}, {2,3},{3,4}},
            1
        },
        {
            "Graph with isolated vertices",
            5,
            {{0,1},{1,2},{2,0}},
            0
        },
        {
            "C4 with one diagonal",
            4,
            {{0,1},{1,2},{2,3},{3,0}, {1,3}},
            2
        },
        {
            "C5 with one chord",
            5,
            {{0,1},{1,2},{2,3},{3,4},{4,0}, {0,2}},
            2
        },
        {
            "C6 with symmetric chords",
            6,
            {{0,1},{1,2},{2,3},{3,4},{4,5},{5,0}, {0,3},{1,4}},
            3
        },
        {
            "Complete K4",
            4,
            {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}},
            3
        },
        {
            "Complete K5",
            5,
            {{0,1},{0,2},{0,3},{0,4},{1,2},{1,3},{1,4},{2,3},{2,4},{3,4}},
            4
        },
        {
            "Triangle with asymmetric multiplicities",
            3,
            {{0,1},{0,1},{0,1}, {1,2}, {2,0}},
            2
        },
        {
            "Dual-path bottleneck",
            8,
            {{0,1},{1,0}, {2,3},{3,2},  
            {0,4},{4,5},{5,2},          
            {1,6},{6,7},{7,3}},         
            2
        },
        {
            "K4 with pendant via 2 edges",
            5,
            {{0,1},{0,2},{0,3},{1,2},{1,3},{2,3}, {3,4},{3,4}},
            2
        },
        {
            "Weighted star graph",
            5,
            {{0,1},{0,1}, {0,2},{0,2},{0,2}, {0,3}, {0,4}},
            1
        },
        {
            "K5 minus one edge",
            5,
            {{0,1},{0,2},{0,3},{0,4},{1,2},{1,3},{1,4},{2,3},{2,4}},
            3
        },
        {
            "Bowtie (two triangles, shared vertex)",
            5,
            {{0,1},{1,2},{2,0}, {2,3},{3,4},{4,2}},
            2
        }
    };
    
    bool all_passed = true;
    
    for (const auto& test : tests) {
        int result = deterministic_degree_biased_karger(test.n, test.edges);
        bool passed = (result == test.expected);
        all_passed = all_passed && passed;
        
        cout << "[" << (passed ? "PASS" : "FAIL") << "] " << test.name << endl;
        cout << "  Expected: " << test.expected << ", Got: " << result << endl;
    }
    
    cout << string(50, '=') << endl;
    if (all_passed) {
        cout << "All tests PASSED" << endl;
    } else {
        cout << "Some tests FAILED" << endl;
    }
    cout << string(50, '=') << endl;
}

int main(int argc, char* argv[]) {
    if (argc > 1 && string(argv[1]) == "--test") {
        run_tests();
    } else {
        run_cli();
    }
    
    return 0;
}