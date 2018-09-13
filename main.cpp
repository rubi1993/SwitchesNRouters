#include <iostream>
#include <fstream>
#include <random>
#include <chrono>
#include "rules.h"
#include "data_structures.h"


std::list<const Rule*> generate_rules(int num_of_rules, bool print_rules = false){
    std::list<const Rule *> rule_table;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> binary_distribution(0, 1);
    std::uniform_int_distribution<int> length_distribution(0, 8);
    std::uniform_int_distribution<int> port_distribution(0, 1000);
    for (int i = 0; i < num_of_rules; i++) {
        std::string source_address = "";
        for (int j = 0; j < length_distribution(generator); j++) {
            source_address += binary_distribution(generator) == 0 ? '0' : '1';
        }
        if (source_address.length() < 8) {
            source_address += "*";
        }
        std::string destination_address = "";
        for (int j = 0; j < length_distribution(generator); j++) {
            destination_address += binary_distribution(generator) == 0 ? '0' : '1';
        }
        if (destination_address.length() < 8) {
            destination_address += "*";
        }
        int source_port_start = port_distribution(generator);
        std::uniform_int_distribution<int> end_port_distribution(source_port_start, 1000);
        int source_port_end = end_port_distribution(generator);
        int destination_port_start = port_distribution(generator);
        std::uniform_int_distribution<int> end_port_distribution2(destination_port_start, 1000);
        int destination_port_end = end_port_distribution2(generator);
        std::string protocol;
        if (binary_distribution(generator) == 0) {
            protocol = "TCP";
        } else {
            protocol = "UDP";
        }
        if(print_rules) {
            std::cout << "Rule " << i << ": " << source_address << " " << destination_address << " "
                      << source_port_start \
                        << "-" << source_port_end << " " << destination_port_start << "-" << destination_port_end << " " \
                        << protocol << std::endl;
        }
        rule_table.push_back(new Rule(source_address, destination_address, source_port_start, source_port_end, \
                                 destination_port_start, destination_port_end, protocol, i,
                                      "Rule " + std::to_string(i)));
    }
    return rule_table;
}

std::list<const PacketHeader*> generate_headers(int num_of_headers, bool print_headers = false){
    std::list<const PacketHeader*> header_list;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> binary_distribution(0, 1);
    std::uniform_int_distribution<int> length_distribution(0, 8);
    std::uniform_int_distribution<int> port_distribution(0, 1000);
    for (int i = 0; i < num_of_headers; i++) {
        std::string source_address = "";
        for (int j = 0; j < 8; j++) {
            source_address += binary_distribution(generator) == 0 ? '0' : '1';
        }
        std::string destination_address = "";
        for (int j = 0; j < 8; j++) {
            destination_address += binary_distribution(generator) == 0 ? '0' : '1';
        }
        int source_port = port_distribution(generator);
        int destination_port = port_distribution(generator);
        std::string protocol;
        if (binary_distribution(generator) == 0) {
            protocol = "TCP";
        } else {
            protocol = "UDP";
        }
        header_list.push_back(new PacketHeader(source_address, destination_address, source_port, destination_port, protocol));
    }
    return header_list;
}

std::pair<std::pair<size_t, long double>, std::pair<size_t, long double>> run_single_test(std::list<const Rule*>& rule_table,
                                                                                          std::list<const PacketHeader*>& headers,
                                                                                          bool print = true){
    RegularTrie::reset_tracking_values();
    TrieOfTries::reset_tracking_values();
    EpsilonT::reset_tracking_values();
    TreeTrieEpsilonCluster::reset_tracking_values();
    struct timespec start1, finish1;
    long double elapsed = 0, elapsed2 = 0;

    TrieOfTries test(rule_table);
    TreeTrieEpsilon test2(rule_table);

    for(const PacketHeader* header : headers){
        clock_gettime(CLOCK_REALTIME, &start1);
        std::pair<const Rule *, int> returned_pair = test.get_matching_rule(*header);
        clock_gettime(CLOCK_REALTIME, &finish1);
        elapsed += (finish1.tv_sec - start1.tv_sec);
        elapsed += (finish1.tv_nsec - start1.tv_nsec) / 1000000000.0;
        std::pair<const Rule *, int> returned_pair2 = test2.get_matching_rule(*header);
    }
    elapsed2 = test2.total_time;
    size_t memory_usage1 = RegularTrie::get_total_node_number() * RegularTrie::get_max_node_size() +
                           TrieOfTries::get_total_node_number() * TrieOfTries::get_max_node_size(),
            memory_usage2 = EpsilonT::get_total_node_number() * EpsilonT::get_max_node_size() +
                            TreeTrieEpsilonCluster::get_total_node_number() *
                            TreeTrieEpsilonCluster::get_max_node_size();
    if(print){
        std::cout << "------------------------------------------------------------------" << std::endl;
        std::cout << "Memory Usage Summary:\n" <<
                  "Trie of Tries: \n" <<
                  "Number of Top Level Trie Nodes - " << TrieOfTries::get_total_node_number() << std::endl <<
                  "Max Size of Top Level Trie Node - " << TrieOfTries::get_max_node_size() << std::endl <<
                  "Number of Regular Trie Nodes - " << RegularTrie::get_total_node_number() << std::endl <<
                  "Max Size of Regular Trie Node - " << RegularTrie::get_max_node_size() << std::endl <<
                  "Estimated Total Real Hardware Implementation Memory Usage - " << memory_usage1 << std::endl <<
                  "Estimated Bytes per Rule - " << memory_usage1 / rule_table.size() << std::endl <<
                  "\nEpsilon Tree Trie: \n" <<
                  "Number of Top Level Trie Nodes - " << TreeTrieEpsilonCluster::get_total_node_number() << std::endl <<
                  "Max Size of Top Level Trie Node - " << TreeTrieEpsilonCluster::get_max_node_size() << std::endl <<
                  "Number of Regular Trie Nodes - " << EpsilonT::get_total_node_number() << std::endl <<
                  "Max Size of Regular Trie Node - " << EpsilonT::get_max_node_size() << std::endl <<
                  "Estimated Total Real Hardware Implementation Memory Usage - " << memory_usage2 << std::endl <<
                  "Estimated Bytes per Rule - " << memory_usage2 / rule_table.size() << std::endl <<
                  std::endl;

        std::cout << "Running time for Trie of Tries: " << elapsed << "\nRunning time for Tree Trie Epsilon: " << elapsed2
                  << std::endl << std::endl;
    }
    return std::make_pair(std::make_pair(memory_usage1 / rule_table.size(), elapsed),
                          std::make_pair(memory_usage2 / rule_table.size(), elapsed2));
}

void run_tests(int num_of_experiments = 10, std::ostream& output = std::cout,
               int num_of_rules = 10000, int num_of_headers = 1000,
               bool verbose = false, bool print_rules = false,
               bool print_headers = false, bool print_summaries= false){
    size_t average_per_rule1 = 0, average_per_rule2 = 0;
    long double average_runtime1 = 0, average_runtime2 = 0;
    for(int i = 0; i < num_of_experiments; i++){
        std::list<const Rule *> rule_table = generate_rules(num_of_rules, print_rules);
        std::list<const PacketHeader*> header_list = generate_headers(num_of_headers, print_headers);
        auto test_results = run_single_test(rule_table, header_list, print_summaries);
        average_per_rule1 += test_results.first.first / num_of_experiments;
        average_per_rule2 += test_results.second.first / num_of_experiments;
        average_runtime1 += test_results.first.second / num_of_experiments;
        average_runtime2 += test_results.second.second / num_of_experiments;
    }
    if(verbose){
        output << "\nAverage Results for " << num_of_experiments << " Experiments:\n" <<
                  "Trie of Tries:\n Bytes per Rule - " << average_per_rule1 <<
                  "\n Runtime - " << average_runtime1 << std::endl <<
                  "Epsilon Tree Trie:\n Bytes per Rule - " << average_per_rule2 <<
                  "\n Runtime - " << average_runtime2  << std::endl;
    }else{
        output << average_per_rule1 << " " <<  average_runtime1 << " " <<
                  average_per_rule2 << " "<< average_runtime2 << std::endl;
    }

}

int main() {
    std::ofstream file;
    file.open("test.txt");
    for(int i = 0; i < 100000; i  += 10000){
        run_tests(2, file, 50000, 1000, false, false, false, true);
    }
    file.close();
    return 0;
}
