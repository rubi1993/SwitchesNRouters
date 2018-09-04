//
// Created by igor.a on 8/30/18.
//

#include "data_structures.h"
#include <iostream>
#include <cmath>
#include <pthread.h>
#include <mutex>

std::mutex mutex;


int EpsilonT::id_counter=0;
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

std::pair<const Rule*, int> RegularTrie::get_matching_rule(const PacketHeader& header) const {
    if(root == nullptr){
        return std::make_pair((const Rule*)nullptr, 0);
    }
    Node* current = root;
    const Rule* best_match = nullptr;
    int best_match_priority = 0;
    int nodes_seen = 0;
    std::string address = use_source_address ? header.source_address : header.destination_address;
    for(char c : address){
        nodes_seen++;
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
                return std::make_pair(best_match, nodes_seen);
            }
            current = current->zero;
        }else if(c == '1'){
            if(current->one == nullptr){
                return std::make_pair(best_match, nodes_seen);
            }
            current = current->one;
        }
    }
    return std::make_pair(best_match, nodes_seen);
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
    if(node->trie == nullptr){
        node->trie = new RegularTrie();
    }
    node->trie->add_rule(rule);
}

void TrieOfTries::remove_rule(const Rule &rule) {
    std::string prefix = rule.source_address;
    Node* node = createPrefixNode(prefix);
    if(node->trie == nullptr){
        node->trie = new RegularTrie();
    }
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

std::pair<const Rule*, int> TrieOfTries::get_matching_rule(const PacketHeader& header) const {
    if(root == nullptr){
        return std::make_pair((const Rule*)nullptr, 0);
    }
    Node* current = root;
    const Rule* best_match = nullptr;
    int best_match_priority = 0;
    int nodes_seen = 0;
    std::string address = header.source_address;
    for(char c : address){
        const Rule* match = nullptr;
        if(current->trie != nullptr){
            std::pair<const Rule*, int> match_pair = current->trie->get_matching_rule(header);
            match = match_pair.first;
            nodes_seen += match_pair.second;
        }
        if(match != nullptr && best_match_priority <= match->priority){
            best_match = match;
        }
        if(c == '0'){
            if(current->zero == nullptr){
                return std::make_pair(best_match, nodes_seen);
            }
            current = current->zero;
        }else if(c == '1'){
            if(current->one == nullptr){
                return std::make_pair(best_match, nodes_seen);
            }
            current = current->one;
        }
    }
    return std::make_pair(best_match, nodes_seen);
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

TrieOfTries::TrieOfTries(std::list<const Rule *> rule_table) {
    root = nullptr;
    for(const Rule* rule : rule_table){
        std::string prefix = rule->source_address;
        Node* node = createPrefixNode(prefix);
        if(node->trie == nullptr){
            node->trie = new RegularTrie();
        }
        node->trie->add_rule(*rule);
    }
}

std::pair<const Rule*, int> EpsilonT::get_matching_rule(const PacketHeader& header) const {
    if(root == nullptr){
        return std::make_pair((const Rule*)nullptr, 0);
    }
    Node* current = root;
    const Rule* best_match = nullptr;
    std::string address = header.destination_address;
    int best_match_priority = 0;
    int nodes_seen = 0;
    int bit_string_position = 0;
    for(char c : address){
        nodes_seen++;
        bool epsilon_nodes = false;
        if(current->bs.length() == 0){
            bit_string_position = 0;
        }else{
            if(current->bs[bit_string_position] == c){
                bit_string_position++;
                continue;
            }else{
                return std::make_pair(best_match, nodes_seen);
            }
        }
        while(current->mid!= nullptr)
        {
            epsilon_nodes = true;
            nodes_seen++;
            if((current->rule->source_port_start == -1 || \
               (current->rule->source_port_start <= header.source_port && current->rule->source_port_end >= header.source_port)) \
               && (current->rule->destination_port_start == -1 || \
                  (current->rule->destination_port_start <= header.destination_port && current->rule->destination_port_end >= header.destination_port)) \
               && (current->rule->protocol == header.protocol || current->rule->protocol == "*") && current->rule->priority >= best_match_priority){
                best_match = current->rule;
                best_match_priority = current->rule->priority;
            }
            current=current->mid;
        }
        if(epsilon_nodes){
            nodes_seen--;
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
                return std::make_pair(best_match, nodes_seen);
            }
            current = current->zero;
        }else if(c == '1'){
            if(current->one == nullptr){
                return std::make_pair(best_match, nodes_seen);
            }
            current = current->one;
        }
    }
    return std::make_pair(best_match, nodes_seen);
}
void EpsilonT::DFSUtil(Node * node,std::map<int,bool> visited){
    bool is_deleted= false;
    bool one_child=false;
    bool have_child=false;
    bool is_root=false;
    Node* cur_node= nullptr;

    if (root== nullptr){
        return;
    }
    if(root->id==node->id){
        is_root=true;
        cur_node=root;

    }else{
        cur_node=node;
    }
    if(cur_node->one== nullptr and cur_node->zero!= nullptr or cur_node->one!= nullptr and cur_node->zero== nullptr){
        one_child=true;
    }
    if(cur_node->rule== nullptr and one_child){
        if(is_root){
            if(cur_node->zero!= nullptr){
                cur_node->zero->bs=root->bs+cur_node->zero->bs;
                cur_node->zero->sv+=root->sv;
                root=cur_node->zero;
                root->prev= nullptr;
                root->sv+=1;
                root->bs+='0';
            }
            else if(cur_node->one!= nullptr){
                cur_node->one->bs=root->bs+cur_node->one->bs;
                cur_node->one->sv+=root->sv;
                root=cur_node->one;
                root->prev= nullptr;
                root->sv+=1;
                root->bs+='1';
            }
            is_root=false;
            is_deleted=true;
        }
        else{
            Node * prev_node=cur_node->prev;
            if(cur_node->zero!= nullptr){
                if(prev_node->one!= nullptr and prev_node->one->id==cur_node->id){

                    prev_node->one=cur_node->zero;
                    cur_node->zero->sv=cur_node->zero->sv+cur_node->sv+1;
                    cur_node->zero->bs=cur_node->bs+cur_node->zero->bs;
                    cur_node->zero->bs+='0';
                    cur_node->zero->prev=prev_node;
                }
                else if(prev_node->zero!= nullptr and prev_node->zero->id==cur_node->id){
                    prev_node->zero=cur_node->zero;
                    cur_node->zero->sv=cur_node->zero->sv+cur_node->sv+1;
                    cur_node->zero->bs=cur_node->bs+cur_node->zero->bs;
                    cur_node->zero->bs+='0';
                    cur_node->zero->prev=prev_node;

                }
            }
            else if(cur_node->one!= nullptr){
                if(prev_node->one!= nullptr and prev_node->one->id==cur_node->id){
                    prev_node->one=cur_node->one;
                    cur_node->one->sv=cur_node->one->sv+cur_node->sv+1;
                    cur_node->one->bs=cur_node->bs+cur_node->one->bs;
                    cur_node->one->bs+='1';
                    cur_node->one->prev=prev_node;

                }else if(prev_node->zero!= nullptr and prev_node->zero->id==cur_node->id){
                    prev_node->zero=cur_node->one;
                    cur_node->one->sv=cur_node->one->sv+cur_node->sv+1;
                    cur_node->one->bs=cur_node->bs+cur_node->one->bs;
                    cur_node->one->bs+='1';
                    cur_node->one->prev=prev_node;

                }
            }
            is_deleted=true;
        }
    }
    visited[node->id]=true;
    if(cur_node->mid!= nullptr and !visited.count(node->mid->id)){
        DFSUtil(cur_node->mid,visited);
    }
    if(cur_node->one!= nullptr and !visited.count(node->one->id)){
        DFSUtil(cur_node->one,visited);
    }
    if(cur_node->zero!= nullptr and !visited.count(node->zero->id)){
        DFSUtil(cur_node->zero,visited);
    }
    if(is_deleted){
        delete(cur_node);
    }
}
void EpsilonT::path_compress() {
    std::map<int,bool> visited;
    DFSUtil(root,visited);
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
        node->mid=new Node(node);
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
}



const Rule* TSS::get_matching_rule(const PacketHeader &header){
    int dest_num= int_counter(header.destination_address);
    int source_num=int_counter(header.source_address);
    int best_match_priority = INT32_MAX;
    const Rule* best_match= nullptr;
    std::tuple<int,int> tup(source_num,dest_num);
    if(adress_map[tup].size()==0)
    {
        return nullptr;
    }
    else{
        std::list<const Rule*>::iterator it;
        for(it = adress_map[tup].begin(); it != adress_map[tup].end(); ++it){
            const Rule* current=it.operator*();
            if((current->source_port_start == -1 || \
               (current->source_port_start <= header.source_port && current->source_port_end >= header.source_port)) \
               && (current->destination_port_start == -1 || \
                  (current->destination_port_start <= header.destination_port && current->destination_port_end >= header.destination_port)) \
               && (current->protocol == header.protocol || current->protocol == "*") && current->priority <= best_match_priority){
                best_match = current;
                best_match_priority = current->priority;
            }
        }
        }
    return best_match;
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
    while(current->prefix != prefix){
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
    current->trie->add_rule(rule);
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

std::pair<const Rule*, int> TreeTrieEpsilonCluster::get_matching_rule(const PacketHeader &header) const {
    if(root == nullptr){
        return std::make_pair((const Rule*)nullptr, 0);
    }
    std::string prefix = header.source_address;
    Node* current = root;
    int nodes_seen = 0;
    while(current!= nullptr && current->prefix != prefix.substr(0, current->prefix.length())){
        nodes_seen++;
        if(prefix < current->prefix){
            current = current->left;
        }else{
            current = current->right;
        }
    }
    if(current != nullptr){
        std::pair<const Rule*, int> match_pair = current->trie->get_matching_rule(header);
        match_pair.second += nodes_seen;
        return match_pair;
    }
    return std::make_pair((const Rule*)nullptr, nodes_seen);
}

void TreeTrieEpsilonCluster::compressSubtreePaths(TreeTrieEpsilonCluster::Node *subroot) {
    if(subroot == nullptr){
        return;
    }
    if(subroot->trie != nullptr){
        subroot->trie->path_compress();
    }
    compressSubtreePaths(subroot->left);
    compressSubtreePaths(subroot->right);
}

void TreeTrieEpsilonCluster::compress_all_paths() {
    compressSubtreePaths(root);
}


TreeTrieEpsilon::TreeTrieEpsilon(std::list<const Rule *> rule_table) {
    RegularTrie* trie = new RegularTrie(true);
    for(const Rule* rule : rule_table){
        trie->add_rule(*rule);
    }
    while(!trie->is_empty()){
        std::list<const Rule*> rule_list = trie->remove_leaves();
        TreeTrieEpsilonCluster* cluster = new TreeTrieEpsilonCluster();
        for(const Rule* r : rule_list){
            cluster->add_rule(*r);
        }
        cluster->compress_all_paths();
        clusters.push_back(cluster);
    }
    delete trie;
}

TreeTrieEpsilon::~TreeTrieEpsilon() {
    for(TreeTrieEpsilonCluster* cluster : clusters){
        delete cluster;
    }
}
struct arg_struct {
    int*  priority;
    TreeTrieEpsilonCluster* tree;
    const PacketHeader* packetHeader;
    const Rule** bestMatch;
};
void* get_matching_by_thread(void *arguments){
    std::cout << pthread_self() << std::endl;
    struct arg_struct *args=(struct arg_struct *)arguments;
    std::pair<const Rule*, int> pair=args->tree->get_matching_rule(*args->packetHeader);
    int best_match_priority=*(args->priority);
    const Rule* cur_rule=pair.first;
    if(cur_rule!= nullptr and cur_rule->priority>=best_match_priority){
        mutex.lock();
        *(args->bestMatch)=cur_rule;
        *(args->priority)=cur_rule->priority;
        mutex.unlock();
    }
    pthread_exit(NULL);
}


std::pair<const Rule*, int>  TreeTrieEpsilon::get_matching_rule(const PacketHeader &header) const {
    pthread_t threads[clusters.size()];
    int priority=0;
    int index=0;
    const Rule* best_match= nullptr;
    for(TreeTrieEpsilonCluster* cluster : clusters){ //todo: to add multithreading.
//        //create thread, send mission
//        const Rule* match = cluster->get_matching_rule(header);
//        if(match != nullptr && match->priority >= best_match_priority){
//            best_match = match;
//            best_match_priority = match->priority;
//        }
        struct arg_struct my_args;
        my_args.priority=&priority;
        my_args.bestMatch= &best_match;
        my_args.packetHeader=&header;
        my_args.tree=cluster;
        pthread_create(&threads[index],NULL,get_matching_by_thread,(void *)&my_args);
        index+=1;
    }
}

void TreeTrieEpsilon::add_rule(const Rule &rule) {
    //placeholder
}

void TreeTrieEpsilon::remove_rule(const Rule &rule) {
    //placeholder
}

//void TSS::add_rule(const Rule &rule) {
//    int dest_num= int_counter(rule.destination_address);
//    int source_num=int_counter(rule.source_address);
//    std::tuple<int,int> tup(source_num,dest_num);
//    adress_map[tup].push_back(&rule);
//}
//
//void TSS::remove_rule(const Rule &rule) {
//    int dest_num= int_counter(rule.destination_address);
//    int source_num=int_counter(rule.source_address);
//    std::tuple<int,int> tup(source_num,dest_num);
//    adress_map[tup].remove(&rule);
//}


int int_counter(const std::string str){
    int result=0;
    for (size_t i = 0; i < str.size(); i++)
    {
        char currentChar = str[i];
        if (currentChar !='*')
            result += 1;
    }
    return result;
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
