#include <iostream>
#include "src/PatriciaTrie.hpp"
#include "src/utils/chains.hpp"
#include "src/utils/test.hpp"

#include <string>
#include <cassert>

int main()
{
    std::vector<std::string> keys_test = {

        // 1. Todos los nibbles individuales (cobertura base del trie)
        "0",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9",
        "A",
        "B",
        "C",
        "D",
        "E",
        "F",

        // 2. Branching básico (pares y extremos)
        "00",
        "0F",
        "F0",
        "FF",
        "10",
        "1F",
        "F1",
        "20",
        "2F",
        "A0",
        "0A",
        "B0",
        "0B",
        "C0",
        "0C",
        "D0",
        "0D",
        "E0",
        "0E",

        // 3. Prefijos progresivos
        "A",
        "AB",
        "ABC",
        "ABCD",
        "ABCDE",
        "ABCDEF",
        "B",
        "BC",
        "BCD",
        "BCDE",
        "C",
        "CD",
        "CDE",
        "CDEF",

        // 4. Patrones tipo palabra hex (muy usados en MPT / Ethereum style)
        "DEAD",
        "BEEF",
        "CAFE",
        "FACE",
        "FEED",
        "C0DE",
        "BADA55",

        // 5. Colisiones estructurales profundas
        "DEADBEEF",
        "DEADBABE",
        "DEADCAFE",
        "FEEDFACE",

        // 6. Variaciones de prefijo común
        "A0A0",
        "A0A1",
        "A0AF",
        "B0B0",
        "B0BF",
        "C0C0",
        "C0CF",
        "D0D0",
        "D0DF",
        "E0E0",
        "E0EF",
        "F0F0",
        "F0FF",

        // 7. Mezclas largas para forzar extension + branch nodes
        "ABCDEF00",
        "ABCDEF01",
        "ABCDEF0F",
        "ABCDEF10",
        "ABCDEFAB",
        "ABCDEFCD",

        // 8. Casos límite de divergencia tardía
        "AAAA",
        "AAAB",
        "AAAF",
        "ABAA",
        "ABAB",
        "ABAF",

        // 9. Secuencias tipo incremento hex
        "0123",
        "012F",
        "01AF",
        "0ABC",
        "0FFF",

    };

    PatriciaTrie patricia;

    for (int i = 0; i < keys_test.size(); i++)
    {
        patricia.insert(chains::get_hex_key_from_string(keys_test[i]), "val");
    }

    for (int i = 0; i < keys_test.size(); i++)
    {
        test::expect(patricia.contains(chains::get_hex_key_from_string(keys_test[i])), keys_test[i]);
    }
}