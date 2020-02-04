#ifndef INC_3A_ECC_CPP_BIGINTEGER_H
#define INC_3A_ECC_CPP_BIGINTEGER_H

#include "ECCTypes.h"
#include "UnsignedBigInteger.h"

namespace ecc {
    class BigInteger {
    public:
        Sign sign;
        UnsignedBigInteger value;

        BigInteger(const UnsignedBigInteger &pValue, Sign pSign = 1) {
            sign = pSign;
            value = pValue;
        }

        BigInteger(Digit digit = 0, Sign pSign = 0) : value(UnsignedBigInteger(digit)), sign(pSign) {}


        BigInteger(const std::string &str) : sign(1) {
            std::istringstream inputStringStream(str);

            inputStringStream >> *this; // see std::istream &operator>>

            if (inputStringStream.fail() || !inputStringStream.eof()) {
                throw std::runtime_error("Error: UnsignedBigInteger::string");
            }
        }

        BigInteger &operator=(Digit digit) {
            *this = BigInteger(digit, 1);
            return *this;
        };

        BigInteger &operator=(const std::string &str) {
            *this = BigInteger(str);
            return *this;
        };

        explicit operator UnsignedBigInteger() {
            return value;
        }

        bool operator==(const BigInteger &other) const {
            return sign == other.sign && value == other.value;
        }

        bool operator!=(const BigInteger &other) const {
            return !(other == *this);
        }

        BigInteger &operator=(const BigInteger &other) = default;


        BigInteger &operator++() {
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

        BigInteger &operator--() {
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

        BigInteger &operator+=(const BigInteger &other) {
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

        friend BigInteger operator+(const BigInteger &a, const BigInteger &b) {
            BigInteger sum(a);
            sum += b;
            return sum;
        }

        BigInteger &operator-=(const BigInteger &other) {
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

        friend BigInteger operator-(const BigInteger &source, const BigInteger &delta) {
            BigInteger difference(source);
            difference -= delta;
            return difference;
        }

        BigInteger operator*=(const BigInteger &other) {
            sign *= other.sign;
            value *= other.value;
            return *this;
        }

        friend BigInteger operator*(const BigInteger &a, const BigInteger &b) {
            BigInteger product(a);
            product *= b;
            return product;
        }

        [[nodiscard]] std::string to_string() {
            std::string res;

            if (sign == -1) {
                res.append("-");
            }

            res.append(value.to_string());

            return res;
        }

        friend std::istream &operator>>(std::istream &inputStream, BigInteger &result) {
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
                    result.value = 10 * result.value + (digit - '0');
                }
            } else {
                inputStream.setstate(std::ios_base::failbit);
            }

            return inputStream;
        }

        friend std::ostream &operator<<(std::ostream &os, BigInteger &u) {
            os << u.to_string();
            return os;
        }

    private:
        bool isPositive() {
            return sign == 1;
        }

        bool isNegative() {
            return sign == -1;
        }
    };
}

#endif //INC_3A_ECC_CPP_BIGINTEGER_H
