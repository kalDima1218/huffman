#include <unordered_map>
#include <queue>
#include "bytes.h"

namespace huffman_tree {
    class huffman_tree_node {
        friend class huffman_tree_node_comparator;
        friend class huffman_tree;
    private:
        char _c;
        int _cnt = 0;
        huffman_tree_node *_l = nullptr, *_r = nullptr;
        bool _terminate = false;
    public:
        huffman_tree_node() {}

        huffman_tree_node(char c, int cnt, bool terminate = true) {
            _c = c;
            _cnt = cnt;
            _terminate = terminate;
        }

        huffman_tree_node(char c, int cnt, huffman_tree_node *l, huffman_tree_node *r) {
            _c = c;
            _cnt = cnt;
            _l = l;
            _r = r;
        }

        char get_c() const {
            return _c;
        }

        bool operator<(const huffman_tree_node &n) const {
            return _cnt > n._cnt;
        }
    };


    struct huffman_tree_node_comparator {
        bool operator()(huffman_tree_node const *l, huffman_tree_node const *r) const {
            return l->_cnt > r->_cnt;
        }
    };


    class huffman_tree {
    private:
        huffman_tree_node *_root = nullptr;

        huffman_tree_node *_merge(huffman_tree_node *a, huffman_tree_node *b) {
            return new huffman_tree_node(0, a->_cnt + b->_cnt, a, b);
        }

        void
        _get_keys(std::unordered_map<char, bytes::bytes_seq> &keys, bytes::bytes_seq &key, huffman_tree_node *node) {
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

        void _destroy_huffman_tree(huffman_tree_node *node) {
            if (node->_l != nullptr) {
                _destroy_huffman_tree(node->_l);
            }
            if (node->_r != nullptr) {
                _destroy_huffman_tree(node->_r);
            }
            delete[] node;
        }

    public:
        huffman_tree() {}

        huffman_tree(int freq[256]) {
            std::priority_queue<huffman_tree_node *, std::vector<huffman_tree_node *>, huffman_tree_node_comparator> sorted_freq;
            for (int i = 0; i < 256; ++i) {
                if (freq[i] > 0) sorted_freq.push(new huffman_tree_node(char(i), freq[i]));
            }
            if (sorted_freq.size() == 1) {
                huffman_tree_node *l = sorted_freq.top();
                sorted_freq.pop();
                huffman_tree_node *r = new huffman_tree_node(char(255 - unsigned(l->get_c())), 0, false);
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

        ~huffman_tree() {
            _destroy_huffman_tree(_root);
        }

        std::unordered_map<char, bytes::bytes_seq> get_keys() {
            bytes::bytes_seq tmp_key;
            std::unordered_map<char, bytes::bytes_seq> keys;
            _get_keys(keys, tmp_key, _root);
            return keys;
        }
    };
}