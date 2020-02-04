#include "src/ecc/UnsignedBigInteger.h"
#include "src/ecc/BigInteger.h"
#include "src/ecc/ModularBigInteger.h"

using ecc::UnsignedBigInteger;
using ecc::ModularBigInteger;

int main(int argc, char *argv[]) {
//    std::string aStr = std::string(argv[1]);
//    std::string bStr = std::string(argv[2]);
//    std::string modStr = std::string(argv[3]);
//
//    UnsignedBigInteger mod = UnsignedBigInteger(modStr);
//    ModularBigInteger a = ModularBigInteger(aStr, mod);
//    ModularBigInteger b = ModularBigInteger(bStr, mod);
//
//    std::cout << "A:" << a << std::endl
//              << "B:" << b << std::endl
//              << "A*B=" << a * b;
    UnsignedBigInteger a = UnsignedBigInteger("12");
    UnsignedBigInteger b = UnsignedBigInteger("15");
    UnsignedBigInteger m = UnsignedBigInteger("71");
    ecc::BigInteger x, y;
    ModularBigInteger::euclidian(a, b, x, y);
    return 0;
}