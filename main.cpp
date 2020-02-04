#include "src/ecc/UnsignedBigInteger.h"
#include "src/ecc/BigInteger.h"
#include "src/ecc/ModularBigInteger.h"

using ecc::UnsignedBigInteger;
using ecc::BigInteger;
using ecc::ModularBigInteger;

int main(int argc, char *argv[]) {
    ModularBigInteger a = ModularBigInteger("12", "23");
    ModularBigInteger b = ModularBigInteger("123", "23");
    BigInteger x, y;
    ModularBigInteger::euclidian(
            static_cast<UnsignedBigInteger>(a),
            static_cast<UnsignedBigInteger>(b),
            x,
            y
    );

    std::cout << a << std::endl
              << b << std::endl
              << x << std::endl
              << y << std::endl;

    return 0;
}