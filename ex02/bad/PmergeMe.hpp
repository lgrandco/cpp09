#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include "colors.hpp"
#include <algorithm>
#include <ctime>
#include <deque>
#include <limits>
#include <list>
#include <sstream>
#include <stdexcept>
#include <typeinfo>
#include <vector>

const unsigned long long JACOBSTHAL[] = {
    3,
    5,
    11,
    21,
    43,
    85,
    171,
    341,
    683,
    1365,
    2731,
    5461,
    10923,
    21845,
    43691,
    87381,
    174763,
    349525,
    699051,
    1398101,
    2796203,
    5592405,
    11184811,
    22369621,
    44739243,
    89478485,
    178956971,
    357913941,
    715827883,
    1431655765,
    2863311531,
    5726623061,
    11453246123,
    22906492245,
    45812984491,
    91625968981,
    183251937963,
    366503875925,
    733007751851,
    1466015503701,
    2932031007403,
    5864062014805,
    11728124029611,
    23456248059221,
    46912496118443
};

extern size_t g_comps;

template <template <typename, typename> class T>
struct Container {
    T<int, std::allocator<int> > seq;
    T<std::pair<int, int>, std::allocator<std::pair<int, int> > > pairs;
    int alone;
    clock_t begin_time;
    T<size_t, std::allocator<size_t> > idxs;
};

template <typename Wrapper>
void get_numbers(char** av, Wrapper& w) {
    long n;
    char c;
    w.begin_time = std::clock();
    long prev = -1;
    for (; *av; ++av) {
        std::istringstream ss(*av);
        if (!(ss >> n) || n > std::numeric_limits<int>::max() || n < 0 || (ss >> c))
            throw(std::runtime_error("Invalid argument"));
        if (prev == -1) prev = n;
        else {
            if (prev < n) w.pairs.push_back(std::make_pair(prev, n));
            else w.pairs.push_back(std::make_pair(n, prev));
            prev = -1;
        }
    }
    w.alone = prev;
    g_comps += w.pairs.size();
}

int comp(int a, int b);

template <typename Pairs>
void concat(const Pairs& left, const Pairs& right, Pairs& merged) {
    typename Pairs::const_iterator it_left = left.begin();
    typename Pairs::const_iterator it_right = right.begin();
    while (it_left != left.end() && it_right != right.end()) {
        if (comp(it_left->second, it_right->second)) merged.push_back(*it_left++);
        else merged.push_back(*it_right++);
    }
    while (it_left != left.end()) merged.push_back(*it_left++);
    while (it_right != right.end()) merged.push_back(*it_right++);
}

template <typename Pairs>
void merge_pairs(Pairs& pairs) {
    if (pairs.size() < 2) return;
    size_t middle = pairs.size() / 2;

    Pairs left;
    Pairs right;
    typename Pairs::iterator it = pairs.begin();
    for (size_t i = 0; i < middle; ++i) left.push_back(*it++);
    for (size_t i = middle; i < pairs.size(); ++i) right.push_back(*it++);
    merge_pairs(left);
    merge_pairs(right);
    pairs.clear();
    concat(left, right, pairs);
}

template < template <typename, typename> class C >
void push_biggest(Container<C>& cont) {
    typename C< std::pair<int, int>, std::allocator<std::pair<int, int> > >::iterator
        it = cont.pairs.begin();
    if (it == cont.pairs.end()) {
        cont.seq.push_back(cont.alone);
        return;
    }
    cont.seq.push_back(it->first);
    cont.seq.push_back(it->second);
    for (++it; it != cont.pairs.end(); ++it) { cont.seq.push_back(it->second); }
}

template <typename Wrapper>
void get_idxs(Wrapper& cont) {
    long long min = 0;
    size_t max = cont.pairs.size() - 1;
    for (size_t jacob_i = 0; cont.idxs.size() < max; ++jacob_i) {
        for (size_t i = std::min(JACOBSTHAL[jacob_i] - 1, (long long)max - cont.idxs.size() + min);
             (long long)i > min && cont.idxs.size() < max;
             --i) {
            cont.idxs.push_back(i);
        }
        min = JACOBSTHAL[jacob_i] - 1;
    }
}

template <typename Wrapper>
void merge_insert(Wrapper& cont) {
    merge_pairs(cont.pairs);
    push_biggest(cont);
    if (cont.alone != -1) cont.pairs.push_back(std::make_pair(cont.alone, -1));
    get_idxs(cont);
    for (size_t i = 0; i < cont.pairs.size() - 1; ++i) {
        int value = cont.pairs[cont.idxs[i]].first;
        cont.seq.insert(std::lower_bound(cont.seq.begin(), cont.seq.end(), value, comp), value);
    }
}

#endif // !PMERGEME_HPP
