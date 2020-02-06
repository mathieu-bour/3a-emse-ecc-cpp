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
        UnsignedBigInteger reducer;
        UnsignedBigInteger mask;
        UnsignedBigInteger reciprocal;
        UnsignedBigInteger invMod;
        UnsignedBigInteger factor;
        UnsignedBigInteger one;
        UnsignedBigInteger r2;


        Montgomery(const UnsignedBigInteger &pModulus);


        UnsignedBigInteger reduceRef(const UnsignedBigInteger &in) const;
        UnsignedBigInteger reduceWiki(const UnsignedBigInteger &T) const;


        UnsignedBigInteger multiply(const UnsignedBigInteger &a, const UnsignedBigInteger &b) const;


        UnsignedBigInteger knuthModularInverse(const UnsignedBigInteger &in) const;


        UnsignedBigInteger modR(const UnsignedBigInteger &in) const;


        UnsignedBigInteger divR(const UnsignedBigInteger &in) const;
    };
}


#endif //INC_3A_ECC_CPP_MONTGOMERY_H
