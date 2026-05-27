#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <cstdint>

namespace chains
{
    char nibble_to_char(const uint8_t &n);

    uint8_t char_to_nibble(const char &c);

    std::vector<uint8_t> get_hex_key_from_string(const std::string &chain);

    std::vector<uint8_t> get_prefix_hex(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b);

    std::string get_prefix_string(const std::string &a, const std::string &b);

    std::vector<uint8_t> get_sufix(const std::vector<uint8_t> &prefix, const std::vector<uint8_t> &chain);

    std::vector<uint8_t> strip_first_nibble(const std::vector<uint8_t> &chain);

    void print_hex_nibble_chain(const std::vector<uint8_t> &chain);

    bool areEqual(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b);

    std::vector<uint8_t> combineBothHexChains(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b);
}