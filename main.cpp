#include "src/ecc/UnsignedBigInteger.cpp"

int main() {
    ECC::UnsignedBigInteger a = ECC::UnsignedBigInteger("27580193559959705877849011840389048093056905856361568521428707301988689241309860865136260764883745107765439761230575");
    std::cout << a * 2;
    return 0;
}