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
