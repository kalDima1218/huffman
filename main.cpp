#include "compression.h"

bool validate(const std::string& filename){
    encode(filename);
    decode(filename + ".wtf");
    std::ifstream fin;
    char chr;
    fin.open(filename);
    bytes_seq source;
    while(fin.get(chr)) {
        source.push_byte(chr);
    }
    fin.close();
    fin.open("decoded_" + filename);
    bytes_seq result;
    while(fin.get(chr)) {
        result.push_byte(chr);
    }
    fin.close();
    return source == result;
}

int main(int argc, char* argv[]) {
    if(argc != 2){
        printf("Usage:\nencode: ./program filename\ndecode: ./program filename.wtf");
        return 0;
    }
    std::string filename = argv[1];
    if(filename.size() < 4 || filename.substr(filename.size() - 4, 4) != ".wtf"){
        encode(filename);
    }
    else{
        decode(filename);
    }
    return 0;
}
