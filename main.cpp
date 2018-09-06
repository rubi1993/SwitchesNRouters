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
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int>binary_distribution(0,1);
    std::uniform_int_distribution<int> length_distribution(0,8);
    std::uniform_int_distribution<int> port_distribution(0,1000);
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
    TreeTrieEpsilon test2(rule_table);
    std::cout << "------------------------------------------------------------------" << std::endl;
    for(int i = 0; i < 20; i++){
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

    }
    std::cout << "Done." << std::endl;
    return 0;
}