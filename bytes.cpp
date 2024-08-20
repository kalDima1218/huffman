#include "bytes.h"

std::size_t bytes_seq::size_byte() const {
    return data.size();
}

std::size_t bytes_seq::size_bit() const {
    return (data.size() - 1) * 8 + last_bit_i + 1;
}

void bytes_seq::push_bit(bool b) {
    last_bit_i++;
    if (last_bit_i == 8) {
        last_bit_i = 0;
        data.push_back(0);
    }
    data.back() |= b * (1 << last_bit_i);
}

void bytes_seq::push_byte(unsigned char b) {
    if (last_bit_i != 7) throw "bytes_seq::push_byte: non-empty buffer";
    data.push_back(b);
}

void bytes_seq::pop_bit() {
    if (last_bit_i > 0) {
        data.back() &= 255 - (1 << last_bit_i);
        last_bit_i--;
    } else {
        data.pop_back();
        last_bit_i = 7;
    }
}

unsigned char bytes_seq::get_byte(int i) const {
    return data[i];
}

bool bytes_seq::get_bit(int i) const {
    return (data[i / 8] & (1 << (i % 8))) > 0;
}

void bytes_seq::operator+=(const bytes_seq &b) {
    for (int i = 0; i < b.size_bit(); ++i) {
        push_bit(b.get_bit(i));
    }
}

bool bytes_seq::operator==(const bytes_seq &b) const {
    return last_bit_i == b.last_bit_i and data == b.data;
}

bytes_seg_ofstream::~bytes_seg_ofstream(){
    close_byte_stream();
}

void bytes_seg_ofstream::open_byte_stream(const std::string &s){
    open(s);
    buff = 0;
    last_bit_i = -1;
}

void bytes_seg_ofstream::close_byte_stream(){
    flush_buff();
    close();
}

// переписать чтобы писалось по байту за операцию через префикс одного и суффикс другого бита
void bytes_seg_ofstream::put_bytes(const bytes_seq &b) {
    for (int i = 0; i < b.size_bit(); ++i) {
        last_bit_i++;
        buff |= b.get_bit(i) * (1<<last_bit_i);
        if(last_bit_i == 7){
            flush_buff();
        }
    }
}

void bytes_seg_ofstream::flush_buff(){
    if(last_bit_i != -1) {
        put(buff);
        buff = 0;
        last_bit_i = -1;
    }
}