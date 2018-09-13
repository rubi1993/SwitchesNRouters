#include <iostream>
#include <random>
#include <chrono>
#include "rules.h"
#include "data_structures.h"

#define NUMBER_OF_RULES 200000
#define NUMBER_OF_HEADERS 2000

int main() {
//    Rule test_rule("00*", "010*", 22, 34, -1, -1, "TCP", 21, "Rule 1");
//    Rule test_rule2("011*", "1111*", 22, 34, -1, -1, "TCP", 1, "Rule 2");
//    Rule test_rule3("11*", "01*", 22, 34, -1, -1, "TCP", 20, "Rule 3");
//    std::list<const Rule*> rule_table;
//    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
//    std::default_random_engine generator(seed);
//    std::uniform_int_distribution<int>binary_distribution(0,1);
//    std::uniform_int_distribution<int> length_distribution(0,8);
//    std::uniform_int_distribution<int> port_distribution(0,1000);
//    for(int i = 0; i < 4000; i++){
//        std::string source_address = "";
//        for(int j=0; j < length_distribution(generator); j++){
//            source_address += binary_distribution(generator) == 0 ? '0' : '1';
//        }
//        if(source_address.length() < 8){
//            source_address += "*";
//        }
//        std::string destination_address = "";
//        for(int j=0; j < length_distribution(generator); j++){
//            destination_address += binary_distribution(generator) == 0 ? '0' : '1';
//        }
//        if(destination_address.length() < 8){
//            destination_address += "*";
//        }
//        int source_port_start = port_distribution(generator);
//        std::uniform_int_distribution<int> end_port_distribution(source_port_start,1000);
//        int source_port_end = end_port_distribution(generator);
//        int destination_port_start = port_distribution(generator);
//        std::uniform_int_distribution<int> end_port_distribution2(destination_port_start,1000);
//        int destination_port_end = end_port_distribution2(generator);
//        std::string protocol;
//        if(binary_distribution(generator) == 0){
//            protocol = "TCP";
//        }else{
//            protocol = "UDP";
//        }
//        std::cout << "Rule " << i << ": " << source_address << " " << destination_address << " " << source_port_start \
//                  << "-" << source_port_end <<  " " << destination_port_start << "-" << destination_port_end << " " \
//                  << protocol << std::endl;
//        rule_table.push_back(new Rule(source_address, destination_address, source_port_start, source_port_end,\
//                             destination_port_start, destination_port_end, protocol, i, "Rule " + std::to_string(i)));
//    }
//    TreeTrieEpsilon test(rule_table);
//    std::cout << "------------------------------------------------------------------" << std::endl;
//    for(int i = 0; i < 20; i++){
//        std::string source_address = "";
//        for(int j=0; j < 8; j++){
//            source_address += binary_distribution(generator) == 0 ? '0' : '1';
//        }
//        std::string destination_address = "";
//        for(int j=0; j < 8; j++){
//            destination_address += binary_distribution(generator) == 0 ? '0' : '1';
//        }
//        int source_port = port_distribution(generator);
//        int destination_port = port_distribution(generator);
//        std::string protocol;
//        if(binary_distribution(generator) == 0){
//            protocol = "TCP";
//        }else{
//            protocol = "UDP";
//        }
//        PacketHeader header(source_address, destination_address, source_port, destination_port, protocol);
//        std::pair<const Rule*, int> returned_pair = test.get_matching_rule(header);
//        std::cout << "Header " << i << ": " << source_address << " " << destination_address << " " << source_port \
//                  <<  " " << destination_port << " " << protocol << " -> Matching Rule: " << \
//                  (returned_pair.first == nullptr ? "None" : returned_pair.first->rule_name) \
//                  << ", Nodes Seen: " << returned_pair.second <<std::endl;
//
//<<<<<<< Updated upstream
//    }
//=======
//    test.add_rule(test_rule);
//    test.add_rule(test_rule2);
//    test.add_rule(test_rule3);
//    //test.path_compress();
//    PacketHeader test_header("00000", "01011", 30, 853, "TCP");
//    std::pair<const Rule*, int> returned_pair = test.get_matching_rule(test_header);
//    std::cout << returned_pair.first->rule_name << ", Nodes Seen - " << returned_pair.second << std::endl;
    struct timespec start1, finish1,start2,finish2;
    double elapsed=0,elapsed2=0;
    std::list<const Rule*> rule_table;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int>binary_distribution(0,1);
    std::uniform_int_distribution<int> length_distribution(0,8);
    std::uniform_int_distribution<int> port_distribution(0,1000);
    int p_trie = 1;
    for(int i = 0; i < NUMBER_OF_RULES; i++){
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
    for(int i = 0; i < NUMBER_OF_HEADERS; i++){
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

        //time measurement for regular trie
        clock_gettime(CLOCK_REALTIME, &start1);
//        time(start1);
        std::pair<const Rule*, int> returned_pair = test.get_matching_rule(header);
 //       time(finish1);
        clock_gettime(CLOCK_REALTIME, &finish1);
        elapsed += (finish1.tv_sec - start1.tv_sec);
        elapsed += (finish1.tv_nsec - start1.tv_nsec);

        //time measurement for improved trie

//        clock_gettime(CLOCK_REALTIME, &start2);
//        time(start2);
        std::pair<const Rule*, int> returned_pair2 = test2.get_matching_rule(header);
//        time(finish2);
//        clock_gettime(CLOCK_REALTIME, &finish2);
//        elapsed2 += (finish2.tv_sec - start2.tv_sec);
//        elapsed2 += (finish2.tv_nsec - start2.tv_nsec);

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
    size_t memory_usage1 = RegularTrie::get_total_node_number() * RegularTrie::get_max_node_size() +
                           TrieOfTries::get_total_node_number() * TrieOfTries::get_max_node_size(),
           memory_usage2 = EpsilonT::get_total_node_number() * EpsilonT::get_max_node_size() +
                           TreeTrieEpsilonCluster::get_total_node_number() * TreeTrieEpsilonCluster::get_max_node_size();
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "Memory Usage Summary:\n" <<
                 "Trie of Tries: \n" <<
                 "Number of Top Level Trie Nodes - " << TrieOfTries::get_total_node_number() << std::endl <<
                 "Max Size of Top Level Trie Node - " << TrieOfTries::get_max_node_size() << std::endl <<
                 "Number of Regular Trie Nodes - " << RegularTrie::get_total_node_number() << std::endl <<
                 "Max Size of Regular Trie Node - " << RegularTrie::get_max_node_size() << std::endl <<
                 "Estimated Total Real Hardware Implementation Memory Usage - " << memory_usage1 << std::endl <<
                 "Estimated Bytes per Rule - " << memory_usage1 / NUMBER_OF_RULES << std::endl <<
                 "\nEpsilon Tree Trie: \n" <<
                 "Number of Top Level Trie Nodes - " << TreeTrieEpsilonCluster::get_total_node_number() << std::endl <<
                 "Max Size of Top Level Trie Node - " << TreeTrieEpsilonCluster::get_max_node_size() << std::endl <<
                 "Number of Regular Trie Nodes - " << EpsilonT::get_total_node_number() << std::endl <<
                 "Max Size of Regular Trie Node - " << EpsilonT::get_max_node_size() << std::endl <<
                 "Estimated Total Real Hardware Implementation Memory Usage - " << memory_usage2 << std::endl <<
                 "Estimated Bytes per Rule - " << memory_usage2 / NUMBER_OF_RULES << std::endl <<
                 std::endl;
    std::cout << "Done." << std::endl;
    elapsed2=test2.total_time;
   std::cout << "Done. Running time for Regular trie: "<< elapsed <<". Running time for improved trie: "<<elapsed2<< std::endl;
    return 0;
}
