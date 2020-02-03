#ifndef INC_3A_ECC_CPP_MODULARUNSIGNEDBIGINTEGER_H
#define INC_3A_ECC_CPP_MODULARUNSIGNEDBIGINTEGER_H

#include "UnsignedBigInteger.h"
#include "Montgomery.h"

namespace ecc {
    class ModularUnsignedBigInteger : public UnsignedBigInteger {
    public:
        ModularUnsignedBigInteger(UnsignedBigInteger &source, UnsignedBigInteger &pModulus) {
            digits = source.digits;
            modulus = pModulus;
        }

        /**
         * From string constructor.
         * This is the most important one, which allow to initialize very big numbers from a string based number in
         * base 10.
         * @param str The big number to instantiate (base 10).
         */
        ModularUnsignedBigInteger(const std::string &str, const UnsignedBigInteger &pModulus) {
            modulus = pModulus;
            std::istringstream inputStringStream(str);

            inputStringStream >> *this; // see std::istream &operator>>

            if (inputStringStream.fail() || !inputStringStream.eof()) {
                throw std::runtime_error("Error: UnsignedBigInteger::string");
            }

            weakReduction();
        }

        explicit operator UnsignedBigInteger() {
            return UnsignedBigInteger(digits);
        }


        /**
         * Addition assignment operator.
         * @param other The other big integer to add.
         * @return The sum big integer reference.
         */
        ModularUnsignedBigInteger &operator+=(const ModularUnsignedBigInteger &other) {
            checkModulus(other); // Check modulus compatibility
            UnsignedBigInteger::operator+=(other);
            weakReduction();

            return *this;
        }

        /**
         * Implementation of the Montgomery multiplication
         * @param other
         * @return
         */
        friend ModularUnsignedBigInteger operator*(ModularUnsignedBigInteger a, const ModularUnsignedBigInteger &b) {
            UnsignedBigInteger c = Montgomery::multiply(static_cast<UnsignedBigInteger>(a), b, a.modulus);
            UnsignedBigInteger m = a.modulus;
            return ModularUnsignedBigInteger(c, m);
        }


    private:
        UnsignedBigInteger modulus;

        /**
         * Check modulus compatibility.
         * @param other
         */
        void checkModulus(const ModularUnsignedBigInteger &other) {
            if (modulus != other.modulus) {
            }
        }

        /**
         * Reduce the number against their modulus (slow).
         */
        void weakReduction() {
            *this %= modulus;
        }
    };
}
#endif //INC_3A_ECC_CPP_MODULARUNSIGNEDBIGINTEGER_H
