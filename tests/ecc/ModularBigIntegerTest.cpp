#include "gtest/gtest.h"
#include "../../includes/ecc/SignedBigInteger.h"
#include "../../includes/ecc/ModularBigInteger.h"

using ecc::UnsignedBigInteger;
using ecc::SignedBigInteger;
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


TEST(ModularBigInteger, computeMontgomeryParams) {
    ModularBigInteger a("12", "23");
    a.computeMontgomeryParams();

    EXPECT_EQ(UnsignedBigInteger("32"), a.r);
    EXPECT_EQ(UnsignedBigInteger("1"), a.r * a.invR - a.modulus * a.invModulus);
}


TEST(ModularBigInteger, montgomeryReduce) {
    ModularBigInteger a("12", "23");
    UnsignedBigInteger ap("16");
    EXPECT_EQ(ap, a.montgomeryReduce());


    ModularBigInteger b("17", "23");
    UnsignedBigInteger bp("15");
    EXPECT_EQ(bp, b.montgomeryReduce());

    ModularBigInteger c("3", "5");
    UnsignedBigInteger cp("4");
    EXPECT_EQ(cp, c.montgomeryReduce());
}


TEST(ModularBigInteger, multiplication) {
    ModularBigInteger a("3", "5");
    ModularBigInteger b("3", "5");
    ModularBigInteger c = a * b;

    EXPECT_EQ(UnsignedBigInteger("9"), c.value);
}