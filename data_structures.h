//
// Created by igor.a on 8/30/18.
//

#ifndef SRA_DATA_STRUCTURES_H
#define SRA_DATA_STRUCTURES_H

#include "rules.h"
#include <string>
#include <list>
#include <map>
#include <utility>

#define SPFAC 4
int int_counter(std::string str);

class PacketClassifier{
    virtual std::pair<const Rule*, int>  get_matching_rule(const PacketHeader& header) const = 0;
    virtual void add_rule(const Rule& rule) = 0;
    virtual void remove_rule(const Rule& rule) = 0;
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
    bool use_source_address;
public:
    RegularTrie() : root(nullptr), use_source_address(false) {}
    RegularTrie(bool flag) : root(nullptr), use_source_address(flag) {};
    ~RegularTrie();
    bool is_empty() {return root == nullptr;}
    std::pair<const Rule*, int> get_matching_rule(const PacketHeader& header) const override;
    void add_rule(const Rule& rule) override;
    void remove_rule(const Rule& rule) override;
    std::list<const Rule*> remove_leaves();
private:
    Node* createPrefixNode(std::string prefix);
    void destroySubtree(RegularTrie::Node* subroot);
    void removeSubtreeLeaves(RegularTrie::Node* subroot, std::list<const Rule*>& rule_list);

};

class TrieOfTries : PacketClassifier{
    class Node {
    public:
        Node* zero;
        Node* one;
        Node* prev;
        RegularTrie* trie;

        Node() : zero(nullptr), one(nullptr), prev(nullptr), trie(nullptr) {}
        Node(Node* pre) : zero(nullptr), one(nullptr), prev(pre), trie(nullptr) {}
        ~Node() {if(trie!= nullptr) {delete trie;}}
    };
    Node* root;

public:
    TrieOfTries() : root(nullptr) {}
    TrieOfTries(std::list<const Rule*> rule_table);
    ~TrieOfTries();
    std::pair<const Rule*, int> get_matching_rule(const PacketHeader& header) const override;
    void add_rule(const Rule& rule) override;
    void remove_rule(const Rule& rule) override;
private:
    Node* createPrefixNode(std::string prefix);
    void destroySubtree(TrieOfTries::Node* subroot);
};

class EpsilonT : PacketClassifier{
public:
    class Node {
    public:
        Node* prev;
        Node* mid;
        Node* one;
        Node* zero;
        int id;
        int size;
        int sv;
        std::string bs;
        std::list<const Rule*> rule_list;
//        Node(const Rule* rule) : rule(rule) ,mid(nullptr),zero(nullptr), one(nullptr), prev(nullptr),
//                                 size(0),sv(0),bs(0) {id=id_counter;id_counter+=1;}
        Node(): rule_list(), size(0),sv(0),bs() ,mid(nullptr),zero(nullptr), one(nullptr), prev(nullptr) {id=id_counter;id_counter+=1;}
        Node(Node* pre) : zero(nullptr),mid(nullptr), one(nullptr), prev(pre), rule_list(), size(0),sv
                (0),bs(){id=id_counter;id_counter+=1;}
    };

    Node* root;
    static int id_counter;
    int p_trie;
public:
    EpsilonT(int p_t = 1) : root(nullptr), p_trie(p_t) {}
    ~EpsilonT();
    bool is_empty() {return root == nullptr;}
    void destroySubtree(Node* subroot);
    std::pair<const Rule*, int> get_matching_rule(const PacketHeader& header) const override;
    void add_rule(const Rule& rule) override;
    Node* createPrefixNode(std::string prefix);
    void remove_rule(const Rule& rule) override;
    void path_compress();

private:
    void DFSUtil(Node * node,std::map<int,bool>);
    Node* getPrefixNode(std::string prefix) const;
};

class TreeTrieEpsilonCluster : PacketClassifier{
    class Node {
    public:
        Node* left;
        Node* right;
        Node* prev;
        std::string prefix;
        int p_trie;
        EpsilonT* trie;
        Node(int p_t = 1) : left(nullptr), right(nullptr), prev(nullptr), p_trie(p_t), trie(new EpsilonT(p_t)) {}
        Node(Node* pre) : left(nullptr), right(nullptr), prev(pre), trie(new EpsilonT(prev->p_trie)) {}
        ~Node() {delete trie;}
    };
    Node* root;
    int p_trie;

public:
    TreeTrieEpsilonCluster(int p_t = 1) : root(nullptr), p_trie(p_t) {}
    ~TreeTrieEpsilonCluster();
    std::pair<const Rule*, int> get_matching_rule(const PacketHeader& header) const override;
    void add_rule(const Rule& rule) override;
    void remove_rule(const Rule& rule) override;
    void compress_all_paths();
private:
    void destroySubtree(TreeTrieEpsilonCluster::Node* subroot);
    void compressSubtreePaths(TreeTrieEpsilonCluster::Node* subroot);
};

class TreeTrieEpsilon : PacketClassifier{
    std::list<TreeTrieEpsilonCluster*> clusters;

public:
    TreeTrieEpsilon() : clusters() {}
    TreeTrieEpsilon(std::list<const Rule*> rule_table, int p_t = 1);
    ~TreeTrieEpsilon();
    std::pair<const Rule*, int> get_matching_rule(const PacketHeader& header) const override;
    void add_rule(const Rule& rule) override;
    void remove_rule(const Rule& rule) override;
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
    std::pair<const Rule*, int> get_matching_rule(const PacketHeader& header) const override;
    void add_rule(const Rule& rule) override;
    void remove_rule(const Rule& rule) override;
private:
    Node* getPrefixNode(std::string prefix) const;
};

class TSS{
    std::map<std::tuple<int,int>,std::list<const Rule*>> adress_map;
public:
    TSS() : adress_map() {}
    ~TSS(){};
    const Rule* get_matching_rule(const PacketHeader& header);
    void add_rule(const Rule& rule);
    void remove_rule(const Rule& rule);
};



#endif //SRA_DATA_STRUCTURES_H

