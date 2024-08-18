#include "compression.h"

signed main() {
    compression::encode("test.png"); // -> test.png.wtf
    compression::decode("test.png.wtf"); // -> decoded_test.png
}
