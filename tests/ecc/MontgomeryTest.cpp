#include "gtest/gtest.h"
#include "../../includes/ecc/UnsignedBigInteger.h"
#include "../../includes/ecc/Montgomery.h"

using ecc::UnsignedBigInteger;
using ecc::Montgomery;

TEST(Montgomery, reduceWiki) {
    UnsignedBigInteger primes[] = {17, 23, 53};

    for (size_t i = 0; i < 3; i++) {
        Montgomery montgomery(primes[i]);

        for (UnsignedBigInteger j(0); j < primes[i]; j++) {
            EXPECT_EQ(montgomery.reduceRef(j), montgomery.reduceWiki(j));
        }
    }
}


TEST(Montgomery, montgomeryMultiplication) {
    Montgomery montgomery(17);
    EXPECT_EQ(UnsignedBigInteger(2), montgomery.multiply(15, 16));
    EXPECT_EQ(UnsignedBigInteger(8), montgomery.multiply(100, 200));
}


TEST(Montgomery, montgomeryMultiplicationHardcore) {
    UnsignedBigInteger m, x, y, r;
    m = "95019687874943282453766626944185226110302651584829";
    x = "74419310983787348047285639088879952108680136023207";
    y = "74245211244475811723593504543727164879694968207507";
    r = "38183077291396469570854763072056246299650917983356";

    Montgomery montgomery(m);

    EXPECT_EQ(r, montgomery.multiply(x, y));
}