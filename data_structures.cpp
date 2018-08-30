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
                current->zero = new Node();
            }
            current = current->zero;
        }else if(c == '1'){
            if(current->one == nullptr){
                current->one = new Node();
            }
            current = current->one;
        }
    }
    return current;
}

void RegularTrie::add_rule(std::string prefix, const Rule& rule) {
    Node* node = createPrefixNode(prefix);
    node->rules.push_back(rule);
}

RegularTrie::Node * RegularTrie::getPrefixNode(std::string address) const {
    if(root == nullptr){
        return nullptr;
    }
    Node* current = root;
    std::string best_match = nullptr;
    for(char c : address){
        for(Rule r : current->rules){
            if
        }
        if(c == '0'){
            if(current->zero == nullptr){
                current->zero = new Node();
            }
            current = current->zero;
        }else if(c == '1'){
            if(current->one == nullptr){
                current->one = new Node();
            }
            current = current->one;
        }
    }
    return current;
}

Rule* RegularTrie::get_matching_rule(const PacketHeader& header) const {

}



