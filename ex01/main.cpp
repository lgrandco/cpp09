#include "RPN.hpp"
#include "colors.hpp"

int main(int ac, char** av) {
    try {
        if (ac != 2) {
            print_color(
                "Usage: ./RPN \"n1 n2 op1 ... \"\nex: ./RPN \"8 9 * 9 - 9 - 9 - 4 - 1 +\"",
                std::cerr
            );
            exit(EXIT_FAILURE);
        }
        rpn(av[1]);
    } catch (std::exception& e) { print_color(e.what(), std ::cerr); }
}
