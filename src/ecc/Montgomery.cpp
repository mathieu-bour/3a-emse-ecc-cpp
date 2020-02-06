#include "../../includes/ecc/Montgomery.h"

using namespace ecc;


Montgomery::Montgomery(const UnsignedBigInteger &pModulus) {
    modulus = pModulus;
    reducerBits = pModulus.getMostSignificantBitIndex();
    reducer = UnsignedBigInteger(1) << reducerBits;
    mask = reducer - 1;
    reciprocal = Montgomery::knuthModularInverse(reducer);
    factor = (reducer * (reciprocal % modulus) - 1) / modulus;
    one = reducer % modulus;
    r2 = (reducer * reducer) % modulus;
}


UnsignedBigInteger Montgomery::reduceRef(const UnsignedBigInteger &x) const {
    return (x << reducerBits) % modulus;
}


UnsignedBigInteger Montgomery::reduceWiki(const UnsignedBigInteger &T) const {
    UnsignedBigInteger ni = reducer - reciprocal;
    UnsignedBigInteger m = ((T % reducer) * ni) % reducer;
    UnsignedBigInteger x = (T + m * modulus) / reducer;

    if (x < modulus) {
        return x;
    } else {
        return x - modulus;
    }
}


UnsignedBigInteger Montgomery::multiply(const UnsignedBigInteger &a, const UnsignedBigInteger &b) const {
    UnsignedBigInteger A = reduceRef(a);
    UnsignedBigInteger B = reduceRef(b);
    UnsignedBigInteger product = A * B;
    UnsignedBigInteger t = ((product & mask) * factor) & mask;
    UnsignedBigInteger red = (product + t * modulus) >> reducerBits;
    UnsignedBigInteger result;

    if (red < modulus) {
        result = red;
    } else {
        result = red - modulus;
    }

    return (result * reciprocal) % modulus;
}


UnsignedBigInteger Montgomery::modR(const UnsignedBigInteger &in) const {
    return in & UnsignedBigInteger(reducer - 1);
}


UnsignedBigInteger Montgomery::divR(const UnsignedBigInteger &in) const {
    return in >> (reducer.getMostSignificantBitIndex() + 1);
}


UnsignedBigInteger Montgomery::knuthModularInverse(const UnsignedBigInteger &in) const {
    UnsignedBigInteger inv, u1, u3, v1, v3, t1, t3, q;
    int iter;
    /* Step X1. Initialise */
    u1 = 1;
    u3 = in;
    v1 = 0;
    v3 = modulus;
    /* Remember odd/even iterations */
    iter = 1;
    /* Step X2. Loop while v3 != 0 */
    while (v3 != 0) {
        /* Step X3. Divide and "Subtract" */
        q = u3 / v3;
        t3 = u3 % v3;
        t1 = u1 + q * v1;
        /* Swap */
        u1 = v1;
        v1 = t1;
        u3 = v3;
        v3 = t3;
        iter = -iter;
    }
    /* Make sure u3 = gcd(u,v) == 1 */
    if (u3 != 1)
        return 0;   /* Error: No inverse exists */
    /* Ensure a positive result */
    if (iter < 0)
        inv = modulus - u1;
    else
        inv = u1;
    return inv;
}
