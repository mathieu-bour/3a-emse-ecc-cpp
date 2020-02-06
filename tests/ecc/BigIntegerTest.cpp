#include "gtest/gtest.h"
#include "../../src/ecc/BigInteger.h"

using ecc::BigInteger;

TEST(BigIntegerTest, addition) {
    BigInteger a, b, c;
    a = "12";
    b = "13";
    c = "25";
    EXPECT_EQ(c, a + b);

    a = "15";
    b = "-25";
    c = "-10";
    EXPECT_EQ(c, a + b);

    a = "0";
    b = "1";
    c = "1";
    EXPECT_EQ(c, a + b);

    a = "52069158701248348990665441625155476344422";
    b = "8976025279197247334376783355988518174544216699351";
    c = "8976025331266406035625132346653959799699693043773";
    EXPECT_EQ(c, a + b);

    a = "52069159197247334376783301248348990";
    b = "-8976066544162515559888754763444222527518174544216699351";
    c = "-8976066544162515559836685604246975193141391242968350361";
    EXPECT_EQ(c, a + b);
}

TEST(BigIntegerTest, subtraction) {
    BigInteger a, b, c;
    a = "12";
    b = "18";
    c = "-6";
    EXPECT_EQ(c, a - b);

    a = "12";
    b = "-45";
    c = "57";
    EXPECT_EQ(c, a - b);

    a = "-97517531365945353460500986231467501975566488681897";
    b = "30165755786462904037747915978978091902166141872378";
    c = "-127683287152408257498248902210445593877732630554275";
    EXPECT_EQ(c, a - b);
}

TEST(BigInteger, euclidean) {
    BigInteger one(1);
    BigInteger a, b, x, y, gcd;
    a = "13";
    b = "17";

    gcd = BigInteger::euclidean(a, b, x, y);
    EXPECT_EQ(one, gcd);
    EXPECT_EQ(one, a * x + b * y);
}