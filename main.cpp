#include <iostream>
#include <random>
#include <chrono>
#include "rules.h"
#include "data_structures.h"

int main() {
//    Rule test_rule("00*", "010*", 22, 34, -1, -1, "TCP", 21, "Rule 1");
//    Rule test_rule2("011*", "1111*", 22, 34, -1, -1, "TCP", 1, "Rule 2");
//    Rule test_rule3("11*", "01*", 22, 34, -1, -1, "TCP", 20, "Rule 3");
//    std::list<const Rule*> rule_table;
//    rule_table.push_back(&test_rule);
//    rule_table.push_back(&test_rule2);
//    rule_table.push_back(&test_rule3);
//    TreeTrieEpsilon test(rule_table);
//    //EpsilonT test;
//
//    test.add_rule(test_rule);
//    test.add_rule(test_rule2);
//    test.add_rule(test_rule3);
//    //test.path_compress();
//    PacketHeader test_header("00000", "01011", 30, 853, "TCP");
//    std::pair<const Rule*, int> returned_pair = test.get_matching_rule(test_header);
//    std::cout << returned_pair.first->rule_name << ", Nodes Seen - " << returned_pair.second << std::endl;

    std::list<const Rule*> rule_table;
    //unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator;
    std::uniform_int_distribution<int>binary_distribution(0,1);
    std::uniform_int_distribution<int> length_distribution(0,8);
    std::uniform_int_distribution<int> port_distribution(0,1000);
    int p_trie = 1;
    for(int i = 0; i < 20000; i++){
        std::string source_address = "";
        for(int j=0; j < length_distribution(generator); j++){
            source_address += binary_distribution(generator) == 0 ? '0' : '1';
        }
        if(source_address.length() < 8){
            source_address += "*";
        }
        std::string destination_address = "";
        for(int j=0; j < length_distribution(generator); j++){
            destination_address += binary_distribution(generator) == 0 ? '0' : '1';
        }
        if(destination_address.length() < 8){
            destination_address += "*";
        }
        int source_port_start = port_distribution(generator);
        std::uniform_int_distribution<int> end_port_distribution(source_port_start,1000);
        int source_port_end = end_port_distribution(generator);
        int destination_port_start = port_distribution(generator);
        std::uniform_int_distribution<int> end_port_distribution2(destination_port_start,1000);
        int destination_port_end = end_port_distribution2(generator);
        std::string protocol;
        if(binary_distribution(generator) == 0){
            protocol = "TCP";
        }else{
            protocol = "UDP";
        }
        std::cout << "Rule " << i << ": " << source_address << " " << destination_address << " " << source_port_start \
                  << "-" << source_port_end <<  " " << destination_port_start << "-" << destination_port_end << " " \
                  << protocol << std::endl;
        rule_table.push_back(new Rule(source_address, destination_address, source_port_start, source_port_end,\
                             destination_port_start, destination_port_end, protocol, i, "Rule " + std::to_string(i)));
    }
    TrieOfTries test(rule_table);
    TreeTrieEpsilon test2(rule_table, p_trie);
    std::cout << "------------------------------------------------------------------" << std::endl;
    for(int i = 0; i < 200; i++){
        std::string source_address = "";
        for(int j=0; j < 8; j++){
            source_address += binary_distribution(generator) == 0 ? '0' : '1';
        }
        std::string destination_address = "";
        for(int j=0; j < 8; j++){
            destination_address += binary_distribution(generator) == 0 ? '0' : '1';
        }
        int source_port = port_distribution(generator);
        int destination_port = port_distribution(generator);
        std::string protocol;
        if(binary_distribution(generator) == 0){
            protocol = "TCP";
        }else{
            protocol = "UDP";
        }
        PacketHeader header(source_address, destination_address, source_port, destination_port, protocol);
        std::pair<const Rule*, int> returned_pair = test.get_matching_rule(header);
        std::pair<const Rule*, int> returned_pair2 = test2.get_matching_rule(header);
        std::cout << "Header " << i << ": " << source_address << " " << destination_address << " " << source_port \
                  <<  " " << destination_port << " " << protocol << " -> [TrieOfTries] Matching Rule: " << \
                  (returned_pair.first == nullptr ? "None" : returned_pair.first->rule_name) \
                  << ", Nodes Seen: " << returned_pair.second << " ?=? [TreeTrieEpsilon]" << \
                  (returned_pair2.first == nullptr ? "None" : returned_pair2.first->rule_name) << ", Nodes Seen:" <<\
                  returned_pair2.second << std::endl;
        if(returned_pair.first != returned_pair2.first){
            std::cout << "DISCREPANCY" << std::endl;
        }

    }
    std::cout << "Done." << std::endl;
    return 0;
}
/*
 * Header 0: 11011111 01101111 208 262 TCP -> [TrieOfTries] Matching Rule: Rule 199937, Nodes Seen: 18705 ?=? [TreeTrieEpsilon]Rule 199937, Nodes Seen:3631
Header 1: 10100101 11100111 783 898 UDP -> [TrieOfTries] Matching Rule: Rule 199660, Nodes Seen: 18749 ?=? [TreeTrieEpsilon]Rule 199660, Nodes Seen:5990
Header 2: 11110010 00101100 625 357 UDP -> [TrieOfTries] Matching Rule: Rule 199918, Nodes Seen: 18577 ?=? [TreeTrieEpsilon]Rule 199918, Nodes Seen:6671
Header 3: 00111000 10110001 137 516 TCP -> [TrieOfTries] Matching Rule: Rule 199907, Nodes Seen: 18712 ?=? [TreeTrieEpsilon]Rule 199907, Nodes Seen:6796
Header 4: 01100100 00100010 183 706 TCP -> [TrieOfTries] Matching Rule: Rule 199711, Nodes Seen: 18576 ?=? [TreeTrieEpsilon]Rule 199711, Nodes Seen:6002
Header 5: 00001100 10110000 652 563 TCP -> [TrieOfTries] Matching Rule: Rule 199981, Nodes Seen: 18691 ?=? [TreeTrieEpsilon]Rule 199981, Nodes Seen:1520
Header 6: 11100111 00011001 367 41 UDP -> [TrieOfTries] Matching Rule: Rule 199967, Nodes Seen: 18610 ?=? [TreeTrieEpsilon]Rule 199967, Nodes Seen:5942
Header 7: 01100111 01100101 875 908 UDP -> [TrieOfTries] Matching Rule: Rule 199090, Nodes Seen: 18527 ?=? [TreeTrieEpsilon]Rule 199090, Nodes Seen:6701
Header 8: 01111111 01101111 934 859 UDP -> [TrieOfTries] Matching Rule: Rule 199656, Nodes Seen: 18538 ?=? [TreeTrieEpsilon]Rule 199656, Nodes Seen:3496
Header 9: 10010101 10001101 584 467 UDP -> [TrieOfTries] Matching Rule: Rule 199768, Nodes Seen: 18842 ?=? [TreeTrieEpsilon]Rule 199768, Nodes Seen:6846
Header 10: 10110000 10000000 794 441 TCP -> [TrieOfTries] Matching Rule: Rule 199840, Nodes Seen: 18831 ?=? [TreeTrieEpsilon]Rule 199840, Nodes Seen:5964
Header 11: 11001011 00111100 774 430 UDP -> [TrieOfTries] Matching Rule: Rule 199918, Nodes Seen: 18684 ?=? [TreeTrieEpsilon]Rule 199918, Nodes Seen:6679
Header 12: 01100111 00011110 372 422 TCP -> [TrieOfTries] Matching Rule: Rule 199945, Nodes Seen: 18528 ?=? [TreeTrieEpsilon]Rule 199945, Nodes Seen:5993
Header 13: 11000000 10010000 952 369 TCP -> [TrieOfTries] Matching Rule: Rule 199892, Nodes Seen: 18706 ?=? [TreeTrieEpsilon]Rule 199892, Nodes Seen:3450
Header 14: 10100010 01010000 459 912 UDP -> [TrieOfTries] Matching Rule: Rule 199659, Nodes Seen: 18594 ?=? [TreeTrieEpsilon]Rule 199659, Nodes Seen:6691
Header 15: 10010110 00111111 967 785 UDP -> [TrieOfTries] Matching Rule: Rule 199783, Nodes Seen: 18377 ?=? [TreeTrieEpsilon]Rule 199783, Nodes Seen:3473
Header 16: 10010001 10110100 393 390 UDP -> [TrieOfTries] Matching Rule: Rule 199967, Nodes Seen: 18730 ?=? [TreeTrieEpsilon]Rule 199967, Nodes Seen:5980
Header 17: 01000000 11101010 148 688 TCP -> [TrieOfTries] Matching Rule: Rule 199597, Nodes Seen: 18691 ?=? [TreeTrieEpsilon]Rule 199597, Nodes Seen:6804
Header 18: 01000101 10010000 190 297 UDP -> [TrieOfTries] Matching Rule: Rule 199964, Nodes Seen: 18697 ?=? [TreeTrieEpsilon]Rule 199964, Nodes Seen:6147
Header 19: 11010010 00011001 115 90 UDP -> [TrieOfTries] Matching Rule: Rule 199745, Nodes Seen: 18656 ?=? [TreeTrieEpsilon]Rule 199745, Nodes Seen:6689
 */