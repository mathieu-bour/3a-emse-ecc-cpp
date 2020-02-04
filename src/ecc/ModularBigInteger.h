#ifndef INC_3A_ECC_CPP_MODULARBIGINTEGER_H
#define INC_3A_ECC_CPP_MODULARBIGINTEGER_H

#include <map>
#include "UnsignedBigInteger.h"
#include "BigInteger.h"

namespace ecc {
    struct MontgomeryCache {
        size_t k = 0;
        UnsignedBigInteger n_p;
        UnsignedBigInteger r;
        UnsignedBigInteger r_p;
    };

    class ModularBigInteger : public UnsignedBigInteger {
    public:
        UnsignedBigInteger modulus;

        ModularBigInteger(UnsignedBigInteger &source, UnsignedBigInteger &pModulus) {
            digits = source.digits;
            modulus = pModulus;
        }

        /**
         * From string constructor.
         * This is the most important one, which allow to initialize very big numbers from a string based number in
         * base 10.
         * @param str The big number to instantiate (base 10).
         */
        ModularBigInteger(const std::string &str, const UnsignedBigInteger &pModulus) {
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
        ModularBigInteger &operator+=(const ModularBigInteger &other) {
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
        friend ModularBigInteger operator*(ModularBigInteger a, const ModularBigInteger &b) {

        }

        static UnsignedBigInteger euclidian(
                const UnsignedBigInteger &a,
                const UnsignedBigInteger &b,
                BigInteger &pX,
                BigInteger &pY
        ) {
            BigInteger x1 = 1, y1 = 0;
            UnsignedBigInteger a1 = a;
            BigInteger x2 = 0, y2 = 1;
            UnsignedBigInteger a2 = b;

            while (a2 != 0) {
                BigInteger x0 = x1;
                BigInteger y0 = y1;
                UnsignedBigInteger a0 = a1;
                x1 = x2;
                y1 = y2;
                a1 = a2;

                UnsignedBigInteger q = a0 / a1;
                a2 = a0 - q * a1;
                if (a2 == 0)
                    break;

                x2 = x0 - BigInteger(q) * x1;
                y2 = y0 - BigInteger(q) * y1;
            }

            pX = x1;
            pY = y1;
            return a1; // the GCD
        }

    private:

        /**
         * Check modulus compatibility.
         * @param other
         */
        void checkModulus(const ModularBigInteger &other) {
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
#endif //INC_3A_ECC_CPP_MODULARBIGINTEGER_H
