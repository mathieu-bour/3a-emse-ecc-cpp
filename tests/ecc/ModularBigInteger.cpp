#include "gtest/gtest.h"
#include "../../src/ecc/ModularBigInteger.h"

using ecc::ModularBigInteger;

TEST(ModularBigInteger, addition) {
    ModularBigInteger a("12", "23");
    ModularBigInteger b("14", "23");
    ModularBigInteger c("16", "23");

    EXPECT_EQ(c, a + b);
}