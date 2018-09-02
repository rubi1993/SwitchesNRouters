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

void RegularTrie::add_rule(const Rule& rule) {
    std::string prefix = use_source_address ? rule.source_address : rule.destination_address;
    Node* node = createPrefixNode(prefix);
    node->rules.push_back(&rule);
}

void RegularTrie::remove_rule(const Rule &rule) {
    std::string prefix = use_source_address ? rule.source_address : rule.destination_address;
    Node* node = createPrefixNode(prefix);
    node->rules.remove(&rule);
    while(node != nullptr && node->rules.size() == 0 && node->zero == nullptr && node->one == nullptr){
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
    int best_match_priority = 0;
    std::string address = use_source_address ? header.source_address : header.destination_address;
    for(char c : address){
        for(const Rule* r : current->rules){
            if((r->source_port_start == -1 || \
               (r->source_port_start <= header.source_port && r->source_port_end >= header.source_port)) \
               && (r->destination_port_start == -1 || \
                  (r->destination_port_start <= header.destination_port && r->destination_port_end >= header.destination_port)) \
               && (r->protocol == header.protocol || r->protocol == "*") && r->priority >= best_match_priority){
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

void RegularTrie::removeSubtreeLeaves(RegularTrie::Node *subroot, std::list<const Rule*>& rule_list) {
    if(subroot == nullptr){
        return;
    }
    if(subroot->zero == nullptr && subroot->one == nullptr){
        for(const Rule* rule : subroot->rules) {
            rule_list.push_back(rule);
        }
        subroot->rules.clear();
        Node* temp = subroot;
        subroot = subroot->prev;
        if(subroot != nullptr){
            if(subroot->zero == temp){
                subroot->zero = nullptr;
            }else{
                subroot->one = nullptr;
            }
        }else{
            root = nullptr;
        }
        delete temp;
    }else{
        removeSubtreeLeaves(subroot->zero, rule_list);
        removeSubtreeLeaves(subroot->one, rule_list);
    }
}

std::list<const Rule*> RegularTrie::remove_leaves() {
    std::list<const Rule*> rule_list;
    removeSubtreeLeaves(root, rule_list);
    return rule_list;
}


TrieOfTries::Node * TrieOfTries::createPrefixNode(std::string prefix){
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

void TrieOfTries::add_rule(const Rule& rule) {
    std::string prefix = rule.source_address;
    Node* node = createPrefixNode(prefix);
    node->trie->add_rule(rule);
}

void TrieOfTries::remove_rule(const Rule &rule) {
    std::string prefix = rule.source_address;
    Node* node = createPrefixNode(prefix);
    node->trie->remove_rule(rule);
    while(node != nullptr && node->trie->is_empty() && node->zero == nullptr && node->one == nullptr){
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

const Rule* TrieOfTries::get_matching_rule(const PacketHeader& header) const {
    if(root == nullptr){
        return nullptr;
    }
    Node* current = root;
    const Rule* best_match = nullptr;
    int best_match_priority = 0;
    std::string address = header.source_address;
    for(char c : address){
        const Rule* match = current->trie->get_matching_rule(header);
        if(match != nullptr && best_match_priority <= match->priority){
            best_match = match;
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

void TrieOfTries::destroySubtree(TrieOfTries::Node* subroot){
    if(subroot != nullptr){
        destroySubtree(subroot->zero);
        destroySubtree(subroot->one);
        delete subroot;
    }
}
TrieOfTries::~TrieOfTries() {
    destroySubtree(root);
}

const Rule* EpsilonT::get_matching_rule(const PacketHeader& header) const {
    if(root == nullptr){
        return nullptr;
    }
    Node* current = root;
    const Rule* best_match = nullptr;
    std::string address = header.destination_address;
    for(char c : address){
        int best_match_priority = 0;
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
        if(current->rule!= nullptr){
            if((current->rule->source_port_start == -1 || \
               (current->rule->source_port_start <= header.source_port && current->rule->source_port_end >= header.source_port)) \
               && (current->rule->destination_port_start == -1 || \
                  (current->rule->destination_port_start <= header.destination_port && current->rule->destination_port_end >= header.destination_port)) \
               && (current->rule->protocol == header.protocol || current->rule->protocol == "*") && current->rule->priority >= best_match_priority){
                best_match = current->rule;
                best_match_priority = current->rule->priority;
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

void EpsilonT::destroySubtree(EpsilonT::Node* subroot){
    if(subroot != nullptr){
        destroySubtree(subroot->zero);
        destroySubtree(subroot->one);
        destroySubtree(subroot->mid);
        delete subroot;
    }
}

EpsilonT::~EpsilonT() {
    destroySubtree(root);
}

void EpsilonT::add_rule(const Rule& rule) {
    std::string prefix = rule.destination_address;
    Node* node = createPrefixNode(prefix);
    while(node->mid!= nullptr){
        node=node->mid;
    }
    if(node->rule != nullptr){
        node->mid=new Node(&rule);
        node=node->mid;
    }
    node->rule = &rule;
}

void EpsilonT::remove_rule(const Rule &rule) {
    std::string prefix = rule.destination_address;
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

void TreeTrieEpsilonCluster::destroySubtree(TreeTrieEpsilonCluster::Node* subroot){
    if(subroot != nullptr){
        destroySubtree(subroot->left);
        destroySubtree(subroot->right);
        delete subroot;
    }
}

TreeTrieEpsilonCluster::~TreeTrieEpsilonCluster() {
    destroySubtree(root);
}

void TreeTrieEpsilonCluster::add_rule(const Rule& rule) {
    std::string prefix = rule.source_address;
    if(root == nullptr){
        if(prefix[prefix.length() - 1] == '*'){
            prefix.erase(prefix.length() - 1);
        }
        root = new Node();
        root->trie->add_rule(rule);
        root->prefix = prefix;
        return;
    }
    if(prefix[prefix.length() - 1] == '*'){
        prefix.erase(prefix.length() - 1);
    }
    while(prefix.length() < root->prefix.length()){
        prefix += "0";
    }
    Node* current = root;
    while(current != nullptr){
        if(prefix < current->prefix){
            if(current->left == nullptr){
                current->left = new Node(current);
                current->left->trie->add_rule(rule);
                current->left->prefix = prefix;
                return;
            }
            current = current->left;
        }else{
            if(current->right == nullptr){
                current->right = new Node(current);
                current->right->trie->add_rule(rule);
                current->right->prefix = prefix;
                return;
            }
            current = current->right;
        }
    }
}

void TreeTrieEpsilonCluster::remove_rule(const Rule& rule) {
    if(root == nullptr){
        return;
    }
    std::string prefix = rule.source_address;
    if(prefix[prefix.length() - 1] == '*'){
        prefix.erase(prefix.length() - 1);
    }
    while(prefix.length() < root->prefix.length()){
        prefix += "0";
    }
    Node* current = root;
    while(current!= nullptr && current->prefix != prefix){
        if(prefix < current->prefix){
            current = current->left;
        }else{
            current = current->right;
        }
    }
    if(current != nullptr){
        current->trie->remove_rule(rule);
        while(current != nullptr && current->trie->is_empty() && current->left == nullptr && current->right == nullptr){
            Node* temp = current;
            current = current->prev;
            if(current != nullptr){
                if(current->left == temp){
                    current->left = nullptr;
                }else{
                    current->right = nullptr;
                }
            }
            delete temp;
        }
    }
}

const Rule* TreeTrieEpsilonCluster::get_matching_rule(const PacketHeader &header) const {
    if(root == nullptr){
        return nullptr;
    }
    std::string prefix = header.source_address;
    Node* current = root;
    while(current!= nullptr && current->prefix != prefix.substr(0, current->prefix.length())){
        if(prefix < current->prefix){
            current = current->left;
        }else{
            current = current->right;
        }
    }
    if(current != nullptr){
        return current->trie->get_matching_rule(header);
    }
    return nullptr;
}

TreeTrieEpsilon::TreeTrieEpsilon(std::list<const Rule *> rule_table) {
    RegularTrie* trie = new RegularTrie(true);
    for(const Rule* rule : rule_table){
        trie->add_rule(rule);
    }
    while(!trie->is_empty()){
        std::list<const Rule*> rule_list = trie->remove_leaves();
        TreeTrieEpsilonCluster* cluster = new TreeTrieEpsilonCluster();
        for(const Rule* r : rule_list){
            cluster->add_rule(r);
        }
        clusters.push_back(cluster);
    }
}

TreeTrieEpsilon::~TreeTrieEpsilon() {
    for(TreeTrieEpsilonCluster* cluster : clusters){
        delete cluster;
    }
}

const Rule* TreeTrieEpsilon::get_matching_rule(const PacketHeader &header) const {
    const Rule* best_match = nullptr;
    int best_match_priority = 0;
    for(TreeTrieEpsilonCluster* cluster : clusters){
        const Rule* match = cluster->get_matching_rule(header);
        if(match != nullptr && match->priority >= best_match_priority){
            best_match = match;
            best_match_priority = match->priority;
        }
    }
    return best_match;
}

void TreeTrieEpsilon::add_rule(const Rule &rule) {
    //placeholder
}

void TreeTrieEpsilon::remove_rule(const Rule &rule) {
    //placeholder
}

//
//static int HiCuts::spmf(int n) {
//    return n*SPFAC;
//}
//
//int HiCuts::Node::num_of_cuts_needed(){
//    int n=num_of_rules();
//    int nump=std::max(4,std::sqrt(n));
//    int smC=0;
//    while(smC<spmf(n))
//    {
//
//        std::list<Rule>::iterator ptr;
//        for(  ptr = rules.begin() ; ptr != rules.end() ; ptr++)
//        {
//            smC+=ptr.operator*().num_of_partitions_colliding(partition_size)
//
//        }
//    }
//}
