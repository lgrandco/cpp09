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

        // print_color("FINAL");
        // for (size_t i = 0; i < vector.seq.size(); i++) std::cout << vector.seq[i] << " ";
        // print_color("----------");
        test_container(av, vector, "std::vector");
        // test_container(av, deque, "std::deque");
    } catch (std::exception& e) { print_color(e.what(), std ::cerr); }
}
