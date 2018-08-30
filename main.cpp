#include <iostream>
#include "rules.h"
#include "data_structures.h"

int main() {
    RegularTrie test;
    test.add_rule("010*", "Test Rule 1");
    std::cout << "Done." << std::endl;
    return 0;
}