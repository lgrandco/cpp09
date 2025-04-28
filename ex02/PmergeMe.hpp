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

const size_t JACOBSTHAL[] = {
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
    typedef T<int, std::allocator<int> > seq_type;

    seq_type seq;
    bool is_alone;
    T<size_t, std::allocator<size_t> > idxs;
    clock_t begin_time;
};

template <typename Wrapper>
void get_numbers(char** av, Wrapper& w) {
    long n;
    char c;
    for (; *av; ++av) {
        std::istringstream ss(*av);
        if (!(ss >> n) || n > std::numeric_limits<int>::max() || n < 0 || (ss >> c))
            throw(std::runtime_error("Invalid argument"));
        w.seq.push_back(n);
    }
}

template <typename Wrapper>
void make_pairs(Wrapper& w) {
    long prev = -1;
    for (size_t i = 0; i < w.seq.size(); ++i) {
        int n = w.seq[i];
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

// template <typename Container>
// void push_biggest(Container& dst, const Container& src, size_t level) {
//     if (src.begin() == src.end()) { return; }
//     for (typename Container::const_iterator it = src.begin() + 3 * level;
//          it < src.begin() + (src.size() / level) * level;
//          it += 2 * level) {
//         for (size_t i = 0; i < level; ++i) { dst.push_back(*(it + i)); }
//     }
// }

template <typename Indexes>
void get_idxs(Indexes& idxs, size_t n) {
    size_t min = 0;
    size_t max = n - 1;
    idxs.clear();
    for (size_t jacob_i = 0; idxs.size() < max; ++jacob_i) {
        for (size_t i = std::min(JACOBSTHAL[jacob_i] - 1, max - idxs.size() + min);
             i > min && idxs.size() < max;
             --i) {
            idxs.push_back(i);
        }
        min = JACOBSTHAL[jacob_i] - 1;
    }
}

template <typename Iterator>
void swap_pairs(Iterator a, Iterator b, size_t level) {
    for (size_t i = 0; i < level; ++i) {
        int tmp = *a;
        *a = *b;
        *b = tmp;
        ++a;
        ++b;
    }
}

template <typename Cont>
size_t get_pos(Cont seq, size_t level, int value) {
    size_t left = 0;
    size_t right = seq.size() / level;
    size_t middle;

    while (left < right) {
        std::cout << "comp done" << std::endl;
        ++g_comps;
        middle = left + (right - left) / 2;
        int middle_value = seq[middle * level + level - 1];
        if (middle_value == value) return middle;
        if (middle_value < value) left = middle + 1;
        else right = middle;
    }
    return left * level;
}

template <typename Wrapper>
void merge_insert(Wrapper& cont, size_t level) {
    if (cont.seq.size() / level < 2) return;
    for (typename Wrapper::seq_type::iterator it = cont.seq.begin() + level;
         it < cont.seq.begin() + (cont.seq.size() / level) * level;
         it += 2 * level) {
        typename Wrapper::seq_type::iterator last = it - level;
        if (comp(*(it + level - 1), *(last + level - 1))) swap_pairs(last, it, level);
    }
    merge_insert(cont, level * 2);
    typename Wrapper::seq_type main_chain;
    // lvl * 2 for first 2 pairs
    for (typename Wrapper::seq_type::iterator it = cont.seq.begin();
         it < cont.seq.begin() + level * 2;
         ++it)
        main_chain.push_back(*it);

    cont.is_alone = cont.seq.size() % (2 * level) >= level;
    get_idxs(cont.idxs, cont.seq.size() / (2 * level) + cont.is_alone);

    // push_biggest(main_chain, cont.seq, level);
    // template <typename Container>
    // void push_biggest(Container& dst, const Container& src, size_t level) {
    //     if (src.begin() == src.end()) { return; }
    //     for (typename Container::const_iterator it = src.begin() + 3 * level;
    //          it < src.begin() + (src.size() / level) * level;
    //          it += 2 * level) {
    //         for (size_t i = 0; i < level; ++i) { dst.push_back(*(it + i)); }
    //     }
    // }
    std::cout << "level : " << level << "\n";
    std::cout << "sequence : ";
    for (size_t y = 0; y < cont.seq.size(); ++y) std::cout << cont.seq[y] << " ";
    std::cout << "\n";

    std::cout << "main_chain : ";
    for (size_t y = 0; y < main_chain.size(); ++y) std::cout << main_chain[y] << " ";
    std::cout << "\n";

    std::cout << "idxs : ";
    for (size_t y = 0; y < cont.idxs.size(); ++y) std::cout << cont.idxs[y] << " ";
    std::cout << "\n";
    size_t pushed = 1;
    typename Wrapper::seq_type::iterator to_be_pushed = cont.seq.begin() + 3 * level;
    for (size_t i = 0; i < cont.idxs.size(); ++i) {
        std::cout << "main_chain before push : ";
        for (size_t y = 0; y < main_chain.size(); ++y) std::cout << main_chain[y] << " ";
        std::cout << "\n";
        while (pushed < cont.idxs[i] &&
               to_be_pushed < cont.seq.begin() + (cont.seq.size() / level) * level) {
            for (size_t j = 0; j < level; ++j) { main_chain.push_back(*(to_be_pushed + j)); }
            to_be_pushed += 2 * level;
        }
        std::cout << "main_chain after push : ";
        for (size_t y = 0; y < main_chain.size(); ++y) std::cout << main_chain[y] << " ";
        std::cout << "\n";
        typename Wrapper::seq_type::iterator it = cont.seq.begin() + cont.idxs[i] * 2 * level;
        int value = *(it + level - 1);
        size_t pos = get_pos(main_chain, level, value);
        main_chain.insert(main_chain.begin() + pos, it, it + level);
        std::cout << "main_chain after insert : ";
        for (size_t y = 0; y < main_chain.size(); ++y) std::cout << main_chain[y] << " ";
        std::cout << "\n";
    }
    main_chain.insert(
        main_chain.end(),
        cont.seq.begin() + cont.seq.size() / level * level,
        cont.seq.end()
    );
    print_color("FINAL BEFORE RETURNING");
    for (size_t y = 0; y < main_chain.size(); ++y) std::cout << main_chain[y] << " ";
    print_color("end \n");
    // cont.seq = main_chain;
    std::swap(main_chain, cont.seq);
}

template <typename Wrapper>
void test_container(char** av, Wrapper& cont, const std::string& name) {
    g_comps = 0;
    cont.begin_time = std::clock();
    get_numbers(av + 1, cont);
    merge_insert(cont, 1);
    std::cout << get_color() << "Time: to process a range of " << cont.seq.size()
              << " elements with " << name << " : "
              << (std::clock() - cont.begin_time) / (double)CLOCKS_PER_SEC * 1000 << "ms in "
              << g_comps << " comparisons" << ANSI_RESET << std::endl;
}

#endif // !PMERGEME_HPP
