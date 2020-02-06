#ifndef INC_3A_ECC_CPP_UNSIGNEDBIGINTEGER_H
#define INC_3A_ECC_CPP_UNSIGNEDBIGINTEGER_H

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>
#include <vector>
#include "ECCTypes.h"

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
        static const unsigned BITS = 32;

        /**
         * The unsigned big integer digits. Each is a 32-bits unsigned integer and may support up to 2^32 values.
         * Thus, the size of `digits` is optimized. The first "digit" is the lowest-order bits.
         */
        Digits digits;


        /**
         * Default constructor with a single 32-bits digit. By default, if no args were given, it is initialized to
         * zero.
         * @param digit The 32-bits digit.
         */
        UnsignedBigInteger(Digit pDigit = 0);


        /**
         * Construct from an existing vector.
         * @param pDigits The 32-bit digits vector.
         */
        UnsignedBigInteger(Digits pDigits);


        /**
         * From string constructor.
         * This is the most important one, which allow to initialize very big integers from a string based number in
         * base 10.
         * @param str The big integer to instantiate (base 10).
         */
        UnsignedBigInteger(const std::string &str);


        /**
         * Default copy constructor, which basically copies the digits.
         * @param copy The copy reference.
         */
        UnsignedBigInteger(const UnsignedBigInteger &copy) = default;


        /**
         * Assignment operator. Basically copies the instance digits from the other reference.
         * @param other The other reference.
         * @return this
         */
        UnsignedBigInteger &operator=(const UnsignedBigInteger &other) = default;


        /**
         * Assignment from a unsigned 32-bit integer, which allow easier initialization.
         * @param str The unsigned 32-bit integer.
         * @return this
         */
        UnsignedBigInteger &operator=(Digit pDigit);


        /**
         * Assignment from a string, which allow easier initialization.
         * @param str The string representing the number in base 10.
         * @return this
         */
        UnsignedBigInteger &operator=(const std::string &str);


        /**
         * Equality operator. The big integers are considered equal if their digits are the same.
         * @param other Another big integer.
         * @return if a's digits are the same as b's ones.
         */
        bool operator==(const UnsignedBigInteger &other) const;


        /**
         * Not equal operator. Uses the equality operator.
         * @param b Another big integer.
         * @return if a's digits are not the same as b's ones.
         */
        bool operator!=(const UnsignedBigInteger &other) const;


        /**
         * Operator < (strictly less than). It will be used for operators >, <= and >=
         * @param other Another big integer.
         * @return if the other big integer is bigger than the current one.
         */
        bool operator<(const UnsignedBigInteger &other) const;


        /**
         * Operator > (strictly more than). Uses the strictly less than operator.
         * @param a A big integer.
         * @param other Another big integer.
         * @return true if a > b.
         */
        bool operator>(const UnsignedBigInteger &other) const;


        /**
         * Operator <= (more than or equals). Uses the strictly less than operator.
         * @param a A big integer.
         * @param other Another big integer.
         * @return true if a <= b.
         */
        bool operator<=(const UnsignedBigInteger &other) const;


        /**
         * Operator >= (more than or equals). Uses the strictly less than operator.
         * @param a A big integer.
         * @param other Another big integer.
         * @return true if a >= b.
         */
        bool operator>=(const UnsignedBigInteger &other) const;


        /**
         * Pre-increment operator.
         * @return The incremented big integer reference.
         */
        UnsignedBigInteger &operator++();


        /**
         * Post-increment operator. Uses the pre-increment operator.
         * @return The incremented big integer.
         */
        const UnsignedBigInteger operator++(int);


        /**
         * Pre-decrement operator.
         * @return The decremented big integer reference.
         */
        UnsignedBigInteger &operator--();


        /**
         * Post-decrement operator. Uses the pre-increment operator.
         * @return The decremented big integer.
         */
        const UnsignedBigInteger operator--(int);


        /**
         * Addition operator. Uses the addition assignment operator.
         * @param a A big integer.
         * @param other Another big integer.
         * @return The sum of a and b.
         */
        UnsignedBigInteger operator+(const UnsignedBigInteger &other) const;


        /**
         * Addition assignment operator.
         * @param other The other big integer to add.
         * @return The sum big integer reference.
         */
        UnsignedBigInteger &operator+=(const UnsignedBigInteger &other);


        /**
         * Subtraction operator. Uses the subtraction assignment operator.
         * @param source
         * @param delta
         * @return The difference between the source and the delta.
         */
        UnsignedBigInteger operator-(const UnsignedBigInteger &delta) const;


        /**
         * Subtraction assignment operator.
         * @param delta The delta to remove from the current big integer.
         * @return The difference reference.
         */
        UnsignedBigInteger &operator-=(const UnsignedBigInteger &delta);


        /**
         * Multiplication operator.
         * @param a A big integer.
         * @param other Another big integer.
         * @return The product of a by b.
         */
        UnsignedBigInteger operator*(const UnsignedBigInteger &other) const;


        /**
         * Multiplication assignment operator
         * @param other The other big integer.
         * @return The big integer product reference.
         */
        UnsignedBigInteger &operator*=(const UnsignedBigInteger &other);


        /**
         * Division operator. Division is achieved through the divide method.
         * @param dividend The dividend big integer.
         * @param divider The divider big integer.
         * @return The quotient big integer.
         */
        UnsignedBigInteger operator/(const UnsignedBigInteger &divider) const;


        /**
         * Division assignment operator. Division is achieved through the divide method.
         * @param divider The divider big integer.
         * @return The quotient big integer reference.
         */
        UnsignedBigInteger &operator/=(const UnsignedBigInteger &divider);


        /**
         * Modulo operator. Division is achieved through the divide method.
         * @param divider The divider big integer.
         * @return The reminder big integer.
         */
        UnsignedBigInteger operator%(const UnsignedBigInteger &divider) const;


        /**
         * Modulo assignment operator. Division is achieved through the divide method.
         * @param divider The divider big integer.
         * @return The reminder big integer reference.
         */
        UnsignedBigInteger &operator%=(const UnsignedBigInteger &divider);


        /**
         * Left shift operator.
         * @param shiftSize The number of bits to shift to the left.
         * @return The shifted big integer.
         */
        UnsignedBigInteger operator<<(size_t shiftSize) const;


        /**
         * Left shift assignment operator.
         * @param shiftSize The number of bits to shift to the left.
         * @return The shifted big integer reference.
         */
        UnsignedBigInteger &operator<<=(size_t shiftSize);


        /**
         * Right shift operator.
         * @param shiftSize The number of bits to shift to the right.
         * @return The shifted big integer.
         */
        UnsignedBigInteger operator>>(size_t shiftSize) const;


        /**
         * Right shift assignment operator.
         * @param shiftSize The number of bits to shift to the right.
         * @return The shifted big integer reference.
         */
        UnsignedBigInteger &operator>>=(size_t shiftSize);


        /**
         * Bitwise AND operator. Uses the bitwise AND assignment operator.
         * @param a A big integer.
         * @param b Another big integer.
         * @return a AND b.
         */
        friend UnsignedBigInteger operator&(const UnsignedBigInteger &a, const UnsignedBigInteger &b);


        /**
         * Bitwise AND assignment operator
         * @param other The other big integer.
         * @return this AND other
         */
        UnsignedBigInteger &operator&=(const UnsignedBigInteger &other);


        /**
         * Bitwise OR operator. Uses the bitwise OR assignment operator.
         * @param other Another big integer.
         * @return this OR other.
         */
        UnsignedBigInteger operator|(const UnsignedBigInteger &other) const;


        /**
         * Bitwise OR assignment operator
         * @param other The other big integer.
         * @return this OR other
         */
        UnsignedBigInteger &operator|=(const UnsignedBigInteger &other);


        /**
         * Bitwise XOR operator. Uses the bitwise XOR assignment operator.
         * @param other Another big integer.
         * @return this XOR other.
         */
        UnsignedBigInteger operator^(const UnsignedBigInteger &other) const;


        /**
         * Bitwise XOR assignment operator.
         * @param other
         * @return this XOR other
         */
        UnsignedBigInteger &operator^=(const UnsignedBigInteger &other);


        /**
         * Extraction operator. For now, only base 10 digit strings are supported.
         * @param inputStream The input stream.
         * @param result The parsed big integer.
         * @return The input stream.
         */
        friend std::istream &operator>>(std::istream &inputStream, UnsignedBigInteger &result);


        /**
         * Insertion operator. Basically convert the big integer to a base 10 representation.
         * @param outputStream The output stream.
         * @param source The source big integer.
         * @return The output stream.
         */
        friend std::ostream &operator<<(std::ostream &outputStream, const UnsignedBigInteger &source);


        /**
         * Get the most significant bit of the Modular Big Integer
         * @return
         */
        size_t getMostSignificantBitIndex() const;


        /**
         * Get the nth bit of the representation of the big integer.
         * @param bitIndex The index of the desired bit.
         * @return
         */
        uint8_t getBit(size_t bitIndex);


        /**
         * @return the string representation of the number in base 10.
         */
        std::string to_string() const;


    protected:
        /**
         * Divide a big integer, and assign the quotient and the reminder to their respective references.
         * @param divider The divider.
         * @param quotient The quotient reference.
         * @param reminder The reminder reference.
         */
        void divide(UnsignedBigInteger divider, UnsignedBigInteger &quotient, UnsignedBigInteger &reminder) const;


        /**
         * Drop leading zeros of the digits (e.g. remove the digits equal to zeros, starting from the back of the vector).
         */
        void trim();
    };
}
#endif //INC_3A_ECC_CPP_UNSIGNEDBIGINTEGER_H