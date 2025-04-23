#include "BitcoinExchange.hpp"
#include "colors.hpp"

int main(int ac, char** av) {
    try {
        if (ac != 2) {
            print_color("Usage:" + std::string(av[0]) + " <filename>", std::cerr);
            return EXIT_FAILURE;
        }
        std::ifstream in(av[1]);
        if (!in) {
            get_color(std::cerr);
            perror(av[1]);
            std::cerr << ANSI_RESET;
            return EXIT_FAILURE;
        }
        std::ifstream database("data.csv");
        if (!database) {
            get_color(std::cerr);
            perror("data.csv");
            std::cerr << ANSI_RESET;
            return EXIT_FAILURE;
        }
        return (exchange(in, database));
    } catch (const std::exception& e) {
        print_color(e.what(), std::cerr);
        return EXIT_FAILURE;
    }
}