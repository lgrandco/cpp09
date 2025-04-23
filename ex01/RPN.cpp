#include "RPN.hpp"

void doOp(std::stack<long >& stack, char op) {

    long second = stack.top();
    stack.pop();
    long first = stack.top();
    stack.pop();
    if (op == '/' && second == 0) throw std::runtime_error("Division by zero");
    stack.push(
        op == '+'   ? first + second
        : op == '-' ? first - second
        : op == '*' ? first * second
                    : first / second
    );
}

void rpn(char* s) {
    std::stack<long> stack;
    size_t nums = 0;
    for (size_t i = 0; s[i]; i++) {
        if (i % 2 && s[i] != ' ') throw std::runtime_error("Bad input format");
        else if (i % 2 < 1) {
            if (strchr("+-/*", s[i]) && nums < 2) {
                throw std::runtime_error("Bad input format");
            } else if (strchr("+-/*", s[i])) {
                doOp(stack, s[i]);
                --nums;
            } else if (isdigit(s[i])) {
                nums++;
                stack.push(s[i] - '0');
            } else throw std::runtime_error("Bad input format");
        }
    }
    if (stack.size() != 1)
        throw std::runtime_error("Number of digits not matching number of operators");
    std::cout << get_color() << stack.top() << ANSI_RESET << std::endl;
}
