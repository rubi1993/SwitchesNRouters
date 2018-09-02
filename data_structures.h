//
// Created by igor.a on 8/30/18.
//

#ifndef SRA_DATA_STRUCTURES_H
#define SRA_DATA_STRUCTURES_H

#include "rules.h"
#include <string>
#include <list>

#define SPFAC 4

class PacketClassifier{
    virtual const Rule* get_matching_rule(const PacketHeader& header) const = 0;
    virtual void add_rule(std::string prefix, const Rule& rule) = 0;
    virtual void remove_rule(std::string prefix, const Rule& rule) = 0;
};

class RegularTrie : PacketClassifier{
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
    bool is_empty() {return root == nullptr;}
    const Rule* get_matching_rule(const PacketHeader& header) const override;
    void add_rule(std::string prefix, const Rule& rule) override;
    void remove_rule(std::string prefix, const Rule& rule) override;
private:
    Node* createPrefixNode(std::string prefix);
    void destroySubtree(RegularTrie::Node* subroot);

};

class TrieOfTries : PacketClassifier{
    class Node {
    public:
        Node* zero;
        Node* one;
        Node* prev;
        RegularTrie* trie;

        Node() : zero(nullptr), one(nullptr), prev(nullptr), trie(new RegularTrie) {}
        Node(Node* pre) : zero(nullptr), one(nullptr), prev(pre), trie(new RegularTrie) {}
    };
    Node* root;

public:
    const Rule* get_matching_rule(const PacketHeader& header) const override;
    void add_rule(std::string prefix, const Rule& rule) override;
    void remove_rule(std::string prefix, const Rule& rule) override;
private:
    Node* createPrefixNode(std::string prefix);
    void destroySubtree(TrieOfTries::Node* subroot);
};

class EpsilonT : PacketClassifier{
    class Node {
    public:
        Node* prev;
        Node* mid;
        Node* one;
        Node* zero;
        int size;
        int sv;
        int bs;
        const Rule* rule;
        Node(const Rule* rule) : rule(rule) ,mid(nullptr),zero(nullptr), one(nullptr), prev(nullptr),
        size(0),sv(0),bs(0) {}
        Node():rule(nullptr), size(0),sv(0),bs(0) ,mid(nullptr),zero(nullptr), one(nullptr), prev(nullptr) {}
        Node(Node* pre) : zero(nullptr),mid(nullptr), one(nullptr), prev(pre), rule(), size(0),sv
        (0),bs(0){}
    };

    Node* root;
public:
    EpsilonT() : root(nullptr) {}
    ~EpsilonT();
    void destroySubtree(Node* subroot);
    const Rule* get_matching_rule(const PacketHeader& header) const override;
    void add_rule(std::string prefix, const Rule& rule) override;
    Node* createPrefixNode(std::string prefix);
    void remove_rule(std::string prefix, const Rule& rule) override;
private:
    Node* getPrefixNode(std::string prefix) const;
};


class HiCuts : PacketClassifier{
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
    const Rule* get_matching_rule(const PacketHeader& header) const override;
    void add_rule(std::string prefix, const Rule& rule) override;
    void remove_rule(std::string prefix, const Rule& rule) override;
private:
    Node* getPrefixNode(std::string prefix) const;
};



#endif //SRA_DATA_STRUCTURES_H

