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
        UnsignedBigInteger(Digit digit = 0) : digits(1, digit) {}

        UnsignedBigInteger(Digits pDigits) : digits(std::move(pDigits)) {}

        /**
         * Default copy constructor, which basically copies the digits.
         * @param copy The copy reference.
         */
        UnsignedBigInteger(const UnsignedBigInteger &copy) : digits(copy.digits) {
            // empty
        }

        /**
         * From string constructor.
         * This is the most important one, which allow to initialize very big numbers from a string based number in
         * base 10.
         * @param str The big number to instantiate (base 10).
         */
        UnsignedBigInteger(const std::string &str) : digits(1, 0) {
            std::istringstream inputStringStream(str);

            inputStringStream >> *this; // see std::istream &operator>>

            if (inputStringStream.fail() || !inputStringStream.eof()) {
                throw std::runtime_error("Error: UnsignedBigInteger::string");
            }
        }

        /**
         * Assignment operator (`=`). Basically copies the instance digits from the other reference.
         * @param other The other reference.
         * @return this
         */
        UnsignedBigInteger &operator=(const UnsignedBigInteger &other) = default;

        UnsignedBigInteger &operator=(const std::string &str) {
            *this = UnsignedBigInteger(str);
            return *this;
        };

        /**
         * Direct increment operator. Uses the reference increment operator.
         * @return The incremented big integer.
         */
        const UnsignedBigInteger operator++(int) {
            UnsignedBigInteger that(*this);
            ++(*this);
            return that;
        }

        /**
         * Reference increment operator.
         * @return The incremented big integer reference.
         */
        UnsignedBigInteger &operator++() {
            /*
             * Increment strategy:
             * Take the first digit and increment it. If the digit is now equal to zero, it means that it the digit
             * overflowed and that we need to increment the second digit. Continue until reaching the end of the vector.
             */
            for (size_t digitIdx = 0; digitIdx < digits.size() && ++digits[digitIdx] == 0; ++digitIdx);

            if (digits.back() == 0) {
                // If the last digit is equal to zero, it means that we need to push a 1 to the end (like 999 => 1000).
                digits.push_back(1);
            }

            return *this;
        }

        /**
         * Direct decrement operator. Uses the referenece decrement operator.
         * @return The decremented big integer.
         */
        const UnsignedBigInteger operator--(int) {
            UnsignedBigInteger w(*this);
            --(*this);
            return w;
        }

        /**
         * Reference decrement operator.
         * @return The decremented big integer reference.
         */
        UnsignedBigInteger &operator--() {
            if (digits.back() == 0) {
                // zero may not be decremented
                throw std::underflow_error("Error: UnsignedBigInteger: decrement underflow");
            }

            /*
             * Increment strategy:
             * Take the first digit and decrement it. If the digit is was equal to zero before decrement, it means that
             * it will underflow that we need to decrement the second digit. Continue until reaching the end of the
             * vector.
             */
            for (size_t digitIdx = 0; digitIdx < digits.size() && digits[digitIdx]-- == 0; ++digitIdx);

            trim(); // Decrement may introduce leading zeros (like 1000-- => 0999).

            return *this;
        }

        /**
         * Direct addition operator. Uses the addition assignment operator.
         * @param a A big integer.
         * @param b Another big integer.
         * @return The sum of a and b.
         */
        friend UnsignedBigInteger operator+(const UnsignedBigInteger &a, const UnsignedBigInteger &b) {
            UnsignedBigInteger sum(a);
            sum += b;
            return sum;
        }

        /**
         * Addition assignment operator.
         * @param other The other big integer to add.
         * @return The sum big integer reference.
         */
        UnsignedBigInteger &operator+=(const UnsignedBigInteger &other) {
            const size_t otherSize = other.digits.size();

            if (digits.size() < otherSize) {
                /*
                 * Current number has a lower number of digits than the other big integer and thus its digits vector
                 * has to be resized.
                 */
                digits.resize(otherSize, 0);
            }

            size_t j = 0;
            Digit64 carry = 0; // Since the digits size are 32 bits, we are working with 64 bits reminders

            for (; j < otherSize; ++j) {
                carry = carry + digits[j] + other.digits[j];
                digits[j] = static_cast<Digit>(carry); // The static cast will only take the lowest 32-bits
                carry >>= BITS; // Take the highest 32-bits to add at the next iteration
            }

            /*
             * j is now equal to the number of digits of the other big integer.
             * If the current number of digits is greater than j, we will continue to add the carry against the
             * following digits.
             */
            for (; carry != 0 && j < digits.size(); ++j) {
                carry += digits[j];
                digits[j] = static_cast<Digit>(carry);
                carry >>= BITS;
            }

            if (carry != 0) {
                // Reminder was not null, add a leading unit.
                digits.push_back(1);
            }

            return *this;
        }

        /**
         * Direct subtraction operator. Used the subtraction assignment operator.
         * @param source
         * @param delta
         * @return The difference between the source and the delta.
         */
        friend UnsignedBigInteger operator-(const UnsignedBigInteger &source, const UnsignedBigInteger &delta) {
            UnsignedBigInteger difference(source);
            difference -= delta;
            return difference;
        }

        /**
         * Subtraction assignment operator.
         * @param delta The delta to remove from the current big integer.
         * @return The difference reference.
         */
        UnsignedBigInteger &operator-=(const UnsignedBigInteger &delta) {
            if ((*this) < delta) {
                // We may not differentiate a greater number.
                throw std::underflow_error("Error: UnsignedBigInteger: subtraction underflow");
            }

            size_t deltaSize = 0;
            Digit64 carry = 0;

            /*
             * We are now guaranteed that the current big integer has a least the same number of digits of the delta.
             */
            for (; deltaSize < delta.digits.size(); ++deltaSize) {
                carry = carry + digits[deltaSize] - delta.digits[deltaSize];
                digits[deltaSize] = static_cast<Digit>(carry);
                carry = ((carry >> BITS) ? -1 : 0); // -1 means "maximum 32 bits unsigned integer"
            }

            /*
             * For each additional digit in the current big integer, apply the carry, if it is not null.
             */
            for (; carry != 0 && deltaSize < digits.size(); ++deltaSize) {
                carry += digits[deltaSize];
                digits[deltaSize] = static_cast<Digit>(carry);
                carry = ((carry >> BITS) ? -1 : 0);
            }

            trim(); // subtraction may introduce leading zeros, remove then

            return *this;
        }

        /**
         * Direct multiplication operator.
         * @param a A big integer.
         * @param b Another big integer.
         * @return The product of a by b.
         */
        friend UnsignedBigInteger operator*(const UnsignedBigInteger &a, const UnsignedBigInteger &b) {
            const size_t aSize = a.digits.size();
            const size_t bSize = b.digits.size();

            UnsignedBigInteger result; // result is initialized to zero

            // The maximum number of digits is aSize + bSize (like 999*999=998001 -> 6 digits)
            result.digits.resize(aSize + bSize, 0);

            for (size_t bDigitIdx = 0; bDigitIdx < bSize; ++bDigitIdx) {
                Digit64 carry = 0;

                for (size_t aDigitIdx = 0; aDigitIdx < aSize; ++aDigitIdx) {
                    carry += static_cast<Digit64>(a.digits[aDigitIdx]) * b.digits[bDigitIdx] // on a 64-bits
                             + result.digits[aDigitIdx + bDigitIdx];

                    // The static cast will only take the lowest 32-bits
                    result.digits[aDigitIdx + bDigitIdx] = static_cast<Digit>(carry);

                    carry >>= BITS; // Reminder
                }

                result.digits[bDigitIdx + aSize] = static_cast<Digit>(carry);
            }

            result.trim(); // result was provisioned using the maximum digits

            return result;
        }

        /**
         * Multiplication assignment operator
         * @param other The other big number.
         * @return The big integer product reference.
         */
        UnsignedBigInteger &operator*=(const UnsignedBigInteger &other) {
            *this = (*this) * other;
            return *this;
        }

        /**
         * Direct division operator. Division is achieved through the divide method.
         * @param dividend The dividend big integer.
         * @param divider The divider big integer.
         * @return The quotient big integer.
         */
        friend UnsignedBigInteger operator/(const UnsignedBigInteger &dividend, const UnsignedBigInteger &divider) {
            UnsignedBigInteger quotient, reminder;
            dividend.divide(divider, quotient, reminder); // We do not care of the reminder
            return quotient;
        }

        /**
         * Division assignment operator. Division is achieved through the divide method.
         * @param divider The divider big integer.
         * @return The quotient big integer reference.
         */
        UnsignedBigInteger &operator/=(const UnsignedBigInteger &divider) {
            UnsignedBigInteger reminder;
            divide(divider, *this, reminder); // We do not care of the reminder
            return *this;
        }

        /**
         * Direct modulo operator. Division is achieved through the divide method.
         * @param dividend The dividend big integer.
         * @param divider The divider big integer.
         * @return The reminder big integer.
         */
        friend UnsignedBigInteger operator%(const UnsignedBigInteger &dividend, const UnsignedBigInteger &divider) {
            UnsignedBigInteger quotient, reminder;
            dividend.divide(divider, quotient, reminder); // We do not care of the quotient
            return reminder;
        }

        /**
         * Modulo assignment operator. Division is achieved through the divide method.
         * @param divider The divider big integer.
         * @return The reminder big integer reference.
         */
        UnsignedBigInteger &operator%=(const UnsignedBigInteger &divider) {
            UnsignedBigInteger quotient;
            divide(divider, quotient, *this); // We do not care of the quotient
            return *this;
        }

        /**
         * Divide a bid number
         * @param divider The divider.
         * @param quotient The quotient reference.
         * @param reminder The reminder reference.
         */
        void divide(UnsignedBigInteger divider, UnsignedBigInteger &quotient, UnsignedBigInteger &reminder) const {
            if (divider.digits.back() == 0) {
                // Division by zero
                throw std::overflow_error("Error: UnsignedBigInteger: division by zero overflow");
            }

            reminder.digits = digits;
            const size_t n = divider.digits.size();

            // Divider is larger in term of digits, return zero (reminder was already set).
            if (digits.size() < n) {
                quotient.digits.assign(1, 0);
                return;
            }

            // Normalize divisor (divider[n-1] >= BASE/2).
            unsigned d = BITS;
            for (Digit vn = divider.digits.back(); vn != 0; vn >>= 1, --d);
            divider <<= d;
            reminder <<= d;
            const Digit vn = divider.digits.back();

            // Ensure first single-digit quotient (u[m-1] < divider[n-1]).
            reminder.digits.push_back(0);
            const size_t m = reminder.digits.size();
            quotient.digits.resize(m - n);

            UnsignedBigInteger w;
            w.digits.resize(n + 1);
            const Digit64 MAX_DIGIT = (static_cast<Digit64>(1) << BITS) - 1;
            for (size_t j = m - n; j-- != 0;) {
                // Estimate quotient digit.
                Digit64 qhat = std::min(MAX_DIGIT,
                                        (static_cast<Digit64>(reminder.digits[j + n]) << BITS |
                                         reminder.digits[j + n - 1]) / vn);

                // Compute partial product (w = qhat * divider).
                Digit64 k = 0;
                for (size_t i = 0; i < n; ++i) {
                    k += qhat * divider.digits[i];
                    w.digits[i] = static_cast<Digit>(k);
                    k >>= BITS;
                }
                w.digits[n] = static_cast<Digit>(k);

                // Check if qhat is too large (u - w < 0).
                bool is_trial = true;
                while (is_trial) {
                    size_t i = n;
                    for (; i != 0 && reminder.digits[j + i] == w.digits[i]; --i);
                    if ((is_trial = (reminder.digits[j + i] < w.digits[i]))) {
                        // Adjust partial product (w -= divider).
                        --qhat;
                        k = 0;
                        for (size_t i = 0; i < n; ++i) {
                            k = k + w.digits[i] - divider.digits[i];
                            w.digits[i] = static_cast<Digit>(k);
                            k = ((k >> BITS) ? -1 : 0);
                        }
                        w.digits[n] = static_cast<Digit>(k + w.digits[n]);
                    }
                }
                quotient.digits[j] = static_cast<Digit>(qhat);

                // Compute partial remainder (u -= w).
                k = 0;
                for (size_t i = 0; i < n; ++i) {
                    k = k + reminder.digits[j + i] - w.digits[i];
                    reminder.digits[j + i] = static_cast<Digit>(k);
                    k = ((k >> BITS) ? -1 : 0);
                }
            }

            // Denormalize reminder.
            quotient.trim();
            reminder.digits.resize(n);
            reminder >>= d;
        }

        /**
         * Direct left shift operator.
         * @param subject The big integer.
         * @param shiftSize The number of bits to shift to the left.
         * @return The shifted big integer.
         */
        friend UnsignedBigInteger operator<<(const UnsignedBigInteger &subject, size_t shiftSize) {
            UnsignedBigInteger w(subject);
            w <<= shiftSize;
            return w;
        }

        /**
         * Left shift assignment operator.
         * @param shiftSize The number of bits to shift to the left.
         * @return The shifted big integer reference.
         */
        UnsignedBigInteger &operator<<=(size_t shiftSize) {
            // Left-shifting zero has no effect
            if (digits.back() != 0 && shiftSize != 0) {
                const size_t n = shiftSize / BITS;
                digits.insert(digits.begin(), n, 0); // Insert zeros for every 32-bits shifts

                // Propagate the shift trough all digits
                shiftSize -= n * BITS;
                Digit64 carry = 0;

                for (size_t j = n; j < digits.size(); ++j) {
                    carry |= static_cast<Digit64>(digits[j]) << shiftSize;
                    digits[j] = static_cast<Digit>(carry);
                    carry >>= BITS;
                }

                if (carry != 0) {
                    digits.push_back(static_cast<Digit>(carry));
                }
            }

            return *this;
        }

        /**
         * Direct right shift operator.
         * @param subject The big integer.
         * @param shiftSize The number of bits to shift to the right.
         * @return The shifted big integer.
         */
        friend UnsignedBigInteger operator>>(const UnsignedBigInteger &subject, size_t shiftSize) {
            UnsignedBigInteger result(subject);
            result >>= shiftSize;
            return result;
        }


        /**
         * Right shift assignment operator.
         * @param shiftSize The number of bits to shift to the right.
         * @return The shifted big integer reference.
         */
        UnsignedBigInteger &operator>>=(size_t shiftSize) {
            const size_t n = shiftSize / BITS;
            if (n >= digits.size()) {
                digits.assign(1, 0);
            } else {
                digits.erase(digits.begin(), digits.begin() + n);
                shiftSize -= n * BITS;
                Digit64 k = 0;
                for (size_t j = digits.size(); j-- != 0;) {
                    k = k << BITS | digits[j];
                    digits[j] = static_cast<Digit>(k >> shiftSize);
                    k = static_cast<Digit>(k);
                }
                trim();
            }
            return *this;
        }

        /**
         * Bitwise and operator.
         * @param a
         * @param b
         * @return
         */
        friend UnsignedBigInteger operator&(const UnsignedBigInteger &a, const UnsignedBigInteger &b) {
            UnsignedBigInteger w(a);
            w &= b;
            return w;
        }

        UnsignedBigInteger &operator&=(const UnsignedBigInteger &rhs) {
            const size_t n = rhs.digits.size();
            if (digits.size() > n) {
                digits.resize(n);
            }
            for (size_t j = 0; j < digits.size(); ++j) {
                digits[j] &= rhs.digits[j];
            }
            trim();
            return *this;
        }

        friend UnsignedBigInteger operator^(const UnsignedBigInteger &u, const UnsignedBigInteger &v) {
            UnsignedBigInteger w(u);
            w ^= v;
            return w;
        }

        UnsignedBigInteger &operator^=(const UnsignedBigInteger &rhs) {
            const size_t n = rhs.digits.size();
            if (digits.size() < n) {
                digits.resize(n, 0);
            }
            for (size_t j = 0; j < n; ++j) {
                digits[j] ^= rhs.digits[j];
            }
            trim();
            return *this;
        }

        friend UnsignedBigInteger operator|(const UnsignedBigInteger &u, const UnsignedBigInteger &v) {
            UnsignedBigInteger w(u);
            w |= v;
            return w;
        }

        UnsignedBigInteger &operator|=(const UnsignedBigInteger &other) {
            const size_t n = other.digits.size();

            if (digits.size() < n) {
                digits.resize(n, 0);
            }

            for (size_t j = 0; j < n; ++j) {
                digits[j] |= other.digits[j]; // Simply or each digit
            }

            return *this;
        }

        /**
         * Operator < (strictly less than). It will be used for operators >, <= and >=
         * @param a
         * @param b
         * @return true if a < b.
         */
        friend bool operator<(const UnsignedBigInteger &a, const UnsignedBigInteger &b) {
            const size_t m = a.digits.size();
            size_t n = b.digits.size();

            if (m != n) {
                // Easy case, when digits length mismatch
                return (m < n);
            }

            // Get the first different digit
            for (--n; n != 0 && a.digits[n] == b.digits[n]; --n);

            return (a.digits[n] < b.digits[n]);
        }

        friend bool operator>(const UnsignedBigInteger &u, const UnsignedBigInteger &v) {
            return (v < u);
        }

        friend bool operator<=(const UnsignedBigInteger &u, const UnsignedBigInteger &v) {
            return !(v < u);
        }

        friend bool operator>=(const UnsignedBigInteger &u, const UnsignedBigInteger &v) {
            return !(u < v);
        }

        friend bool operator==(const UnsignedBigInteger &u, const UnsignedBigInteger &v) {
            return (u.digits == v.digits);
        }

        friend bool operator!=(const UnsignedBigInteger &u, const UnsignedBigInteger &v) {
            return (u.digits != v.digits);
        }

        /**
         * @return the string representation of the number in base 10.
         */
        [[nodiscard]] std::string to_string() const {
            std::ostringstream outputStringStream;
            UnsignedBigInteger quotient(*this), reminder;

            do {
                quotient.divide(10, quotient, reminder);
                outputStringStream << reminder.digits[0];
            } while (quotient.digits.back() != 0);

            std::string output(outputStringStream.str());
            std::reverse(output.begin(), output.end());

            return output;
        }

        friend std::ostream &operator<<(std::ostream &os, const UnsignedBigInteger &u) {
            os << u.to_string();
            return os;
        }

        /**
         * Convert an input string in base 10 to an UnsignedBigInteger
         * @param inputStream
         * @param result
         * @return the input stream
         */
        friend std::istream &operator>>(std::istream &inputStream, UnsignedBigInteger &result) {
            char digit = '\0';
            inputStream >> digit;

            if (inputStream.good() && std::isdigit(digit)) {
                result = digit - '0'; // Convert from ASCII

                while (std::isdigit(inputStream.peek())) {
                    inputStream >> digit;
                    // Safe to use result as a number since we implemented the operators
                    result = 10 * result + (digit - '0');
                }
            } else {
                inputStream.setstate(std::ios_base::failbit);
            }

            return inputStream;
        }


        /**
         * Get the most significant bit of the Modular Big Integer
         * @return
         */
        [[nodiscard]] size_t getMostSignificantBitIndex() const {
            size_t d = BITS - 1;
            for (; digits.back() >> d == 0; d--); // Scan the last block (highest order)

            return (digits.size() - 1) * BITS - 1 + d;
        }

        /**
         * Get the nth bit of the representation of the big integer.
         * @param bitIndex The index of the desired bit.
         * @return
         */
        uint8_t getBit(size_t bitIndex) {
            size_t blockIndex = bitIndex / BITS;
            uint8_t blockBitIndex = bitIndex % BITS;
            return (digits[blockIndex] >> blockBitIndex) & 1;
        }

    protected:

        /**
         * Drop leading zeros of the digits (remove the digits equal to zeros, starting from the back of the vector).
         */
        void trim() {
            while (digits.back() == 0 && digits.size() > 1) {
                digits.pop_back();
            }
        }
    };
}
#endif //INC_3A_ECC_CPP_UNSIGNEDBIGINTEGER_H