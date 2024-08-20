#include <vector>
#include <fstream>

class bytes_seq {
    friend class std::hash<bytes_seq>;
private:
    std::vector<unsigned char> data;
    int last_bit_i = 7;
public:
    bytes_seq() = default;
    std::size_t size_byte() const;
    std::size_t size_bit() const;
    void push_bit(bool b);
    void push_byte(unsigned char b);
    void pop_bit();
    unsigned char get_byte(int i) const;
    bool get_bit(int i) const;
    void operator+=(const bytes_seq &b);
    bool operator==(const bytes_seq &b) const;
};

class bytes_seg_ofstream : public std::ofstream {
private:
    char buff = 0;
    int last_bit_i = -1;
public:
    bytes_seg_ofstream() = default;
    bytes_seg_ofstream(const std::string &s) : std::ofstream(s) {}
    ~bytes_seg_ofstream();
    void open_byte_stream(const std::string &s);
    void close_byte_stream();
    void put_bytes(const bytes_seq &b);
    void flush_buff();
};

template<>
struct std::hash<bytes_seq> {
    size_t operator()(const bytes_seq &data) const {
        size_t value = data.size_byte();
        for (unsigned char i: data.data) {
            value = hash<unsigned char>{}(i) + (value << 6) + (value >> 2);
        }
        return value;
    }
};