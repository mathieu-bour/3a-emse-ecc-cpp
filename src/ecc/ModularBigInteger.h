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

        explicit operator UnsignedBigInteger() {
            return value;
        }

        static UnsignedBigInteger euclidian(
                const UnsignedBigInteger &a,
                const UnsignedBigInteger &b,
                BigInteger &x,
                BigInteger &y
        ) {
            // Base Case
            if (a == 0) {
                x = 0;
                y = 1;
                return b;
            }

            BigInteger x1, y1; // To store results of recursive call
            UnsignedBigInteger gcd = euclidian(b % a, a, x1, y1);

            // Update x and y using results of
            // recursive call
            BigInteger b_a = BigInteger(b / a);
            x = y1 - b_a * x1;
            y = x1;

            return gcd;
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
