#include <fstream>
#include "huffman_tree.h"

void encode(const std::string& source) {
    std::ifstream fin;
    char chr;
    int freq[256] = {};
    int data_len = 0;
    fin.open(source);
    while (fin.get(chr)) {
        freq[static_cast<unsigned char>(chr)]++;
        data_len++;
    }
    fin.close();
    std::unordered_map<char, bytes_seq> keys;
    huffman_tree h_tree(freq);
    keys = h_tree.get_keys();
    bytes_seg_ofstream fout(source + ".wtf");
    int keys_cnt = keys.size();
    for (int i = 0; i < 4; ++i) {
        fout.put(char(keys_cnt % 256));
        keys_cnt >>= 8;
    }
    for (auto &i: keys) {
        fout.put(char(1));
        fout.put(i.first);
        fout.put(char(i.second.size_bit()));
        fout.put_bytes(i.second);
        fout.flush_buff();
    }
    fin.open(source);
    for (int i = 0; i < 4; ++i) {
        fout.put(char(data_len % 256));
        data_len >>= 8;
    }
    while (fin.get(chr)) {
        fout.put_bytes(keys[chr]);
    }
    fin.close();
    fout.close_byte_stream();
}

void decode(const std::string& source) {
    std::ifstream fin(source);
    char chr;
    int keys_cnt = 0;
    for (int i = 0; i < 4; ++i) {
        fin.get(chr);
        keys_cnt |= static_cast<unsigned char>(chr) * (1 << (8 * i));
    }
    huffman_tree h_tree;
    for (int i = 0; i < keys_cnt; ++i) {
        bytes_seq key;
        unsigned char key_len = 0;
        char c;
        fin.get(chr);
        fin.get(chr);
        c = chr;
        fin.get(chr);
        key_len = chr;
        for (int j = 0; j < (key_len + 7) / 8; ++j) {
            fin.get(chr);
            key.push_byte(chr);
        }
        while (key.size_bit() > key_len) {
            key.pop_bit();
        }
        h_tree.add_key(key, c);
    }
    std::ofstream fout("decoded_" + source.substr(0, source.size() - 4));
    huffman_tree_node* v = h_tree.get_root();
    int data_len = 0;
    for (int i = 0; i < 4; ++i) {
        fin.get(chr);
        data_len |= static_cast<unsigned char>(chr) * (1 << (8 * i));
    }
    while (fin.get(chr) && data_len > 0) {
        for (int i = 0; i < 8 && data_len > 0; ++i) {
            v = v->go(chr & 1);
            chr >>= 1;
            if (v->is_terminate()) {
                fout.put(v->get_char());
                v = h_tree.get_root();
                data_len--;
            }
        }
    }
}