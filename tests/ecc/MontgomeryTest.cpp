#include "gtest/gtest.h"
#include "../../includes/ecc/UnsignedBigInteger.h"
#include "../../includes/ecc/Montgomery.h"

using ecc::UnsignedBigInteger;
using ecc::Montgomery;

TEST(Montgomery, knuthModularInverse) {
    UnsignedBigInteger one(1);
    UnsignedBigInteger primes[] = {17, 23, 53};

    for (size_t i = 0; i < 3; i++) {
        Montgomery montgomery(primes[i]);

        for (UnsignedBigInteger j(1); j < primes[i]; j++) {
            EXPECT_EQ(one,
                      (Montgomery::knuthModularInverse(j, primes[i]) * UnsignedBigInteger(j)) % montgomery.modulus);
        }
    }
}

TEST(Montgomery, montgomery) {
    UnsignedBigInteger primes[] = {17, 23, 53};
    UnsignedBigInteger rs[] = {32, 32, 64};

    for (size_t i = 0; i < 3; i++) {
        Montgomery montgomery(primes[i]);

        for (UnsignedBigInteger j(1); j < primes[i]; j++) {
            EXPECT_EQ(montgomery.montgomeryRef(j, rs[i]), montgomery.montgomery(j, rs[i]));
        }
    }
}


TEST(Montgomery, montgomeryMultiplication) {
    Montgomery montgomery(17);
    EXPECT_EQ(UnsignedBigInteger(2), montgomery.multiplication(15, 16));
    EXPECT_EQ(UnsignedBigInteger(8), montgomery.multiplication(100, 200));
}


TEST(Montgomery, montgomeryMultiplicationHardcore) {
    UnsignedBigInteger m, x, y, r;
    m = "95019687874943282453766626944185226110302651584829";
    x = "74419310983787348047285639088879952108680136023207";
    y = "74245211244475811723593504543727164879694968207507";
    r = "38183077291396469570854763072056246299650917983356";

    Montgomery montgomery(m);

    EXPECT_EQ(r, montgomery.multiplication(x, y));
}