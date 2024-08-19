#include <cstring>
#include "compression.h"

int main(int argc, char* argv[]) {
    if(argc != 3){
        printf("Usage:\n-e filename\n-d filename");
        return 0;
    }
    if(strcmp(argv[1], "-e") == 0){
        compression::encode(argv[2]);
    }
    else if(strcmp(argv[1], "-d") == 0){
        compression::decode(argv[2]);
    }
    else{
        printf("Usage:\n-e filename\n-d filename");
    }
    return 0;
}
