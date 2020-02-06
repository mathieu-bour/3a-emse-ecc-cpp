#ifndef INC_3A_ECC_CPP_SIGNEDBIGINTEGER_H
#define INC_3A_ECC_CPP_SIGNEDBIGINTEGER_H

#include "ECCTypes.h"
#include "UnsignedBigInteger.h"

namespace ecc {
    class SignedBigInteger {
    public:
        static const Sign SIGN_NEGATIVE = -1;
        static const Sign SIGN_NULL = 0;
        static const Sign SIGN_POSITIVE = 1;

        Sign sign;
        UnsignedBigInteger value;


        /**
         * Default constructor with a single unsigned 32-bits digit. By default, if no args were given, it is initialized to
         * zero.
         * @param pDigit The 32-bits digit.
         */
        SignedBigInteger(Digit pDigit = 0);


        /**
         * Construct from single signed 32-bits digit. By default, if no args were given, it is initialized
         * to zero.
         * @param pDigit The signed 32-bits digit.
         */
        SignedBigInteger(int32_t pDigit);


        /**
         * Construct from an unsigned big integer. The big integer can be declared as negative by passing SIGN_NEGATIVE.
         * @param pValue
         * @param pSign
         */
        SignedBigInteger(const UnsignedBigInteger &pValue, Sign pSign = SIGN_POSITIVE);


        /**
         * Construct from a string. String may start with a dash to indicate that the number is negative.
         * @param str
         */
        SignedBigInteger(const std::string &str);


        /**
         * Default assignment operator.
         * @param other The other signed big integer.
         * @return This assigned signed big integer.
         */
        SignedBigInteger &operator=(const SignedBigInteger &other);


        /**
         * Assignment from a unsigned 32-bit integer, which allow easier initialization.
         * @param str The unsigned 32-bit integer.
         * @return This assigned signed big integer.
         */
        SignedBigInteger &operator=(uint32_t unsignedDigit);


        /**
         * Assignment from a signed 32-bit integer, which allow easier initialization.
         * @param str The signed 32-bit integer.
         * @return This assigned signed big integer.
         */
        SignedBigInteger &operator=(int32_t signedDigit);


        /**
         * Assignment from a string, which allow easier initialization.
         * @param str The string representing the number in base 10.
         * @return This assigned signed big integer.
         */
        SignedBigInteger &operator=(const std::string &str);


        /**
         * Equality operator.
         * @param other The other signed bit integer.
         * @return
         */
        bool operator==(const SignedBigInteger &other) const;


        /**
         * Equality operator with a signed 32 bits signed integer.
         * @param signedInteger The 32-bit signed integer.
         * @return
         */
        bool operator==(const int32_t signedInteger);


        /**
         * Inequality operator.
         * @param other The other signed bit integer.
         * @return
         */
        bool operator!=(const SignedBigInteger &other) const {
            return !(other == *this);
        }


        SignedBigInteger &operator++() {
            if (isPositive()) {
                value++;
            } else if (isNegative()) {
                value--;

                if (value == 0) {
                    sign = 0;
                }
            } else {
                value = 1;
                sign = 1;
            }

            return *this;
        }


        SignedBigInteger &operator--() {
            if (isPositive()) {
                value--;

                if (value == 0) {
                    sign = 0;
                }
            } else if (isNegative()) {
                value++;
            } else {
                value = 1;
                sign = -1;
            }

            return *this;
        }


        SignedBigInteger operator+(const SignedBigInteger &other) {
            SignedBigInteger sum(*this);
            sum += other;
            return sum;
        }


        SignedBigInteger &operator+=(const SignedBigInteger &other) {
            if (sign == other.sign) {
                // Same sign, do the addition
                value += other.value;
            } else if (value > other.value) {
                // Opposite signs, u1 is bigger => sign will not change
                value -= other.value;
            } else {
                // Opposite signs, u2 is bigger => flip the sign
                value = other.value - value;
                sign = other.sign;
            }

            if (value == 0) {
                sign = 0;
            }

            return *this;
        }


        SignedBigInteger &operator-=(const SignedBigInteger &other) {
            if (value == 0 && other != 0) {
                sign = other.sign;
            }

            if (sign != other.sign) {
                // Opposite sign, do the addition
                value += other.value;
            } else if (value > other.value) {
                // Same signs, u1 is bigger => sign will not change
                value -= other.value;
            } else {
                // Same signs, u2 is bigger => flip the sign
                value = other.value - value;
                sign = other.sign * -1;
            }

            return *this;
        }


        friend SignedBigInteger operator-(const SignedBigInteger &source, const SignedBigInteger &delta) {
            SignedBigInteger difference(source);
            difference -= delta;
            return difference;
        }


        SignedBigInteger operator*=(const SignedBigInteger &other) {
            sign *= other.sign;
            value *= other.value;
            return *this;
        }


        friend SignedBigInteger operator*(const SignedBigInteger &a, const SignedBigInteger &b) {
            SignedBigInteger product(a);
            product *= b;
            return product;
        }


        SignedBigInteger operator/=(const SignedBigInteger &other) {
            sign *= other.sign;
            value /= other.value;
            return *this;
        }


        friend SignedBigInteger operator/(const SignedBigInteger &a, const SignedBigInteger &b) {
            SignedBigInteger quotient(a);
            quotient /= b;
            return quotient;
        }


        SignedBigInteger operator%=(const SignedBigInteger &other) {
            value %= other.value;
            sign = value == 0 ? 0 : 1;
            return *this;
        }


        friend SignedBigInteger operator%(const SignedBigInteger &a, const SignedBigInteger &b) {
            SignedBigInteger reminder(a);
            reminder %= b;
            return reminder;
        }


        /**
         * Left shift operator.
         * @param shiftLeft The number of bits to shift to the left.
         * @return The shifted signed big integer.
         */
        SignedBigInteger operator<<(const size_t shiftLeft);


        /**
         * Left shift assignment operator.
         * @param shiftLeft The number of bits to shift to the left.
         * @return The shifted signed big integer reference.
         */
        SignedBigInteger &operator<<=(const size_t shiftLeft);


        /**
         * Left shift operator.
         * @param shiftRight The number of bits to shift to the right.
         * @return The shifted signed big integer.
         */
        SignedBigInteger operator>>(const size_t shiftRight) const;


        /**
         * Left shift assignment operator.
         * @param shiftRight The number of bits to shift to the right.
         * @return The shifted signed big integer reference.
         */
        SignedBigInteger &operator>>=(const size_t shiftRight);


        /**
         * Bitwise AND operator.
         * @param other The other big integer.
         * @return this AND other.
         */
        SignedBigInteger operator&(const SignedBigInteger &other) const;


        /**
         * Bitwise AND assignment operator.
         * @param other The other big integer.
         * @return this AND other.
         */
        SignedBigInteger &operator&=(const SignedBigInteger &other);


        [[nodiscard]] std::string to_string() {
            std::string res;

            if (sign == -1) {
                res.append("-");
            }

            res.append(value.to_string());

            return res;
        }


        friend std::istream &operator>>(std::istream &inputStream, SignedBigInteger &result) {
            char digit = '\0';
            inputStream >> digit;

            // Handle minus sign
            if (digit == '-') {
                result.sign = -1;
                inputStream >> digit;
            }

            if (inputStream.good() && std::isdigit(digit)) {
                result.value = digit - '0'; // Convert from ASCII

                while (std::isdigit(inputStream.peek())) {
                    inputStream >> digit;
                    // Safe to use result as a number since we implemented the operators
                    result.value = UnsignedBigInteger(10) * result.value + (digit - '0');
                }
            } else {
                inputStream.setstate(std::ios_base::failbit);
            }

            return inputStream;
        }


        friend std::ostream &operator<<(std::ostream &os, SignedBigInteger &u) {
            os << u.to_string();
            return os;
        }


        size_t getMostSignificantBitIndex() const;


        /**
         * Execute the extended euclidean algorithm on u and v such as u*x + v*y = 1.
         * @param u A signed big integer.
         * @param v Another signed big integer.
         * @param x The u's co-factor.
         * @param y The v's co-factor.
         * @return The greatest common divider of u and v.
         */
        static SignedBigInteger euclidean(
                const SignedBigInteger &u,
                const SignedBigInteger &v,
                SignedBigInteger &x,
                SignedBigInteger &y
        );


        const bool isPositive() const {
            return sign == 1;
        }


        const bool isNegative() const {
            return sign == -1;
        }
    };
}

#endif //INC_3A_ECC_CPP_SIGNEDBIGINTEGER_H
