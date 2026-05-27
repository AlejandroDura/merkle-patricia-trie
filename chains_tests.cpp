#include <iostream>
#include "src/PatriciaTrie.hpp"
#include "src/utils/chains.hpp"

int main()
{
    PatriciaTrie patricia_trie;

    std::string prefix = chains::get_prefix_string("sabsb", "sabsb");

    std::cout << "Prefix strings: " << prefix << std::endl;

    //{0xA, 0xB, 0xC, 0xD}
    //{0xA, 0xB, 0xE}
    // CALCULATE PREFIX
    std::string A = "A";
    std::string B = "AAAABB";

    std::vector<uint8_t> prefix_hex = chains::get_prefix_hex(
        chains::get_hex_key_from_string(A),
        chains::get_hex_key_from_string(B));

    chains::print_hex_nibble_chain(prefix_hex);

    // CALCULATE SUFIX
    prefix = "";
    std::string key = "AB";
    std::vector<uint8_t> sufix_hex = chains::get_sufix(
        chains::get_hex_key_from_string(prefix),
        chains::get_hex_key_from_string(key));

    chains::print_hex_nibble_chain(sufix_hex);
    return 0;
}