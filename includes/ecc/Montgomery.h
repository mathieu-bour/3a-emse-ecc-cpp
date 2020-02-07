//
// Created by mathieu on 07/02/2020.
//

#ifndef INC_3A_ECC_CPP_MONTGOMERY_H
#define INC_3A_ECC_CPP_MONTGOMERY_H

#include "UnsignedBigInteger.h"

namespace ecc {
    class Montgomery {
    public:
        UnsignedBigInteger modulus;
        size_t reducerBits;
        UnsignedBigInteger r;
        UnsignedBigInteger invR;
        UnsignedBigInteger invN;
        UnsignedBigInteger r2modN;

        Montgomery(const UnsignedBigInteger &pModulus);

        UnsignedBigInteger montgomeryRef(const UnsignedBigInteger &a, const UnsignedBigInteger &b) const;

        UnsignedBigInteger montgomery(const UnsignedBigInteger &a, const UnsignedBigInteger &b) const;

        UnsignedBigInteger modR(const UnsignedBigInteger &in) const;

        UnsignedBigInteger divR(const UnsignedBigInteger &in) const;

        static UnsignedBigInteger knuthModularInverse(const UnsignedBigInteger &in, const UnsignedBigInteger &mod);

        UnsignedBigInteger multiplication(const UnsignedBigInteger &a, const UnsignedBigInteger &b) const;
    };
}


#endif //INC_3A_ECC_CPP_MONTGOMERY_H
