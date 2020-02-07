#include "../../includes/ecc/ModularBigInteger.h"
#include "../../includes/ecc/Montgomery.h"

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


ModularBigInteger ModularBigInteger::operator*(const ModularBigInteger &other) const {
    ModularBigInteger product(*this);
    product *= other;

    return product;
}

ModularBigInteger &ModularBigInteger::operator*=(const ModularBigInteger &other) {
    Montgomery montgomery(modulus);
    value = montgomery.multiplication(value, other.value);
    return *this;
}
