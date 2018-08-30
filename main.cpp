#include <iostream>
#include "rules.h"
#include "data_structures.h"

int main() {
    RegularTrie test;
    Rule test_rule("*", "010*", 22, 34, -1, -1, "TCP", 1, "Rule 1");
    test.add_rule("010*", test_rule);
    PacketHeader test_header("00000", "01011", 30, 853, "TCP");
    const Rule* returned_rule = test.get_matching_rule(test_header);
    std::cout << returned_rule->rule_name << std::endl;
    std::cout << "Done." << std::endl;
    return 0;
}