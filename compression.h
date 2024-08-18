#include <string>
#include <fstream>
#include "huffman_tree.h"

namespace compression {
    void encode(const std::string& source) {
        std::ifstream fin;
        std::ofstream fout;
        int freq[256] = {};
        char chr;
        fin.open(source);
        while (fin.get(chr)) {
            freq[static_cast<unsigned char>(chr)]++;
        }
        fin.close();

        std::unordered_map<char, bytes::bytes_seq> keys;

        huffman_tree::huffman_tree h_tree(freq);
        keys = h_tree.get_keys();
//    for(auto i:keys){
//        std::cout << int(i.first) << " ";
//        for(int j = 0; j < i.second.size_bit(); ++j){
//            std::cout << i.second.get_bit(j);
//        }
//        std::cout << "\n";
//    }

        fout.open(source + ".wtf");
        int keys_cnt = keys.size();
        for (int i = 0; i < 4; ++i) {
            fout.put(char(keys_cnt % 256));
            keys_cnt >>= 8;
        }
        // пишутся ключи
        for (auto &i: keys) {
            fout.put(char(1));
            fout.put(i.first);
            fout.put(char(i.second.size_bit()));
            for (int j = 0; j < i.second.size_byte(); ++j) {
                fout.put(char(i.second.get_byte(j)));
            }
        }

        // пишутся данные
        fin.open(source);
        bytes::bytes_seq cipher;
        int data_len = 0;

        while (fin.get(chr)) {
            cipher += keys[chr];
            data_len++;
        }

        for (int i = 0; i < 4; ++i) {
            fout.put(char(data_len % 256));
            data_len >>= 8;
        }

        for (int i = 0; i < cipher.size_byte(); ++i) {
            fout.put(char(cipher.get_byte(i)));
        }
        fin.close();
        fout.close();
    }

    void decode(const std::string& source) {
        std::unordered_map<bytes::bytes_seq, char> keys;
        std::ifstream fin(source);
        std::ofstream fout("decoded_" + source.substr(0, source.size() - 4));
        char chr;

        // читаются ключи
        int keys_cnt = 0;
        for (int i = 0; i < 4; ++i) {
            fin.get(chr);
            keys_cnt |= static_cast<unsigned char>(chr) * (1 << (8 * i));
        }

        for (int i = 0; i < keys_cnt; ++i) {
            bytes::bytes_seq key;
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

            keys[key] = c;
        }
//    for(auto i:keys){
//        std::cout << int(i.second) << " ";
//        for(int j = 0; j < i.first.size_bit(); ++j){
//            std::cout << i.first.get_bit(j);
//        }
//        std::cout << "\n";
//    }

        // читаются данные
        bytes::bytes_seq tmp;
        int data_len = 0;

        for (int i = 0; i < 4; ++i) {
            fin.get(chr);
            data_len |= static_cast<unsigned char>(chr) * (1 << (8 * i));
        }

        while (fin.get(chr) && data_len > 0) {
            for (int i = 0; i < 8 && data_len > 0; ++i) {
                tmp.push_bit(chr & 1);
                chr >>= 1;
                if (keys.count(tmp)) {
                    fout.put(keys[tmp]);
                    tmp = bytes::bytes_seq();
                    data_len--;
                }
            }
        }
    }
}