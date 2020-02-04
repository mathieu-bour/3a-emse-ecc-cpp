#ifndef INC_3A_ECC_CPP_BIGINTEGER_H
#define INC_3A_ECC_CPP_BIGINTEGER_H

#include "UnsignedBigInteger.h"

namespace ecc {
    class BigInteger : public UnsignedBigInteger {
    public:
        typedef std::int8_t Sign;
        Sign sign;

        BigInteger(const UnsignedBigInteger &integer, Sign pSign = 1) {
            digits = integer.digits;
            sign = pSign;
        }

        BigInteger(Digit digit = 0, Sign pSign = 0) : UnsignedBigInteger(digit), sign(pSign) {}

        BigInteger &operator=(const BigInteger &other) = default;


        BigInteger &operator++() {
            UnsignedBigInteger tmp = toUnsigned();

            if (isPositive()) {
                tmp++;
            } else if (isNegative()) {
                tmp--;

                if (tmp == 0) {
                    sign = 0;
                }
            } else {
                tmp = 1;
                sign = 1;
            }

            digits = tmp.digits;
        }

        BigInteger &operator--() {
            UnsignedBigInteger tmp = toUnsigned();

            if (isPositive()) {
                tmp--;

                if (tmp == 0) {
                    sign = 0;
                }
            } else if (isNegative()) {
                tmp++;
            } else {
                tmp = 1;
                sign = -1;
            }

            digits = tmp.digits;
        }

        BigInteger &operator+=(BigInteger other) {
            UnsignedBigInteger unsigned1 = toUnsigned();
            UnsignedBigInteger unsigned2 = other.toUnsigned();

            if (sign && other.sign) {
                // Same sign, do the addition
                digits = (unsigned1 + unsigned2).digits;
            } else if (unsigned1 > unsigned2) {
                // Opposite signs, u1 is bigger => sign will not change
                digits = (unsigned1 - unsigned2).digits;
            } else {
                // Opposite signs, u2 is bigger => flip the sign
                digits = (unsigned2 - unsigned1).digits;
                sign = other.sign;
            }

            return *this;
        }

        /**
         * Direct subtraction operator. Used the subtraction assignment operator.
         * @param source
         * @param delta
         * @return The difference between the source and the delta.
         */
        friend BigInteger operator-(const BigInteger &source, const BigInteger &delta) {
            BigInteger difference(source);
            difference -= delta;
            return difference;
        }

        BigInteger &operator-=(BigInteger other) {
            UnsignedBigInteger unsigned1 = toUnsigned();
            UnsignedBigInteger unsigned2 = other.toUnsigned();

            if (sign != other.sign) {
                // Opposite sign, do the addition
                digits = (unsigned1 + unsigned2).digits;
            } else if (unsigned1 > unsigned2) {
                // Same signs, u1 is bigger => sign will not change
                digits = (unsigned1 - unsigned2).digits;
            } else {
                // Same signs, u2 is bigger => flip the sign
                digits = (unsigned2 - unsigned1).digits;
                sign = other.sign;
            }

            return *this;
        }

        friend BigInteger operator*(BigInteger &a, BigInteger b) {
            BigInteger result;
            result.sign = a.sign * b.sign;
            result.digits = (a.toUnsigned() * b.toUnsigned()).digits;
            return result;
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
        bool isPositive() {
            return sign == 1;
        }

        bool isNegative() {
            return sign == -1;
        }

        UnsignedBigInteger toUnsigned() {
            return UnsignedBigInteger(digits);
        }
    };
}

#endif //INC_3A_ECC_CPP_BIGINTEGER_H
