#include "PmergeMe.hpp"

size_t g_comps = 0;

int comp(int a, int b) {
    ++g_comps;
    return a < b;
}
