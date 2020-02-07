#ifndef INC_3A_ECC_CPP_MODULARBIGINTEGER_H
#define INC_3A_ECC_CPP_MODULARBIGINTEGER_H

#include "ECCTypes.h"
#include "UnsignedBigInteger.h"
#include "SignedBigInteger.h"

namespace ecc {
    class ModularBigInteger {
    public:
        UnsignedBigInteger value;
        UnsignedBigInteger modulus;
        UnsignedBigInteger r;
        UnsignedBigInteger invR;
        UnsignedBigInteger invModulus;

        ModularBigInteger() : value(0), modulus(1) {}


        ModularBigInteger(const UnsignedBigInteger &pValue, const UnsignedBigInteger &pModulus) {
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
         * Addition operator.
         * @param other The other modular big integer to add.
         * @return The modular big integer sum.
         */
        ModularBigInteger operator+(const ModularBigInteger &other);


        /**
         * Addition assignment operator.
         * @param other The other modular big integer to add.
         * @return The modular big integer sum reference.
         */
        ModularBigInteger &operator+=(const ModularBigInteger &other);


        /**
         * Subtraction operator.
         * @param delta The other modular big integer to subtract.
         * @return The modular big integer difference.
         */
        ModularBigInteger operator-(const ModularBigInteger &delta);


        /**
         * Subtraction assignment operator.
         * @param delta The other modular big integer to subtract.
         * @return The modular big integer difference reference.
         */
        ModularBigInteger &operator-=(const ModularBigInteger &delta);


        /**
         * Multiplication operator.
         * @param other The other modular big integer to multiply.
         * @return The modular big integer product.
         */
        ModularBigInteger operator*(const ModularBigInteger &other) const;


        /**
         * Multiplication assignment operator.
         * @param other The other modular big integer to multiply.
         * @return The modular big integer product reference.
         */
        ModularBigInteger &operator*=(const ModularBigInteger &other);

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
