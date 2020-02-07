
#ifndef INC_3A_ECC_CPP_POINT_H
#define INC_3A_ECC_CPP_POINT_H

#include "UnsignedBigInteger.h"
#include "ModularBigInteger.h"

namespace ecc {
    class Point {
    public:
        UnsignedBigInteger m;
        ModularBigInteger x;
        ModularBigInteger y;
        ModularBigInteger z;
        ModularBigInteger a;
        ModularBigInteger b;

        Point(
                const UnsignedBigInteger &pX = 0,
                const UnsignedBigInteger &pY = 0,
                const UnsignedBigInteger &pZ = 0,
                const UnsignedBigInteger &pA = 0,
                const UnsignedBigInteger &pB = 0,
                const UnsignedBigInteger &pM = 1
        );

        Point factory(const ModularBigInteger &pX, const ModularBigInteger &pY, const ModularBigInteger &pZ) const;

        bool isZero() const;

        bool isOnCurve() const;

        Point twice() const;

        bool operator==(const Point &rhs) const;

        bool operator!=(const Point &other) const;

        Point &operator=(const Point &other) = default;

        Point operator+(const Point &other) const;

        Point &operator+=(const Point &other);

        Point operator-(const Point &other) const;

        Point &operator-=(const Point &other);

        Point operator*(const UnsignedBigInteger &other) const;

        Point &operator*=(const UnsignedBigInteger &other);
    };
}


#endif //INC_3A_ECC_CPP_POINT_H
