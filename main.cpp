// Test cases
#include "karger.hpp"
#include <iostream>
using namespace karger;

int main() {
    //Dom S Test Cases - Randomised Karger
    // simple 4-node test
    std::vector<Edge> edges1 = {
        {0,1}, {0,2}, {1,2}, {1,3}, {2,3}
    };
    std::cout << "test 1:\n";
    minCutRandomised(4, edges1, 123);

    // triangle graph min cut 2
    std::vector<Edge> edges2 = {
        {0,1}, {1,2}, {0,2}
    };
    std::cout << "\ntest 2:\n";
    minCutRandomised(3, edges2, 55);

    // two cliques with bridge
    std::vector<Edge> edges3 = {
        {0,1},{1,2},{0,2}, {3,4},{4,5},{3,5}, {2,3}
    };
    std::cout << "\ntest 3:\n";
    minCutRandomised(6, edges3, 77);

    // square cycle min cut 2
    std::vector<Edge> edges4 = {
        {0,1},{1,2},{2,3},{3,0}
    };
    std::cout << "\ntest 4:\n";
    minCutRandomised(4, edges4, 99);

    // star graph min cut 1
    std::vector<Edge> edges5 = {
        {0,1},{0,2},{0,3},{0,4}
    };
    std::cout << "\ntest 5:\n";
    minCutRandomised(5, edges5, 111);

    // complete graph k4 cut 3
    std::vector<Edge> edges6 = {
        {0,1},{0,2},{0,3},{1,2},{1,3},{2,3}
    };
    std::cout << "\ntest 6:\n";
    minCutRandomised(4, edges6, 222);

    // disconnected graph cut 0
    std::vector<Edge> edges7 = {
        {0,1}
    };
    std::cout << "\ntest 7:\n";
    minCutRandomised(4, edges7, 333);

    return 0;
    //Dom C Test Cases
    std::cout << "\nRunning Deterministic (Fixed Permutation):\n";
    karger::minCutFixedPermutation(n, edges);

    return 0;
}
