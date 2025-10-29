// Test cases
#include "karger.hpp"
#include <iostream>
using namespace karger;

int main() {

    // Dom S Tests
    std::cout << "===== Dom S - Randomised Karger Tests =====\n\n";

    struct TestCase {
        std::string name;
        int n;
        std::vector<Edge> edges;
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
        minCutRandomised(test.n, test.edges, 123);
        std::cout << "------------------------------------------\n";
    }

    //Dom C Test Cases
    //std::cout << "\nRunning Deterministic (Fixed Permutation):\n";
   // karger::minCutFixedPermutation(n, edges);

   // return 0;
//}
