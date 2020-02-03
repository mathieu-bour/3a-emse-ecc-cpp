#include "src/ecc/UnsignedBigInteger.h"
#include "src/ecc/ModularUnsignedBigInteger.h"

using ecc::UnsignedBigInteger;
using ecc::ModularUnsignedBigInteger;

int main(int argc, char *argv[]) {
    std::string aStr = std::string(argv[1]);
    std::string bStr = std::string(argv[2]);
    std::string modStr = std::string(argv[3]);

    UnsignedBigInteger mod = UnsignedBigInteger(modStr);
    ModularUnsignedBigInteger a = ModularUnsignedBigInteger(aStr, mod);
    ModularUnsignedBigInteger b = ModularUnsignedBigInteger(bStr, mod);

    std::cout << "A:" << a << std::endl
              << "B:" << b << std::endl
              << "A*B=" << a * b;
    return 0;
}