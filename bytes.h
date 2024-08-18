#include <vector>

namespace bytes {
    class bytes_seq {
        friend class std::hash<bytes_seq>;
    private:
        std::vector<unsigned char> data;
        int last_bit_i = 7;
    public:
        std::size_t size_byte() const {
            return data.size();
        }

        std::size_t size_bit() const {
            return (data.size() - 1) * 8 + last_bit_i + 1;
        }

        void push_bit(bool b) {
            last_bit_i++;
            if (last_bit_i == 8) {
                last_bit_i = 0;
                data.push_back(0);
            }
            data.back() |= b * (1 << last_bit_i);
        }

        void push_byte(unsigned char b) {
            if (last_bit_i != 7) throw std::exception();
            data.push_back(b);
        }

        void pop_bit() {
            if (last_bit_i > 0) {
                data.back() &= 255 - (1 << last_bit_i);
                last_bit_i--;
            } else {
                data.pop_back();
                last_bit_i = 7;
            }
        }

        unsigned char get_byte(int i) const {
            return data[i];
        }

        bool get_bit(int i) const {
            return (data[i / 8] & (1 << (i % 8))) > 0;
        }

        void operator+=(bytes_seq &b) {
            for (int i = 0; i < b.size_bit(); ++i) {
                push_bit(b.get_bit(i));
            }
        }

        bool operator==(const bytes_seq &b) const {
            return last_bit_i == b.last_bit_i and data == b.data;
        }
    };
}

template<>
struct std::hash<bytes::bytes_seq> {
    size_t operator()(const bytes::bytes_seq &data) const {
        size_t value = data.size_byte();
        for (unsigned char i: data.data) {
            value = hash<unsigned char>{}(i) + (value << 6) + (value >> 2);
        }
        return value;
    }
};