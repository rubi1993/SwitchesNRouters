#include <iostream>
#include "rules.h"
#include "data_structures.h"

int main() {
    EpsilonT test;
    Rule test_rule("*", "010*", 22, 34, -1, -1, "TCP", 1, "Rule 1");
    Rule test_rule2("*", "1111*", 22, 34, -1, -1, "TCP", 1, "Rule 2");
    test.add_rule("010*", test_rule);
    PacketHeader test_header("00000", "01011", 30, 853, "TCP");
    const Rule* returned_rule = test.get_matching_rule(test_header);
    std::cout << returned_rule->rule_name << std::endl;
    test.remove_rule("*010*", test_rule);
    std::cout << "Done." << std::endl;
    return 0;
}