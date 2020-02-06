#include "../../includes/ecc/ModularBigInteger.h"

using namespace ecc;


ModularBigInteger ModularBigInteger::operator+(const ModularBigInteger &other) {
    ModularBigInteger sum(*this);
    sum += other;

    return sum;
}


ModularBigInteger &ModularBigInteger::operator+=(const ModularBigInteger &other) {
    value += other.value;
    weakReduction();

    return *this;
}


ModularBigInteger ModularBigInteger::operator-(const ModularBigInteger &delta) {
    ModularBigInteger difference(*this);
    difference -= delta;

    return difference;
}


ModularBigInteger &ModularBigInteger::operator-=(const ModularBigInteger &delta) {
    if (*this < delta) {
        value += modulus;
    }

    value -= delta.value;

    return *this;
}


ModularBigInteger ModularBigInteger::operator*(const ModularBigInteger &other) {
    ModularBigInteger product(*this);
    product *= other;

    return product;
}


ModularBigInteger &ModularBigInteger::operator*=(const ModularBigInteger &other) {
    value = montgomeryMultiplication(other);

    return *this;
}


void ModularBigInteger::computeMontgomeryParams() {
    if (invR != 0) {
        return;
    }

    r = 1;
    r <<= modulus.getMostSignificantBitIndex();
    SignedBigInteger signedInvR, signedInvModulus;
    SignedBigInteger::euclidean(r, modulus, signedInvR, signedInvModulus);

    invR = signedInvR.value;
    invModulus = signedInvModulus.value;
}


UnsignedBigInteger ModularBigInteger::montgomeryReduce() {
    computeMontgomeryParams();

    UnsignedBigInteger q = modR(modR(value) * invModulus);
    SignedBigInteger a = divR(SignedBigInteger(value) - q * modulus);

    if (a.isNegative()) {
        a += modulus;
    }

    return a.value;
}


UnsignedBigInteger ModularBigInteger::montgomeryMultiplication(ModularBigInteger other) {
    ModularBigInteger o("1", modulus.to_string());

    UnsignedBigInteger ap = montgomeryReduce();
    UnsignedBigInteger bp = other.montgomeryReduce();

    UnsignedBigInteger x = ap * bp;
    UnsignedBigInteger s = modR(modR(x) * invModulus);
    SignedBigInteger t = divR(x + s * modulus);

    if (t.value < modulus) {
        return t.value;
    } else {
        return t.value - modulus;
    }
}


UnsignedBigInteger ModularBigInteger::modR(const UnsignedBigInteger &in) const {
    return in & UnsignedBigInteger(r - 1);
}


SignedBigInteger ModularBigInteger::divR(const SignedBigInteger &in) const {
    return in >> r.getMostSignificantBitIndex();
}