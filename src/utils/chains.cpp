#include "chains.hpp"

namespace chains
{
    char nibble_to_char(const uint8_t &n)
    {

        if (n < 10)
            return '0' + n;

        if (n >= 10 && n <= 15)
            return 'A' + (n - 10);

        throw std::invalid_argument("Invalid nibble");
    }

    uint8_t char_to_nibble(const char &c)
    {
        if (c >= '0' && c <= '9')
            return c - '0';

        if (c >= 'A' && c <= 'F')
            return 10 + (c - 'A');

        if (c >= 'a' && c <= 'f')
            return 10 + (c - 'A');

        throw std::invalid_argument("Invalid hex char");
    }

    std::vector<uint8_t> get_hex_key_from_string(const std::string &chain)
    {
        std::vector<uint8_t> res;
        for (int i = 0; i < chain.length(); i++)
        {
            res.push_back(char_to_nibble(chain[i]));
        }

        return res;
    }

    std::vector<uint8_t> get_prefix_hex(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b)
    {
        std::vector<uint8_t> res;

        size_t shortest_length = std::min(a.size(), b.size());

        size_t i = 0;
        while (i < shortest_length && a[i] == b[i])
        {
            res.push_back(a[i]);
            i++;
        }

        return res;
    }

    std::string get_prefix_string(const std::string &a, const std::string &b)
    {
        if (a.length() == 0 || b.length() == 0 || a == "" || b == "")
        {
            return "";
        }

        std::string res = "";
        const std::string &shortest_key = a.length() < b.length() ? a : b;
        const std::string &longest_key = (shortest_key == a) ? b : a;

        for (int i = 0; i < shortest_key.length() && shortest_key[i] == longest_key[i]; i++)
        {
            res.push_back(shortest_key[i]);
        }

        return res;
    }

    std::vector<uint8_t> get_sufix(const std::vector<uint8_t> &prefix, const std::vector<uint8_t> &chain)
    {
        if (prefix.size() == 0 || prefix.size() >= chain.size())
        {
            return {};
        }

        std::vector<uint8_t> res;
        int i = 0;
        // Hacer la version de calcular el min length entre ambos y usar ese min length para extraer el prefijo
        // Hacer uso de return std::vector<uint8_t>(chain.begin() + i, chain.end());
        while (i < prefix.size() && prefix[i] == chain[i])
        {
            i++;
        }

        if (i < prefix.size())
        {
            throw std::invalid_argument("Invalid prefix");
        }

        for (int j = i; j < chain.size(); j++)
        {
            res.push_back(chain[j]);
        }
        return res;
    }

    std::vector<uint8_t> strip_first_nibble(const std::vector<uint8_t> &chain)
    {
        if (chain.size() <= 1)
        {
            return {};
        }

        return std::vector<uint8_t>(chain.begin() + 1, chain.end());
    }

    void print_hex_nibble_chain(const std::vector<uint8_t> &chain)
    {
        std::string acum;
        for (int i = 0; i < chain.size(); i++)
        {
            acum.push_back(nibble_to_char(chain[i]));
        }

        std::cout << "Nibble chain: " << acum << std::endl;
    }

    bool areEqual(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b)
    {
        if (a.size() != b.size())
        {
            return false;
        }

        size_t i = 0;
        size_t size = a.size();

        while (i < size && a[i] == b[i])
        {
            i++;
        }

        return i == size;
    }

    std::vector<uint8_t> combineBothHexChains(const std::vector<uint8_t> &a, const std::vector<uint8_t> &b)
    {

        if (a.empty() && b.empty())
        {
            return {};
        }

        if (a.empty())
        {
            return b;
        }

        if (b.empty())
        {
            return a;
        }

        std::vector<uint8_t> aux;
        aux.insert(aux.end(), a.begin(), a.end());
        aux.insert(aux.end(), b.begin(), b.end());

        return aux;
    }

}