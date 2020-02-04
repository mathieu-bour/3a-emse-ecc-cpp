#ifndef INC_3A_ECC_CPP_BIGINTEGER_H
#define INC_3A_ECC_CPP_BIGINTEGER_H

#include "UnsignedBigInteger.h"

namespace ecc {
    class BigInteger : public UnsignedBigInteger {
    public:
        typedef std::int8_t Sign;
        Sign sign;

        BigInteger(UnsignedBigInteger integer) {
            digits = integer.digits;
        }

        BigInteger(Digit digit, Sign pSign = 1) : UnsignedBigInteger(digit), sign(pSign) {}


        UnsignedBigInteger &operator++() {
            if (sign == -1) {
                this->operator--();
            } else {

            }
        }

        friend std::istream &operator>>(std::istream &inputStream, BigInteger &result) {
            char digit = '\0';
            inputStream >> digit;

            if (digit == '-') {
                result.sign = -1;
            }

            if (inputStream.good() && std::isdigit(digit)) {
                result = digit - '0'; // Convert from ASCII

                while (std::isdigit(inputStream.peek())) {
                    inputStream >> digit;
                    // Safe to use result as a number since we implemented the operators
                    result = 10 * result + (digit - '0');
                }
            } else {
                inputStream.setstate(std::ios_base::failbit);
            }

            return inputStream;
        }

    private:

    };
}

#endif //INC_3A_ECC_CPP_BIGINTEGER_H
