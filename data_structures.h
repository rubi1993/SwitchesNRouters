//
// Created by igor.a on 8/30/18.
//

#ifndef SRA_DATA_STRUCTURES_H
#define SRA_DATA_STRUCTURES_H

#include "rules.h"
#include "string.h"

class RegularTrie{
    class Node {
        Node* zero;
        Node* one;
        std::string rule_name;

        Node() : zero(nullptr), one(nullptr), rule_name("") {}
        Node(std::string rule) : rule_name(rule) {}
    };

    Node* root;

    RegularTrie() : root(nullptr) {}
    std::string get_matching_rule(std::string address) const;
    void add_rule(std::string prefix, std::string rule);
    bool remove_rule(std::string prefix, std::string rule);
private:
    Node* getPrefixNode(std::string prefix) const;

};


#endif //SRA_DATA_STRUCTURES_H
