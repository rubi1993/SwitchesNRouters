//
// Created by igor.a on 8/30/18.
//

#ifndef SRA_DATA_STRUCTURES_H
#define SRA_DATA_STRUCTURES_H

#include "rules.h"
#include <string>
#include <list>

#define SPFAC 4

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


class EpsilonT{
    class Node {
        Node* right;
        Node* left;
        int size;
        int sv;
        int bs;
        std::string rule_name;

        Node() : zero(nullptr), one(nullptr), rule_name("") {}
        Node(std::string rule) : rule_name(rule) {}
    };

    Node* root;

    EpsilonT() : root(nullptr) {}
    std::string get_matching_rule(std::string address) const;
    void add_rule(std::string prefix, std::string rule);
    bool remove_rule(std::string prefix, std::string rule);
private:
    Node* getPrefixNode(std::string prefix) const;
};


class HiCuts{
    static int spmf(int n);
    class Node {
        std::list<int> subspace; // represents the geometric subspace stored at v.
        int cuts;//the number of partitions of the subspace
        std::list<Rule> rules; // the rules that are contained or cuts the subspace.
        Node(std::list<Rule> tempRules) : subspace(), cuts(0), rules(tempRules) {}
        int num_of_rules();
        int num_of_cuts_needed();
    };
    Node* root;
    HiCuts() : root(nullptr) {}
    std::string get_matching_rule(std::string address) const;
    void add_rule(std::string prefix, std::string rule);
    bool remove_rule(std::string prefix, std::string rule);
private:
    Node* getPrefixNode(std::string prefix) const;
};



#endif //SRA_DATA_STRUCTURES_H
