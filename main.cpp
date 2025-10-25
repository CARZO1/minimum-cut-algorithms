// Test cases
#include "karger.hpp"
#include <iostream>
using namespace karger;

int main() {
    std::vector<Edge> edges = {
        {0, 1}, {0, 2}, {1, 2}, {1, 3}, {2, 3}
    };
    int n = 4;

    int cut = minCutFixedPermutation(n, edges);
}
