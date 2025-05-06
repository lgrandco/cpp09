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

bool comp(int a, int b);

template <typename Container>
void push_biggest(Container& dst, const Container& src, size_t pair_size) {
    if (src.begin() == src.end()) { return; }
    for (typename Container::const_iterator it = src.begin() + 3 * pair_size;
         it < src.begin() + (src.size() / pair_size) * pair_size;
         it += 2 * pair_size) {
        for (size_t i = 0; i < pair_size; ++i) { dst.push_back(*(it + i)); }
    }
}

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
void swap_pairs(Iterator a, Iterator b, size_t pair_size) {
    for (size_t i = 0; i < pair_size; ++i) {
        int tmp = *a;
        *a = *b;
        *b = tmp;
        ++a;
        ++b;
    }
}

template <typename Cont>
size_t get_pos(const Cont& seq, size_t right, size_t pair_size, int value) {
    size_t left = 0;
    size_t middle;
    while (left < right) {
        ++g_comps;
        middle = left + (right - left) / 2;
        int middle_value = seq[middle * pair_size + pair_size - 1];
        if (middle_value == value) return middle * pair_size;
        if (middle_value < value) left = middle + 1;
        else right = middle;
    }
    return left * pair_size;
}

template <typename Wrapper>
void merge_insert(Wrapper& cont, size_t pair_size) {
    if (cont.seq.size() / pair_size < 2) return;
    for (typename Wrapper::seq_type::iterator it = cont.seq.begin() + pair_size;
         it < cont.seq.begin() + (cont.seq.size() / pair_size) * pair_size;
         it += 2 * pair_size) {
        typename Wrapper::seq_type::iterator last = it - pair_size;
        if (comp(*(it + pair_size - 1), *(last + pair_size - 1))) swap_pairs(last, it, pair_size);
    }
    merge_insert(cont, pair_size * 2);
    typename Wrapper::seq_type main_chain;
    // lvl * 2 for first 2 pairs
    for (typename Wrapper::seq_type::iterator it = cont.seq.begin();
         it < cont.seq.begin() + pair_size * 2;
         ++it)
        main_chain.push_back(*it);

    cont.is_alone = cont.seq.size() % (2 * pair_size) >= pair_size;
    get_idxs(cont.idxs, cont.seq.size() / (2 * pair_size) + cont.is_alone);

    push_biggest(main_chain, cont.seq, pair_size);
    size_t inserted = 1;
    for (size_t i = 0; i < cont.idxs.size(); ++i) {
        typename Wrapper::seq_type::iterator it = cont.seq.begin() + cont.idxs[i] * 2 * pair_size;
        int value = *(it + pair_size - 1);
        size_t pos = get_pos(main_chain, cont.idxs[i] + inserted++, pair_size, value);
        main_chain.insert(main_chain.begin() + pos, it, it + pair_size);
    }
    main_chain.insert(
        main_chain.end(),
        cont.seq.begin() + cont.seq.size() / pair_size * pair_size,
        cont.seq.end()
    );
    std::swap(main_chain, cont.seq);
}

template <typename Wrapper>
void test_container(char** av, Wrapper& cont, const std::string& name) {
    g_comps = 0;
    cont.begin_time = std::clock();
    get_numbers(av + 1, cont);
    if (name == "std::vector") {
        std::cout << get_color() << "Entered numbers:" << ANSI_RESET << std::endl;
        for (typename Wrapper::seq_type::iterator it = cont.seq.begin(); it != cont.seq.end();
             ++it) {
            std::cout << get_color() << *it << " ";
        }
        std::cout << ANSI_RESET << std::endl;
        print_sep();
    }
    merge_insert(cont, 1);
    if (name == "std::vector") {
        std::cout << get_color() << "Sorted numbers:" << ANSI_RESET << std::endl;
        for (typename Wrapper::seq_type::iterator it = cont.seq.begin(); it != cont.seq.end();
             ++it) {
            std::cout << get_color() << *it << " ";
        }
        std::cout << ANSI_RESET << std::endl;
        print_sep();
    }
    std::cout << get_color() << "Time: to process a range of " << cont.seq.size()
              << " elements with " << name << " : "
              << (std::clock() - cont.begin_time) / (double)CLOCKS_PER_SEC * 1000 << "ms in "
              << g_comps << " comparisons" << ANSI_RESET << std::endl;
}

#endif // !PMERGEME_HPP
