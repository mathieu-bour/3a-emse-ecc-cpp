#include "../../includes/ecc/Point.h"

using namespace ecc;

Point::Point(
        const UnsignedBigInteger &pX,
        const UnsignedBigInteger &pY,
        const UnsignedBigInteger &pZ,
        const UnsignedBigInteger &pA,
        const UnsignedBigInteger &pB,
        const UnsignedBigInteger &pM
) {
    m = pM;
    x = ModularBigInteger(pX, pM);
    y = ModularBigInteger(pX, pM);
    z = ModularBigInteger(pX, pM);
    a = ModularBigInteger(pA, pM);
    b = ModularBigInteger(pB, pM);
}


bool Point::isZero() const {
    return x.value == 0;
}

Point Point::factory(const ModularBigInteger &pX, const ModularBigInteger &pY, const ModularBigInteger &pZ) const {
    Point build;
    build.x = pX;
    build.y = pY;
    build.z = pZ;
    build.a = a;
    build.b = b;
    build.m = m;
    return build;
}

Point Point::twice() const {
    if (isZero() || y.value == 0) {
        return Point(0, 0, 0, a.value, b.value, m);
    }

    ModularBigInteger two(2, m);
    ModularBigInteger three(3, m);

    ModularBigInteger t = x * x * three + a * z * z;
    ModularBigInteger u = y * z * two;
    ModularBigInteger v = u * x * y * two;
    ModularBigInteger w = t * t - v * two;
    ModularBigInteger rx = u * w;
    ModularBigInteger ry = t * (v - w) - u * u * y * y * two;
    ModularBigInteger rz = u * u * u;

    return factory(rx, ry, rz);
}

Point Point::operator+(const Point &other) const {
    Point sum(*this);
    sum += other;
    return sum;
}

Point &Point::operator+=(const Point &other) {
    if (other.isZero()) {
        return *this;
    } else if (isZero()) {
        *this = other;
        return *this;
    }

    ModularBigInteger t0 = y * other.z;
    ModularBigInteger t1 = other.y * z;
    ModularBigInteger u0 = x * other.z;
    ModularBigInteger u1 = other.x * z;


    if (u0 == u1) {
        if (t0 == t1) {
            // x1*z2 = x2*z1 and y1*z2=y2*z1 -> the Points are the same, simply double
            *this = twice();
            return *this;
        } else {
            *this = Point(0, 0, 0, a.value, b.value, m);
            return *this;
        }
    }

    // Finish the computation of the projective variables
    ModularBigInteger t = t0 - t1;
    ModularBigInteger u = u0 - u1;
    ModularBigInteger u2 = u * u;
    ModularBigInteger v = z * other.z;
    ModularBigInteger w = t * t * v - u2 * (u0 + u1);
    ModularBigInteger u3 = u * u2;
    ModularBigInteger rx = u * w;
    ModularBigInteger ry = t * (u0 * u2 - w) - t0 * u3;
    ModularBigInteger rz = u3 * v;
    *this = factory(rx, ry, rz);
    return *this;
}

bool Point::operator==(const Point &other) const {
    if (isZero() || other.isZero()) {
        return isZero() and other.isZero();
    }

    return (x * other.z == other.x * z)
           && (y * other.z == other.x * z)
           && a == other.a
           && b == other.b
           && m == other.m;
}

bool Point::operator!=(const Point &other) const {
    return !(other == *this);
}

bool Point::isOnCurve() const {
    return !isZero() && y * y * z == x * x * x + a * x * z * z + b * z * z * z;
}

Point Point::operator-(const Point &other) const {
    Point difference(*this);
    difference -= other;
    return difference;
}

Point &Point::operator-=(const Point &other) {
    // Negate "other"
    Point otherCopy(other);
    otherCopy.y.value = m - other.y.value;

    *this += otherCopy;
    return *this;
}

Point Point::operator*(const UnsignedBigInteger &other) const {
    Point product(*this);
    product *= other;
    return product;
}

Point &Point::operator*=(const UnsignedBigInteger &p) {
    UnsignedBigInteger n(p);
    Point result;
    Point temp(*this);
    UnsignedBigInteger one(1);

    while (n != 0) {
        UnsignedBigInteger nAndOne = n & one;
        if (nAndOne != one) {
            result += temp;
        }

        temp = temp.twice();
        n >>= 1;
    }

    *this = result;
    return *this;
}
