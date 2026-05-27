#include "test.hpp"
#include <iostream>

namespace test
{
    constexpr const char *BOLD = "\033[1m";
    constexpr const char *GREEN = "\033[32m";
    constexpr const char *RED = "\033[31m";
    constexpr const char *RESET = "\033[0m";

    void expect(bool condition, const std::string &name)
    {
        if (condition)
            std::cout << BOLD << GREEN << "[PASS] " << name << RESET << "\n";
        else
            std::cout << BOLD << RED << "[FAIL] " << name << RESET << "\n";
    }
}