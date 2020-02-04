#ifndef INC_3A_ECC_CPP_MODULARBIGINTEGER_H
#define INC_3A_ECC_CPP_MODULARBIGINTEGER_H

#include "ECCTypes.h"
#include "UnsignedBigInteger.h"
#include "BigInteger.h"

namespace ecc {
    class ModularBigInteger {
    public:
        UnsignedBigInteger value;
        UnsignedBigInteger modulus;

        ModularBigInteger(UnsignedBigInteger &pValue, UnsignedBigInteger &pModulus) {
            modulus = pModulus;
            value = pValue % modulus;
        }

        ModularBigInteger(const std::string &pValue, const std::string &pModulus) {
            modulus = UnsignedBigInteger(pModulus);
            value = UnsignedBigInteger(pValue) % modulus; // Hard reduction
        }

        bool operator==(const ModularBigInteger &other) const {
            return value == other.value && modulus == other.modulus;
        }

        bool operator!=(const ModularBigInteger &other) const {
            return !(other == *this);
        }

        bool operator<(const ModularBigInteger &other) const {
            return value < other.value;
        }

        bool operator>(const ModularBigInteger &other) const {
            return other < *this;
        }

        bool operator<=(const ModularBigInteger &other) const {
            return !(other < *this);
        }

        bool operator>=(const ModularBigInteger &other) const {
            return !(*this < other);
        }

        /**
         * Sum two modular integers.
         * @param a
         * @param b
         * @return
         */
        friend ModularBigInteger operator+(const ModularBigInteger &a, const ModularBigInteger &b) {
            ModularBigInteger sum(a);
            sum.value += b.value;
            sum.weakReduction();
            return sum;
        }

        friend ModularBigInteger operator-(const ModularBigInteger &a, const ModularBigInteger &b) {
            ModularBigInteger difference(a);

            if (a < b) {
                difference.value += difference.modulus;
            }

            difference.value -= b.value;
            return difference;
        }


        explicit operator UnsignedBigInteger() {
            return value;
        }

        static void euclidian(
                const ModularBigInteger &a,
                const ModularBigInteger &b,
                BigInteger &pX,
                BigInteger &pY,
                UnsignedBigInteger &gcd
        ) {
            euclidian(a.value, b.value, pX, pY, gcd);
        }

        static void euclidian(
                const UnsignedBigInteger &a,
                const UnsignedBigInteger &b,
                BigInteger &pX,
                BigInteger &pY,
                UnsignedBigInteger &gcd
        ) {
            BigInteger x0 = 1, y0 = 0;
            UnsignedBigInteger a0 = a;
            BigInteger x1 = 0, y1 = 1;
            UnsignedBigInteger a1 = b;

            while (a1 != 0) {
                UnsignedBigInteger q = a0 / a1;
                UnsignedBigInteger a2 = a0 - q * a1;
                if (a2 == 0) {
                    x0 = x1;
                    y0 = y1;
                    a0 = a1;
                    break;
                }

                BigInteger q2 = BigInteger(q);
                BigInteger x2 = x0 - q2 * x1;
                BigInteger y2 = y0 - q2 * y1;

                x0 = x1;
                y0 = y1;
                a0 = a1;
                x1 = x2;
                y1 = y2;
                a1 = a2;
            }

            pX = x0;
            pY = y0;
            gcd = a0;
        }

    private:
        /**
         * Weakly reduce the number, assuming there is only 1 overflow
         */
        void weakReduction() {
            if (value >= modulus) {
                value -= modulus;
            }
        }
    };
}
#endif //INC_3A_ECC_CPP_MODULARBIGINTEGER_H
