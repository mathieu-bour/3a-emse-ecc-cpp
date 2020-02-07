#include "../../includes/ecc/UnsignedBigInteger.h"

using namespace ecc;


/*
 * Constructors
 * ======================================================================
 */
UnsignedBigInteger::UnsignedBigInteger(Digit pDigit) {
    digits = Digits(1, pDigit);
}


UnsignedBigInteger::UnsignedBigInteger(Digits pDigits) {
    digits = std::move(pDigits);
    trim();
}


UnsignedBigInteger::UnsignedBigInteger(const std::string &str) {
    digits = Digits(1, 0);

    std::istringstream inputStringStream(str);

    inputStringStream >> *this; // see std::istream &operator>>

    if (inputStringStream.fail() || !inputStringStream.eof()) {
        throw std::runtime_error("Error: UnsignedBigInteger::string");
    }
}


/*
 * Operators
 * ======================================================================
 */
UnsignedBigInteger &UnsignedBigInteger::operator=(Digit pDigit) {
    *this = UnsignedBigInteger(pDigit);

    return *this;
}


UnsignedBigInteger &UnsignedBigInteger::operator=(const std::string &str) {
    *this = UnsignedBigInteger(str);

    return *this;
}


bool UnsignedBigInteger::operator==(const UnsignedBigInteger &other) const {
    return (digits == other.digits);
}


bool UnsignedBigInteger::operator!=(const UnsignedBigInteger &other) const {
    return !(*this == other);
}


bool UnsignedBigInteger::operator<(const UnsignedBigInteger &other) const {
    const size_t m = digits.size();
    size_t n = other.digits.size();

    if (m != n) {
        // Easy and fast case, when digits length mismatch
        return (m < n);
    }

    // Get the first different digit
    for (--n; n != 0 && digits[n] == other.digits[n]; --n);

    return (digits[n] < other.digits[n]);
}


bool UnsignedBigInteger::operator>(const UnsignedBigInteger &other) const {
    return (other < *this);
}


bool UnsignedBigInteger::operator<=(const UnsignedBigInteger &other) const {
    return !(other < *this);
}


bool UnsignedBigInteger::operator>=(const UnsignedBigInteger &other) const {
    return !(*this < other);
}


UnsignedBigInteger &UnsignedBigInteger::operator++() {
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


const UnsignedBigInteger UnsignedBigInteger::operator++(int) {
    UnsignedBigInteger that(*this);
    ++(*this);
    return that;
}


UnsignedBigInteger &UnsignedBigInteger::operator--() {
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


const UnsignedBigInteger UnsignedBigInteger::operator--(int) {
    UnsignedBigInteger that(*this);
    --(*this);

    return that;
}


UnsignedBigInteger UnsignedBigInteger::operator+(const UnsignedBigInteger &other) const {
    UnsignedBigInteger sum(*this);
    sum += other;

    return sum;
}


UnsignedBigInteger &UnsignedBigInteger::operator+=(const UnsignedBigInteger &other) {
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


UnsignedBigInteger UnsignedBigInteger::operator-(const UnsignedBigInteger &delta) const {
    UnsignedBigInteger difference(*this);
    difference -= delta;

    return difference;
}


UnsignedBigInteger &UnsignedBigInteger::operator-=(const UnsignedBigInteger &delta) {
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


UnsignedBigInteger UnsignedBigInteger::operator*(const UnsignedBigInteger &other) const {
    const size_t aSize = digits.size();
    const size_t bSize = other.digits.size();

    UnsignedBigInteger result; // result is initialized to zero

    // The maximum number of digits is aSize + bSize (like 999*999=998001 -> 6 digits)
    result.digits.resize(aSize + bSize, 0);

    for (size_t bDigitIdx = 0; bDigitIdx < bSize; ++bDigitIdx) {
        Digit64 carry = 0;

        for (size_t aDigitIdx = 0; aDigitIdx < aSize; ++aDigitIdx) {
            carry += static_cast<Digit64>(digits[aDigitIdx]) * other.digits[bDigitIdx] // on a 64-bits
                     + result.digits[aDigitIdx + bDigitIdx];

            // The static cast will only take the lowest 32-bits
            result.digits[aDigitIdx + bDigitIdx] = static_cast<Digit>(carry);

            carry >>= UnsignedBigInteger::BITS; // Reminder
        }

        result.digits[bDigitIdx + aSize] = static_cast<Digit>(carry);
    }

    result.trim(); // result was provisioned using the maximum digits

    return result;
}


UnsignedBigInteger &UnsignedBigInteger::operator*=(const UnsignedBigInteger &other) {
    *this = (*this) * other;

    return *this;
}


UnsignedBigInteger UnsignedBigInteger::operator/(const UnsignedBigInteger &divider) const {
    UnsignedBigInteger quotient, reminder;
    divide(divider, quotient, reminder); // We do not care of the reminder

    return quotient;
}


UnsignedBigInteger &UnsignedBigInteger::operator/=(const UnsignedBigInteger &divider) {
    UnsignedBigInteger reminder;
    divide(divider, *this, reminder); // We do not care of the reminder

    return *this;
}


UnsignedBigInteger UnsignedBigInteger::operator%(const UnsignedBigInteger &divider) const {
    UnsignedBigInteger quotient, reminder;
    divide(divider, quotient, reminder); // We do not care of the quotient

    return reminder;
}


UnsignedBigInteger &UnsignedBigInteger::operator%=(const UnsignedBigInteger &divider) {
    UnsignedBigInteger quotient;
    divide(divider, quotient, *this); // We do not care of the quotient

    return *this;
}


UnsignedBigInteger UnsignedBigInteger::operator<<(size_t shiftSize) const {
    UnsignedBigInteger shifted(*this);
    shifted <<= shiftSize;

    return shifted;
}


UnsignedBigInteger &UnsignedBigInteger::operator<<=(size_t shiftSize) {
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


UnsignedBigInteger UnsignedBigInteger::operator>>(size_t shiftSize) const {
    UnsignedBigInteger result(*this);
    result >>= shiftSize;

    return result;
}


UnsignedBigInteger &UnsignedBigInteger::operator>>=(size_t shiftSize) {
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


UnsignedBigInteger ecc::operator&(const UnsignedBigInteger &a, const UnsignedBigInteger &b) {
    UnsignedBigInteger result(a);
    result &= b;

    return result;
}


UnsignedBigInteger &UnsignedBigInteger::operator&=(const UnsignedBigInteger &other) {
    const size_t n = other.digits.size();

    if (digits.size() > n) {
        digits.resize(n);
    }

    for (size_t j = 0; j < digits.size(); ++j) {
        digits[j] &= other.digits[j];
    }

    trim(); // And operator may introduce leading zeros

    return *this;
}


UnsignedBigInteger UnsignedBigInteger::operator|(const UnsignedBigInteger &other) const {
    UnsignedBigInteger result(*this);
    result |= other;

    return result;
}


UnsignedBigInteger &UnsignedBigInteger::operator|=(const UnsignedBigInteger &other) {
    const size_t n = other.digits.size();

    if (digits.size() < n) {
        digits.resize(n, 0);
    }

    for (size_t j = 0; j < n; ++j) {
        digits[j] |= other.digits[j]; // Simply OR each digit
    }

    return *this;
}


UnsignedBigInteger UnsignedBigInteger::operator^(const UnsignedBigInteger &other) const {
    UnsignedBigInteger result(*this);
    result ^= other;

    return result;
}


UnsignedBigInteger &UnsignedBigInteger::operator^=(const UnsignedBigInteger &other) {
    const size_t n = other.digits.size();

    if (digits.size() < n) {
        digits.resize(n, 0);
    }

    for (size_t j = 0; j < n; ++j) {
        digits[j] ^= other.digits[j];
    }

    trim();

    return *this;
}


std::istream &ecc::operator>>(std::istream &inputStream, UnsignedBigInteger &result) {
    char digit = '\0';
    inputStream >> digit;

    if (inputStream.good() && std::isdigit(digit)) {
        result = digit - '0'; // Convert from ASCII

        while (std::isdigit(inputStream.peek())) {
            inputStream >> digit;
            // Safe to use result as a number since we implemented these operators
            result = UnsignedBigInteger(10) * result + (digit - '0');
        }
    } else {
        inputStream.setstate(std::ios_base::failbit);
    }

    return inputStream;
}


std::ostream &ecc::operator<<(std::ostream &outputStream, const UnsignedBigInteger &source) {
    outputStream << source.to_string();
    return outputStream;
}


/*
 * Methods
 * ======================================================================
 */
void UnsignedBigInteger::divide(
        UnsignedBigInteger divider,
        UnsignedBigInteger &quotient,
        UnsignedBigInteger &reminder
) const {
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

    // Normalize divisor (divider[modulus-1] >= BASE/2).
    unsigned d = BITS;
    for (Digit vn = divider.digits.back(); vn != 0; vn >>= 1, --d);
    divider <<= d;
    reminder <<= d;
    const Digit vn = divider.digits.back();

    // Ensure first single-digit quotient (u[m-1] < divider[modulus-1]).
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


size_t UnsignedBigInteger::getMostSignificantBitIndex() const {
    size_t d = BITS - 1;
    for (; digits.back() >> d == 0; d--); // Scan the last block (highest order)

    return (digits.size() - 1) * BITS + 1 + d;
}

std::string UnsignedBigInteger::to_string() const {
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


void UnsignedBigInteger::trim() {
    while (digits.back() == 0 && digits.size() > 1) {
        digits.pop_back();
    }
}
