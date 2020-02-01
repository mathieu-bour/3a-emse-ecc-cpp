#ifndef UNSIGNED_BIG_INTEGER_H
#define UNSIGNED_BIG_INTEGER_H

#include <cstdint>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <cctype>

using std::size_t;

namespace ecc {
    /**
     * Arbitrary-long unsigned integers.
     * Heavily inspired by fffaraz's implementation of "Arbitrary-precision integer arithmetic in C++".
     *
     * @author Mathieu Bour <mathieu.bour@etu.emse.fr>
     * @link https://gist.github.com/fffaraz/8314ad2df53c338ece0c
     */
    class UnsignedBigInteger {
    public:
        typedef std::uint32_t Digit;
        typedef std::uint64_t Digit64;
        static const unsigned BITS = 32;

        /**
         * Default constructor with a single 32-bits digit. By default, if no args were given, it is initialized to
         * zero.
         * @param digit The 32-bits digit.
         */
        UnsignedBigInteger(Digit digit = 0);

        /**
         * Default copy constructor, which basically copies the digits.
         * @param copy The copy reference.
         */
        UnsignedBigInteger(const UnsignedBigInteger &copy);

        /**
         * From string constructor.
         * This is the most important one, which allow to initialize very big numbers from a string based number in
         * base 10.
         * @param str The big number to instantiate (base 10).
         */
        UnsignedBigInteger(const std::string &str);

        /**
         * Assignment operator (`=`). Basically copies the instance digits from the other reference.
         * @param other The other reference.
         * @return this
         */
        UnsignedBigInteger &operator=(const UnsignedBigInteger &other);

        /**
         * Direct increment operator. Uses the reference increment operator.
         * @return The incremented big integer.
         */
        const UnsignedBigInteger operator++(int);

        /**
         * Reference increment operator.
         * @return The incremented big integer reference.
         */
        UnsignedBigInteger &operator++();

        /**
         * Direct decrement operator. Uses the referenece decrement operator.
         * @return The decremented big integer.
         */
        const UnsignedBigInteger operator--(int);

        /**
         * Reference decrement operator.
         * @return The decremented big integer reference.
         */
        UnsignedBigInteger &operator--();

        /**
         * Direct addition operator. Uses the addition assignment operator.
         * @param a A big integer.
         * @param b Another big integer.
         * @return The sum of a and b.
         */
        friend UnsignedBigInteger operator+(const UnsignedBigInteger &a, const UnsignedBigInteger &b);

        /**
         * Addition assignment operator.
         * @param other The other big integer to add.
         * @return The sum big integer reference.
         */
        UnsignedBigInteger &operator+=(const UnsignedBigInteger &other);

        /**
         * Direct subtraction operator. Used the subtraction assignment operator.
         * @param source
         * @param delta
         * @return The difference between the source and the delta.
         */
        friend UnsignedBigInteger operator-(const UnsignedBigInteger &source, const UnsignedBigInteger &delta);

        /**
         * Subtraction assignment operator.
         * @param delta The delta to remove from the current big integer.
         * @return The difference reference.
         */
        UnsignedBigInteger &operator-=(const UnsignedBigInteger &delta);

        /**
         * Direct multiplication operator.
         * @param a A big integer.
         * @param b Another big integer.
         * @return The product of a by b.
         */
        friend UnsignedBigInteger operator*(const UnsignedBigInteger &a, const UnsignedBigInteger &b);

        /**
         * Multiplication assignment operator
         * @param other The other big number.
         * @return The big integer product reference.
         */
        UnsignedBigInteger &operator*=(const UnsignedBigInteger &other);

        /**
         * Direct division operator. Division is achieved through the divide method.
         * @param dividend The dividend big integer.
         * @param divider The divider big integer.
         * @return The quotient big integer.
         */
        friend UnsignedBigInteger operator/(const UnsignedBigInteger &dividend, const UnsignedBigInteger &divider);

        /**
         * Division assignment operator. Division is achieved through the divide method.
         * @param divider The divider big integer.
         * @return The quotient big integer reference.
         */
        UnsignedBigInteger &operator/=(const UnsignedBigInteger &divider);

        /**
         * Direct modulo operator. Division is achieved through the divide method.
         * @param dividend The dividend big integer.
         * @param divider The divider big integer.
         * @return The reminder big integer.
         */
        friend UnsignedBigInteger operator%(const UnsignedBigInteger &dividend, const UnsignedBigInteger &divider);

        /**
         * Modulo assignment operator. Division is achieved through the divide method.
         * @param divider The divider big integer.
         * @return The reminder big integer reference.
         */
        UnsignedBigInteger &operator%=(const UnsignedBigInteger &divider);

        /**
         * Divide a bid number
         * @param divider The divider.
         * @param quotient The quotient reference.
         * @param reminder The reminder reference.
         */
        void divide(UnsignedBigInteger divider, UnsignedBigInteger &quotient, UnsignedBigInteger &reminder) const;

        /**
         * Direct left shift operator.
         * @param subject The big integer.
         * @param shiftSize The number of bits to shift.
         * @return The shifted big integer.
         */
        friend UnsignedBigInteger operator<<(const UnsignedBigInteger &subject, size_t shiftSize);

        /**
         * Left shift assignment operator.
         * @param shiftSize The number of bits to shift to the left.
         * @return The shifted big integer reference.
         */
        UnsignedBigInteger &operator<<=(size_t shiftSize);

        friend UnsignedBigInteger operator>>(const UnsignedBigInteger &u, size_t v);

        UnsignedBigInteger &operator>>=(size_t rhs);

        friend UnsignedBigInteger operator&(const UnsignedBigInteger &u, const UnsignedBigInteger &v);

        UnsignedBigInteger &operator&=(const UnsignedBigInteger &rhs);

        friend UnsignedBigInteger operator^(const UnsignedBigInteger &u, const UnsignedBigInteger &v);

        UnsignedBigInteger &operator^=(const UnsignedBigInteger &rhs);

        friend UnsignedBigInteger operator|(const UnsignedBigInteger &u, const UnsignedBigInteger &v);

        UnsignedBigInteger &operator|=(const UnsignedBigInteger &other);

        /**
         * Operator < (strictly less than). It will be used for operators >, <= and >=
         * @param a
         * @param b
         * @return true if a < b.
         */
        friend bool operator<(const UnsignedBigInteger &a, const UnsignedBigInteger &b);

        friend bool operator>(const UnsignedBigInteger &u, const UnsignedBigInteger &v);

        friend bool operator<=(const UnsignedBigInteger &u, const UnsignedBigInteger &v);

        friend bool operator>=(const UnsignedBigInteger &u, const UnsignedBigInteger &v);

        friend bool operator==(const UnsignedBigInteger &u, const UnsignedBigInteger &v);

        friend bool operator!=(const UnsignedBigInteger &u, const UnsignedBigInteger &v);

        /**
         * @return the string representation of the number in base 10.
         */
        [[nodiscard]] std::string to_string() const;

        friend std::ostream &operator<<(std::ostream &os, const UnsignedBigInteger &u);

        /**
         * Convert an input string in base 10 to an UnsignedBigInteger
         * @param inputStream
         * @param result
         * @return the input stream
         */
        friend std::istream &operator>>(std::istream &inputStream, UnsignedBigInteger &result);

    private:
        /**
         * The unsigned big integer digits. Each is a 32-bits unsigned integer and may support up to 2^32 values.
         * Thus, the size of `digits` is optimized. The first "digit" is the lowest-order bits.
         */
        std::vector<Digit> digits;

        /**
         * Drop leading zeros of the digits (remove the digits equal to zeros, starting from the back of the vector).
         */
        void trim();
    };
} // namespace ECC

#endif // UNSIGNED_BIG_INTEGER_H