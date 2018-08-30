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


int HiCuts::Node::num_of_rules() {
    return rules.size();
}

static int HiCuts::spmf(int n) {
    return n*SPFAC;
}

int HiCuts::Node::num_of_cuts_needed(){
    int n=num_of_rules();
    int nump=std::max(4,std::sqrt(n));
    int smC=0;
    while(smC<spmf(n)){
        for (int i=0 ; i< n ;i++){
            smC+=0;
        }
    }
}