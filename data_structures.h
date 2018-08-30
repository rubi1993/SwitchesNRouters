//
// Created by igor.a on 8/30/18.
//

#ifndef SRA_DATA_STRUCTURES_H
#define SRA_DATA_STRUCTURES_H

#include "rules.h"
#include <string>
#include <list>

class RegularTrie{
    class Node {
    public:
        Node* zero;
        Node* one;
        std::list<Rule> rules;

        Node() : zero(nullptr), one(nullptr), rules() {}
    };

    Node* root;
public:
    RegularTrie() : root(nullptr) {}
    Rule* get_matching_rule(const PacketHeader& header) const;
    void add_rule(std::string prefix, const Rule& rule);
    bool remove_rule(std::string prefix, const Rule& rule);
private:
    Node* createPrefixNode(std::string prefix);
    Node* getPrefixNode(std::string address) const;

};


#endif //SRA_DATA_STRUCTURES_H
