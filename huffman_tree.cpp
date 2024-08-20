#include <unordered_map>
#include <queue>
#include "huffman_tree.h"

char huffman_tree_node::get_char() const {
    return _c;
}

bool huffman_tree_node::is_terminate() const {
    return _terminate;
}

huffman_tree_node* huffman_tree_node::go(bool b){
    if(b == 0){
        if(_l == nullptr) throw "huffman_tree_node::go: _l nullptr";
        return _l;
    }
    else{
        if(_l == nullptr) throw "huffman_tree_node::go: _r nullptr";
        return _r;
    }
}

bool huffman_tree_node::operator<(const huffman_tree_node &n) const {
    return _cnt > n._cnt;
}

bool huffman_tree::huffman_tree_node_comparator::operator()(const huffman_tree_node *l, const huffman_tree_node *r) const {
    return l->_cnt > r->_cnt;
}

huffman_tree_node* huffman_tree::_merge(huffman_tree_node *a, huffman_tree_node *b) {
    return new huffman_tree_node(0, a->_cnt + b->_cnt, a, b);
}

void huffman_tree::_get_keys(std::unordered_map<char, bytes_seq> &keys, bytes_seq &key, huffman_tree_node *node) {
    if (node->_terminate) {
        keys[node->_c] = key;
    }
    if (node->_l != nullptr) {
        key.push_bit(0);
        _get_keys(keys, key, node->_l);
        key.pop_bit();
    }
    if (node->_r != nullptr) {
        key.push_bit(1);
        _get_keys(keys, key, node->_r);
        key.pop_bit();
    }
}

void huffman_tree::_destroy_huffman_tree(huffman_tree_node *node) {
    if (node->_l != nullptr) {
        _destroy_huffman_tree(node->_l);
    }
    if (node->_r != nullptr) {
        _destroy_huffman_tree(node->_r);
    }
    delete[] node;
}

huffman_tree::huffman_tree(int freq[256]) {
    std::priority_queue<huffman_tree_node *, std::vector<huffman_tree_node *>, huffman_tree_node_comparator> sorted_freq;
    for (int i = 0; i < 256; ++i) {
        if (freq[i] > 0) sorted_freq.push(new huffman_tree_node(char(i), freq[i]));
    }
    if (sorted_freq.size() == 1) {
        huffman_tree_node *l = sorted_freq.top();
        sorted_freq.pop();
        huffman_tree_node *r = new huffman_tree_node(char(255 - unsigned(l->get_char())), 0, false);
        sorted_freq.push(_merge(l, r));
    }
    while (sorted_freq.size() > 1) {
        huffman_tree_node *l = sorted_freq.top();
        sorted_freq.pop();
        huffman_tree_node *r = sorted_freq.top();
        sorted_freq.pop();
        sorted_freq.push(_merge(l, r));
    }
    _root = sorted_freq.top();
}

huffman_tree::~huffman_tree() {
    _destroy_huffman_tree(_root);
}

huffman_tree_node* huffman_tree::get_root() const {
    return _root;
}

void huffman_tree::add_key(const bytes_seq &key, char c){
    huffman_tree_node* v = _root;
    for(int i = 0; i < key.size_bit(); ++i){
        if(key.get_bit(i) == 0){
            if(v->_l == nullptr){
                v->_l = new huffman_tree_node;
            }
            v = v->_l;
        }
        else{
            if(v->_r == nullptr){
                v->_r = new huffman_tree_node;
            }
            v = v->_r;
        }
    }
    v->_c = c;
    v->_terminate = true;
}

char huffman_tree::get_key(const bytes_seq &key) const{
    huffman_tree_node* v = _root;
    for(int i = 0; i < key.size_bit(); ++i){
        if(key.get_bit(i) == 0){
            v = v->_l;
        }
        else{
            v = v->_r;
        }
    }
    return v->_c;
}

std::unordered_map<char, bytes_seq> huffman_tree::get_keys() {
    bytes_seq tmp_key;
    std::unordered_map<char, bytes_seq> keys;
    _get_keys(keys, tmp_key, _root);
    return keys;
}
