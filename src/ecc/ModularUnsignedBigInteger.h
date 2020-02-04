#ifndef INC_3A_ECC_CPP_MODULARUNSIGNEDBIGINTEGER_H
#define INC_3A_ECC_CPP_MODULARUNSIGNEDBIGINTEGER_H

#include <map>
#include "UnsignedBigInteger.h"

namespace ecc {
    struct MontgomeryCache {
        size_t k = 0;
        UnsignedBigInteger n_p;
        UnsignedBigInteger r;
        UnsignedBigInteger r_p;
    };

    class ModularUnsignedBigInteger : public UnsignedBigInteger {
    public:
        UnsignedBigInteger modulus;

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

        }

        static void euclidian(
            const UnsignedBigInteger &a,
            const UnsignedBigInteger &b,
            UnsignedBigInteger &pX,
            UnsignedBigInteger &pY
        ) {
            UnsignedBigInteger x0 = 1, y0 = 0, a0 = a;
            UnsignedBigInteger x1 = 0, y1 = 1, a1 = b;

            while (a1 != 0) {
                UnsignedBigInteger q = a0 / a1;
                UnsignedBigInteger a2 = a0 - q * a1;
                UnsignedBigInteger x2 = x0 - q * x1;
                UnsignedBigInteger y2 = y0 - q * y1;
                x0 = x1;
                y0 = y1;
                a0 = a1;
                x1 = x2;
                y1 = y2;
                a1 = a2;
            }

            pX = x0;
            pY = y0;
        }
    private:

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
