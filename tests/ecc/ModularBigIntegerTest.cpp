#include "gtest/gtest.h"
#include "../../src/ecc/BigInteger.h"
#include "../../src/ecc/ModularBigInteger.h"

using ecc::UnsignedBigInteger;
using ecc::BigInteger;
using ecc::ModularBigInteger;


TEST(ModularBigInteger, addition) {
    ModularBigInteger a("12", "23");
    ModularBigInteger b("14", "23");
    ModularBigInteger c("3", "23");
    EXPECT_EQ(c, a + b);

    a = ModularBigInteger("9538015219187306872831871770784093765052242936573",
                          "16589398644410362140098972598872168730834157521659");
    b = ModularBigInteger("9787826873359737738574143828680880048170382224558",
                          "16589398644410362140098972598872168730834157521659");
    c = ModularBigInteger("2736443448136682471307043000592805082388467639472",
                          "16589398644410362140098972598872168730834157521659");
    EXPECT_EQ(c, a + b);
}

TEST(ModularBigInteger, subtraction) {
    ModularBigInteger a("12", "23");
    ModularBigInteger b("14", "23");
    ModularBigInteger c("21", "23");
    EXPECT_EQ(c, a - b);

    a = ModularBigInteger("9538015219187306872831871770784093765052242936573",
                          "16589398644410362140098972598872168730834157521659");
    b = ModularBigInteger("9787826873359737738574143828680880048170382224558",
                          "16589398644410362140098972598872168730834157521659");
    c = ModularBigInteger("16339586990237931274356700540975382447716018233674",
                          "16589398644410362140098972598872168730834157521659");
    EXPECT_EQ(c, a - b);
}