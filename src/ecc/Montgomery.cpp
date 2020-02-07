#include "../../includes/ecc/Montgomery.h"

using namespace ecc;


Montgomery::Montgomery(const UnsignedBigInteger &pModulus) {
    modulus = pModulus;
    reducerBits = pModulus.getMostSignificantBitIndex();
    r = UnsignedBigInteger(1) << reducerBits;
    invR = knuthModularInverse(r, modulus);
    invN = knuthModularInverse(modulus, r);
    invN = (r + r - invN) % r;
    r2modN = r * r % modulus; // Precompute r² mod modulus
}


UnsignedBigInteger Montgomery::montgomeryRef(const UnsignedBigInteger &a, const UnsignedBigInteger &b) const {
    return (a * b * invR) % modulus;
}

UnsignedBigInteger Montgomery::montgomery(const UnsignedBigInteger &a, const UnsignedBigInteger &b) const {
    UnsignedBigInteger T = a * b;
    UnsignedBigInteger m = modR(modR(T) * invN);
    UnsignedBigInteger t = divR(T + m * modulus);

    if (t >= modulus) {
        return t - modulus;
    } else {
        return t;
    }
}

UnsignedBigInteger Montgomery::multiplication(const UnsignedBigInteger &a, const UnsignedBigInteger &b) const {
    UnsignedBigInteger phyA = montgomery(a, r2modN);
    UnsignedBigInteger phyB = montgomery(b, r2modN);
    UnsignedBigInteger phyC = montgomery(phyA, phyB);
    UnsignedBigInteger c = montgomery(phyC, 1);
    return c;
}

UnsignedBigInteger Montgomery::modR(const UnsignedBigInteger &in) const {
    return in & UnsignedBigInteger(r - 1);
}


UnsignedBigInteger Montgomery::divR(const UnsignedBigInteger &in) const {
    return in >> (r.getMostSignificantBitIndex() - 1);
}


UnsignedBigInteger Montgomery::knuthModularInverse(const UnsignedBigInteger &in, const UnsignedBigInteger &mod) {
    UnsignedBigInteger inv, u1, u3, v1, v3, t1, t3, q;
    int iter;
    /* Step X1. Initialise */
    u1 = 1;
    u3 = in;
    v1 = 0;
    v3 = mod;
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
        inv = mod - u1;
    else
        inv = u1;
    return inv;
}