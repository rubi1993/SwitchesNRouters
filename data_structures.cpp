//
// Created by igor.a on 8/30/18.
//

#include "data_structures.h"
#include <iostream>
#include <cmath>

RegularTrie::Node * RegularTrie::createPrefixNode(std::string prefix){
    if(root == nullptr){
        root = new Node();
    }
    Node* current = root;
    for(char c : prefix){
        if(c == '0'){
            if(current->zero == nullptr){
                current->zero = new Node(current);
            }
            current = current->zero;
        }else if(c == '1'){
            if(current->one == nullptr){
                current->one = new Node(current);
            }
            current = current->one;
        }
    }
    return current;
}

void RegularTrie::add_rule(std::string prefix, const Rule& rule) {
    Node* node = createPrefixNode(prefix);
    node->rules.push_back(&rule);
}

void RegularTrie::remove_rule(std::string prefix, const Rule &rule) {
    Node* node = createPrefixNode(prefix);
    node->rules.remove(&rule);
    while(node != nullptr && node->rules.size() == 0){
        Node* temp = node;
        node = node->prev;
        if(node != nullptr){
            if(node->zero == temp){
                node->zero = nullptr;
            }else{
                node->one = nullptr;
            }
        }
        delete temp;
    }
    if(node == nullptr){
        root = nullptr;
    }
}

void RegularTrie::destroySubtree(RegularTrie::Node* subroot){
    if(subroot != nullptr){
        destroySubtree(subroot->zero);
        destroySubtree(subroot->one);
        delete subroot;
    }
}

RegularTrie::~RegularTrie() {
    destroySubtree(root);
}

const Rule* RegularTrie::get_matching_rule(const PacketHeader& header) const {
    if(root == nullptr){
        return nullptr;
    }
    Node* current = root;
    const Rule* best_match = nullptr;
    std::string address = header.destination_address;
    for(char c : address){

        int best_match_priority = INT32_MAX;
        for(const Rule* r : current->rules){
            if((r->source_port_start == -1 || \
               (r->source_port_start <= header.source_port && r->source_port_end >= header.source_port)) \
               && (r->destination_port_start == -1 || \
                  (r->destination_port_start <= header.destination_port && r->destination_port_end >= header.destination_port)) \
               && (r->protocol == header.protocol || r->protocol == "*") && r->priority <= best_match_priority){
                best_match = r;
                best_match_priority = r->priority;
            }
        }
        if(c == '0'){
            if(current->zero == nullptr){
                return best_match;
            }
            current = current->zero;
        }else if(c == '1'){
            if(current->one == nullptr){
                return best_match;
            }
            current = current->one;
        }
    }
    return best_match;
}


const Rule* EpsilonT::get_matching_rule(const PacketHeader& header) const {
    if(root == nullptr){
        return nullptr;
    }
    Node* current = root;
    const Rule* best_match = nullptr;
    std::string address = header.destination_address;
    for(char c : address){
        int best_match_priority = INT32_MAX;
        while(current->mid!= nullptr)
        {
            if((current->rule->source_port_start == -1 || \
               (current->rule->source_port_start <= header.source_port && current->rule->source_port_end >= header.source_port)) \
               && (current->rule->destination_port_start == -1 || \
                  (current->rule->destination_port_start <= header.destination_port && current->rule->destination_port_end >= header.destination_port)) \
               && (current->rule->protocol == header.protocol || current->rule->protocol == "*") && current->rule->priority <= best_match_priority){
                best_match = current->rule;
                best_match_priority = current->rule->priority;
            }
            current=current->mid;
        }
        if(c == '0'){
            if(current->zero == nullptr){
                return best_match;
            }
            current = current->zero;
        }else if(c == '1'){
            if(current->one == nullptr){
                return best_match;
            }
            current = current->one;
        }
    }
    return best_match;
}

EpsilonT::Node * EpsilonT::createPrefixNode(std::string prefix){
    if(root == nullptr){
        root = new Node();
    }
    Node* current = root;
    for(char c : prefix){
        if(c == '0'){
            if(current->zero == nullptr){
                current->zero = new Node(current);
            }
            current = current->zero;
        }else if(c == '1'){
            if(current->one == nullptr){
                current->one = new Node(current);
            }
            current = current->one;
        }
    }
    return current;
}


void EpsilonT::add_rule(std::string prefix, const Rule& rule) {
    Node* node = createPrefixNode(prefix);
    while(node->mid!= nullptr){
        node=node->mid;
    }
    node->mid=new Node(&rule);
}


void EpsilonT::remove_rule(std::string prefix, const Rule &rule) {
    Node* node = createPrefixNode(prefix);
    while  (node->rule!=&rule)
    {
        node=node->mid;
    }
    Node *temp= nullptr;
    if(node->mid!= nullptr)
    {
        temp = node->mid;
        node->prev->mid=temp;
    }
    else if(node->one!= nullptr){
        temp = node->one;
        node->prev->one=temp;

    }
    else if(node->zero!= nullptr){
        temp = node->zero;
        node->prev->zero=temp;
    }

//    if(node == nullptr){
//        root = nullptr;
//    }
}


static int HiCuts::spmf(int n) {
    return n*SPFAC;
}

int HiCuts::Node::num_of_cuts_needed(){
    int n=num_of_rules();
    int nump=std::max(4,std::sqrt(n));
    int smC=0;
    while(smC<spmf(n))
    {

        std::list<Rule>::iterator ptr;
        for(  ptr = rules.begin() ; ptr != rules.end() ; ptr++)
        {
            smC+=ptr.operator*().num_of_partitions_colliding(partition_size)

        }
    }
}
