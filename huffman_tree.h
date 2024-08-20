#include <unordered_map>
#include "bytes.h"

class huffman_tree_node {
    friend class huffman_tree;
private:
    char _c = 0;
    int _cnt = 0;
    huffman_tree_node *_l = nullptr, *_r = nullptr;
    bool _terminate = false;
public:
    huffman_tree_node() = default;
    huffman_tree_node(char c, int cnt, bool terminate = true) : _c(c), _cnt(cnt), _terminate(terminate) {}
    huffman_tree_node(char c, int cnt, huffman_tree_node *l, huffman_tree_node *r) : _c(c), _cnt(cnt), _l(l), _r(r) {}
    char get_char() const;
    bool is_terminate() const;
    huffman_tree_node* go(bool b);
    bool operator<(const huffman_tree_node &n) const;
};

class huffman_tree {
    struct huffman_tree_node_comparator {
        bool operator()(huffman_tree_node const *l, huffman_tree_node const *r) const;
    };
private:
    huffman_tree_node *_root = new huffman_tree_node;
    static huffman_tree_node *_merge(huffman_tree_node *a, huffman_tree_node *b);
    static void _get_keys(std::unordered_map<char, bytes_seq> &keys, bytes_seq &key, huffman_tree_node *node);
    void _destroy_huffman_tree(huffman_tree_node *node);
public:
    huffman_tree() = default;
    huffman_tree(int freq[256]);
    ~huffman_tree();
    huffman_tree_node* get_root() const;
    void add_key(const bytes_seq &key, char c);
    char get_key(const bytes_seq &key) const;
    std::unordered_map<char, bytes_seq> get_keys();
};