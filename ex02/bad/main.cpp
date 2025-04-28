#include "PmergeMe.hpp"
#include "colors.hpp"

int main(int ac, char** av) {
    try {
        if (ac < 2) {
            print_color("Usage: ./PmergeMe n1 n2 n3 n4 ...", std::cerr);
            exit(EXIT_FAILURE);
        }
        Container<std::vector> vector;
        Container<std::deque> deque;

        get_numbers(av + 1, vector);
        merge_insert(vector);
        print_color("FINAL");
        for (size_t i = 0; i < vector.seq.size(); i++) std::cout << vector.seq[i] << " ";
        print_color("----------");
        std::cout << get_color() << "Time: to process a range of " << vector.seq.size()
                  << " elements with std::vector : "
                  << (std::clock() - vector.begin_time) / (double)CLOCKS_PER_SEC * 1000 << "ms in "
                  << g_comps << " comparisons" << std::endl;

        g_comps = 0;
        get_numbers(av + 1, deque);
        merge_insert(deque);
        std::cout << get_color() << "Time: to process a range of " << deque.seq.size()
                  << " elements with std::deque : "
                  << (std::clock() - deque.begin_time) / (double)CLOCKS_PER_SEC * 1000 << "ms in "
                  << g_comps << " comparisons" << ANSI_RESET << std::endl;

    } catch (std::exception& e) { print_color(e.what(), std ::cerr); }
}
