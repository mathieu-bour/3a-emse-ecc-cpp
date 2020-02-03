//
// Created by mathieu on 03/02/2020.
//

#ifndef INC_3A_ECC_CPP_MONTGOMERY_H
#define INC_3A_ECC_CPP_MONTGOMERY_H

#include <map>
#include "UnsignedBigInteger.h"

namespace ecc {
    struct MontgomeryCache {
        size_t k = 0;
        UnsignedBigInteger n_p;
        UnsignedBigInteger r;
        UnsignedBigInteger r_p;
    };

    class Montgomery {
    public:
        static MontgomeryCache *fromCache(UnsignedBigInteger n) {
            auto result = cache.find(n);

            if (result != cache.end()) {
                return &result->second;
            }

            size_t k = n.digits.size() * UnsignedBigInteger::BITS;
            for (; !n.getBit(k); k--);

            MontgomeryCache fragment;
            fragment.k = k;
            fragment.n_p = UnsignedBigInteger();
            fragment.r = UnsignedBigInteger(2) << k;
            fragment.r_p = UnsignedBigInteger();

            gcdExtended(n, fragment.r, fragment.n_p, fragment.r_p);

            cache.insert({n, fragment});

            return fromCache(n);
        }

        static UnsignedBigInteger gcdExtended(UnsignedBigInteger a, UnsignedBigInteger b, UnsignedBigInteger &x,
                                              UnsignedBigInteger &y) {
            // Base Case
            if (a == 0) {
                x = 0;
                y = 1;
                return b;
            }

            UnsignedBigInteger x1, y1; // To store results of recursive call
            UnsignedBigInteger gcd = gcdExtended(b % a, a, x1, y1);

            // Update x and y using results of
            // recursive call
            x = y1 - (b / a) * x1;
            y = x1;

            return gcd;
        }

        static UnsignedBigInteger multiply(UnsignedBigInteger a, UnsignedBigInteger b, UnsignedBigInteger n) {
            UnsignedBigInteger c_m = multiplyM(a, b, n);
            UnsignedBigInteger c = multiplyM(c_m, 1);
            return c;
        }

        static UnsignedBigInteger multiplyM(UnsignedBigInteger a, UnsignedBigInteger b, UnsignedBigInteger n) {
            MontgomeryCache *fragment = fromCache(n);

            UnsignedBigInteger a_m = reduce(a, n);
            UnsignedBigInteger b_m = reduce(b, n);
            UnsignedBigInteger s = a_m * b_m;
            UnsignedBigInteger t = modR(s * fragment->n_p);
            UnsignedBigInteger m = s + t * n;
            UnsignedBigInteger u = divR(m);

            if (u > n) {
                return u - n;
            } else {
                return u;
            }
        }

        static UnsignedBigInteger reduce(const UnsignedBigInteger &in, const UnsignedBigInteger &n) {
            MontgomeryCache *fragment = fromCache(in);
            UnsignedBigInteger q = modR(modR(in) * fragment->n_p);
            UnsignedBigInteger qn = q * n;

            if (in > qn) {
                return divR(in - qn);
            } else {
                return divR(in + fragment->r * n - qn);
            }
        }

        static UnsignedBigInteger modR(const UnsignedBigInteger &in) {
            MontgomeryCache *fragment = fromCache(in);
            return in & (fragment->r - 1);
        }

        static UnsignedBigInteger divR(const UnsignedBigInteger &in) {
            MontgomeryCache *fragment = fromCache(in);
            return in >> fragment->k;
        }

    private:
        static std::map<UnsignedBigInteger, MontgomeryCache> cache;
    };

    std::map<UnsignedBigInteger, MontgomeryCache> Montgomery::cache = {};
}
#endif //INC_3A_ECC_CPP_MONTGOMERY_H
