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
        Node* prev;
        std::list<const Rule*> rules;

        Node() : zero(nullptr), one(nullptr), prev(nullptr), rules() {}
        Node(Node* pre) : zero(nullptr), one(nullptr), prev(pre), rules() {}
    };

    Node* root;
public:
    RegularTrie() : root(nullptr) {}
    ~RegularTrie();
    const Rule* get_matching_rule(const PacketHeader& header) const;
    void add_rule(std::string prefix, const Rule& rule);
    void remove_rule(std::string prefix, const Rule& rule);
private:
    Node* createPrefixNode(std::string prefix);

};


#endif //SRA_DATA_STRUCTURES_H
