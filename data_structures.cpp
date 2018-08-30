//
// Created by igor.a on 8/30/18.
//

#include "data_structures.h"
#include <iostream>

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
        delete temp;
    }
}

RegularTrie::~RegularTrie() {
    Node* current =
    while(root != nullptr){

    }
}

const Rule* RegularTrie::get_matching_rule(const PacketHeader& header) const {
    if(root == nullptr){
        return nullptr;
    }
    Node* current = root;
    const Rule* best_match = nullptr;
    std::string address = header.destination_address;
    for(char c : address){

        int best_match_priority = INT16_MAX;
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



