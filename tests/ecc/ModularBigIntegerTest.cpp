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

TEST(ModularBigInteger, multplication) {
    ModularBigInteger a("15", "101");
    ModularBigInteger b("78", "101");
    ModularBigInteger c("59", "101");
//    EXPECT_EQ(c, a * b);
//
//    a.modulus = b.modulus = c.modulus = "48302719417429676373373082596797047824218928789657";
//    a.value = "8336372878937308259679704782421890271941742967657";
//    b.value = "3984889828386848004809373789390397393937938937903";
//    c.value = "41595515557066660636620096868365847727884034972595";
//    EXPECT_EQ(c, a * b);

    a.modulus = b.modulus = c.modulus = "10782148755896283475495548862133433546436570882721";
    a.value = "8730825963637287893941742967657970478242189027137";
    b.value = "3984832838684800479389379089880937378939039739393";
    c.value = "3027429495029888573836989950937185745280723518817";
    EXPECT_EQ(c, a * b);
}
