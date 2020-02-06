#include "../../includes/ecc/SignedBigInteger.h"

using namespace ecc;


SignedBigInteger SignedBigInteger::euclidean(
        const SignedBigInteger &u,
        const SignedBigInteger &v,
        SignedBigInteger &x,
        SignedBigInteger &y
) {
    // Base Case
    if (u == 0) {
        x = 0;
        y = 1;
        return v;
    }

    SignedBigInteger x1, y1; // To store results of recursive call
    SignedBigInteger gcd = euclidean(v % u, u, x1, y1);

    // Update x and y using results of
    // recursive call
    x = y1 - (v / u) * x1;
    y = x1;

    return gcd;
}


/*
 * Constructors
 * ======================================================================
 */
SignedBigInteger::SignedBigInteger(Digit pDigit) {
    sign = pDigit == 0 ? SIGN_NULL : SIGN_POSITIVE;
    value = UnsignedBigInteger(pDigit);
}


SignedBigInteger::SignedBigInteger(int32_t pDigit) {
    sign = pDigit == 0 ? SIGN_NULL : (pDigit > 0 ? SIGN_POSITIVE : SIGN_NEGATIVE);
    value = UnsignedBigInteger(pDigit >= 0 ? pDigit : -pDigit);
}


SignedBigInteger::SignedBigInteger(const UnsignedBigInteger &pValue, Sign pSign) {
    sign = pValue == 0 ? SIGN_NULL : pSign;
    value = pValue;
}


SignedBigInteger::SignedBigInteger(const std::string &str) {
    sign = SIGN_POSITIVE; // By default, the number is positive.
    std::istringstream inputStringStream(str);

    inputStringStream >> *this; // see std::istream &operator>>

    if (value == 0) {
        sign = SIGN_NULL;
    }

    if (inputStringStream.fail() || !inputStringStream.eof()) {
        throw std::runtime_error("Error: BigInteger::BigInteger(const std::string &str)");
    }
}


/*
 * Operators
 * ======================================================================
 */
SignedBigInteger &SignedBigInteger::operator=(uint32_t unsignedDigit) {
    *this = SignedBigInteger(unsignedDigit);

    return *this;
}


SignedBigInteger &SignedBigInteger::operator=(int32_t signedDigit) {
    *this = SignedBigInteger(signedDigit);

    return *this;
}


SignedBigInteger &SignedBigInteger::operator=(const std::string &str) {
    *this = SignedBigInteger(str);

    return *this;
}


bool SignedBigInteger::operator==(const SignedBigInteger &other) const {
    return sign == other.sign && value == other.value;
}


bool SignedBigInteger::operator==(const int32_t signedInteger) {
    if (signedInteger == 0) {
        return sign == 0 && value == 0;
    } else if (signedInteger > 0) {
        return sign == 1 && value == signedInteger;
    } else {
        return sign == -1 && value == -signedInteger;
    }
}


SignedBigInteger SignedBigInteger::operator<<(size_t shiftLeft) {
    SignedBigInteger copy(*this);
    copy <<= shiftLeft;

    return copy;
}


SignedBigInteger &SignedBigInteger::operator<<=(const size_t shiftLeft) {
    value <<= shiftLeft;
    return *this;
}


SignedBigInteger SignedBigInteger::operator>>(size_t shiftRight) const {
    SignedBigInteger copy(*this);
    copy >>= shiftRight;

    return copy;
}


SignedBigInteger &SignedBigInteger::operator>>=(const size_t shiftRight) {
    value >>= shiftRight;
    return *this;
}


SignedBigInteger SignedBigInteger::operator&(const SignedBigInteger &other) const {
    SignedBigInteger copy(*this);
    copy &= other;

    return copy;
}


SignedBigInteger &SignedBigInteger::operator&=(const SignedBigInteger &other) {
    value &= other.value;
    sign = value == 0 ? SIGN_NULL : (isNegative() && other.isNegative() ? SIGN_NEGATIVE : SIGN_POSITIVE);
    return *this;
}


size_t SignedBigInteger::getMostSignificantBitIndex() const {
    return value.getMostSignificantBitIndex();
}


SignedBigInteger &SignedBigInteger::operator=(const SignedBigInteger &other) {
    sign = other.sign;
    value = other.value;
    return *this;
//    if (other.value > 0) {
//        sign = SIGN_POSITIVE;
//        value = other.value;
//    } else if (other.value < 0) {
//        sign = SIGN_NEGATIVE;
//        value = other.value;
//    } else {
//        sign = SIGN_NULL;
//        value = 0;
//    }
//
//    return *this;
}

