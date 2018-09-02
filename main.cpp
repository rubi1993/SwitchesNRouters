#include <iostream>
#include "rules.h"
#include "data_structures.h"

int main() {
    Rule test_rule("11*", "010*", 22, 34, -1, -1, "TCP", 1, "Rule 1");
    Rule test_rule2("011", "1111*", 22, 34, -1, -1, "TCP", 1, "Rule 2");
    Rule test_rule3("00*", "01*", 22, 34, -1, -1, "TCP", 20, "Rule 3");
    std::list<const Rule*> rule_table;
    rule_table.push_back(&test_rule);
    rule_table.push_back(&test_rule2);
    rule_table.push_back(&test_rule3);
    TreeTrieEpsilon test(rule_table);
    PacketHeader test_header("00000", "01011", 30, 853, "TCP");
    const Rule* returned_rule = test.get_matching_rule(test_header);
    std::cout << returned_rule->rule_name << std::endl;
    std::cout << "Done." << std::endl;
    return 0;
}