#ifndef MATH_UNSIGNED_H
#define MATH_UNSIGNED_H

#include <cstdint>
#include <vector>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <sstream>
#include <cctype>

using std::size_t;

namespace ECC {
    class BigNumber {
    public:
        typedef std::uint32_t Digit;
        typedef std::uint64_t Digit64;
        static const unsigned BITS = 32;

        /**
         * If there is no params, initialize to zero.
         * @param u
         */
        BigNumber(Digit u = 0) : digits(1, u) {}

        /**
         * Instantiate from a decimal string.
         * @param s The big number to instantiate (base 10).
         */
        BigNumber(const std::string &s) : digits(1, 0) {
            std::istringstream iss(s);
            iss >> *this;
            if (iss.fail() || !iss.eof()) {
                throw std::runtime_error("Error: BigNumber::string");
            }
        }

        /**
         * Copy a big number
         * @param copy
         */
        BigNumber(const BigNumber &copy) : digits(copy.digits) {
            // empty
        }

        BigNumber &operator=(const BigNumber &rhs) {
            digits = rhs.digits;
            return *this;
        }

        BigNumber operator++(int) {
            BigNumber w(*this);
            ++(*this);
            return w;
        }

        BigNumber &operator++() {
            for (size_t j = 0; j < digits.size() && ++digits[j] == 0; ++j);
            if (digits.back() == 0) {
                digits.push_back(1);
            }
            return *this;
        }

        BigNumber operator--(int) {
            BigNumber w(*this);
            --(*this);
            return w;
        }

        BigNumber &operator--() {
            if (digits.back() == 0) {
                throw std::underflow_error("Error: BigNumber::underflow");
            }

            for (size_t j = 0; j < digits.size() && digits[j]-- == 0; ++j);
            trim();
            return *this;
        }

        friend BigNumber operator+(const BigNumber &u, const BigNumber &v) {
            BigNumber w(u);
            w += v;
            return w;
        }

        BigNumber &operator+=(const BigNumber &other) {
            const size_t otherSize = other.digits.size();

            if (digits.size() < otherSize) {
                // Current number has a lower number of digits than the other and thus its digits vector has to be resized
                digits.resize(otherSize, 0);
            }

            size_t j = 0;
            Digit64 k = 0; // Since the digits size are 32 bits, we are working with 64 bits iterators

            for (; j < otherSize; ++j) {
                k = k + digits[j] + other.digits[j];
                digits[j] = static_cast<Digit>(k);
                k >>= BITS; // reminder
            }

            for (; k != 0 && j < digits.size(); ++j) {
                k += digits[j];
                digits[j] = static_cast<Digit>(k);
                k >>= BITS;
            }

            if (k != 0) {
                digits.push_back(1);
            }

            return *this;
        }

        friend BigNumber operator-(const BigNumber &u, const BigNumber &v) {
            BigNumber w(u);
            w -= v;
            return w;
        }

        BigNumber &operator-=(const BigNumber &rhs) {
            if ((*this) < rhs) {
                throw std::underflow_error("Error: BigNumber::underflow");
            }
            size_t j = 0;
            Digit64 k = 0;
            for (; j < rhs.digits.size(); ++j) {
                k = k + digits[j] - rhs.digits[j];
                digits[j] = static_cast<Digit>(k);
                k = ((k >> BITS) ? -1 : 0);
            }
            for (; k != 0 && j < digits.size(); ++j) {
                k += digits[j];
                digits[j] = static_cast<Digit>(k);
                k = ((k >> BITS) ? -1 : 0);
            }
            trim();
            return *this;
        }

        friend BigNumber operator*(const BigNumber &a, const BigNumber &b) {
            const size_t aSize = a.digits.size();
            const size_t bSize = b.digits.size();

            BigNumber result;

            // The maximum number of digits is aSize + bSize (e.g. 999*999=998001 -> 6 digits)
            result.digits.resize(aSize + bSize, 0);

            for (size_t bDigitIdx = 0; bDigitIdx < bSize; ++bDigitIdx) {
                Digit64 k = 0;

                for (size_t aDigitIdx = 0; aDigitIdx < aSize; ++aDigitIdx) {
                    k += static_cast<Digit64>(a.digits[aDigitIdx]) * b.digits[bDigitIdx]
                         + result.digits[aDigitIdx + bDigitIdx];
                    result.digits[aDigitIdx + bDigitIdx] = static_cast<Digit>(k);
                    k >>= BITS; // Reminder
                }

                result.digits[bDigitIdx + aSize] = static_cast<Digit>(k);
            }

            result.trim();
            return result;
        }

        BigNumber &operator*=(const BigNumber &rhs) {
            *this = (*this) * rhs;
            return *this;
        }

        friend BigNumber operator/(const BigNumber &u, const BigNumber &v) {
            BigNumber q, r;
            u.divide(v, q, r);
            return q;
        }

        BigNumber &operator/=(const BigNumber &rhs) {
            BigNumber r;
            divide(rhs, *this, r);
            return *this;
        }

        friend BigNumber operator%(const BigNumber &u, const BigNumber &v) {
            BigNumber q, r;
            u.divide(v, q, r);
            return r;
        }

        BigNumber &operator%=(const BigNumber &rhs) {
            BigNumber q;
            divide(rhs, q, *this);
            return *this;
        }

        void divide(BigNumber divider, BigNumber &quotient, BigNumber &reminder) const {
            // Division by zero
            if (divider.digits.back() == 0) {
                throw std::overflow_error("Error: BigNumber::overflow");
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
            BigNumber w;
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

            // Denormalize remainder.
            quotient.trim();
            reminder.digits.resize(n);
            reminder >>= d;
        }

        friend BigNumber operator<<(const BigNumber &u, size_t v) {
            BigNumber w(u);
            w <<= v;
            return w;
        }

        BigNumber &operator<<=(size_t rhs) {
            if (digits.back() != 0 && rhs != 0) {
                const size_t n = rhs / BITS;
                digits.insert(digits.begin(), n, 0);
                rhs -= n * BITS;
                Digit64 k = 0;
                for (size_t j = n; j < digits.size(); ++j) {
                    k |= static_cast<Digit64>(digits[j]) << rhs;
                    digits[j] = static_cast<Digit>(k);
                    k >>= BITS;
                }
                if (k != 0) {
                    digits.push_back(static_cast<Digit>(k));
                }
            }
            return *this;
        }

        friend BigNumber operator>>(const BigNumber &u, size_t v) {
            BigNumber w(u);
            w >>= v;
            return w;
        }

        BigNumber &operator>>=(size_t rhs) {
            const size_t n = rhs / BITS;
            if (n >= digits.size()) {
                digits.assign(1, 0);
            } else {
                digits.erase(digits.begin(), digits.begin() + n);
                rhs -= n * BITS;
                Digit64 k = 0;
                for (size_t j = digits.size(); j-- != 0;) {
                    k = k << BITS | digits[j];
                    digits[j] = static_cast<Digit>(k >> rhs);
                    k = static_cast<Digit>(k);
                }
                trim();
            }
            return *this;
        }

        friend BigNumber operator&(const BigNumber &u, const BigNumber &v) {
            BigNumber w(u);
            w &= v;
            return w;
        }

        BigNumber &operator&=(const BigNumber &rhs) {
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

        friend BigNumber operator^(const BigNumber &u, const BigNumber &v) {
            BigNumber w(u);
            w ^= v;
            return w;
        }

        BigNumber &operator^=(const BigNumber &rhs) {
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

        friend BigNumber operator|(const BigNumber &u, const BigNumber &v) {
            BigNumber w(u);
            w |= v;
            return w;
        }

        BigNumber &operator|=(const BigNumber &rhs) {
            const size_t n = rhs.digits.size();
            if (digits.size() < n) {
                digits.resize(n, 0);
            }
            for (size_t j = 0; j < n; ++j) {
                digits[j] |= rhs.digits[j];
            }
            return *this;
        }

        friend bool operator<(const BigNumber &u, const BigNumber &v) {
            const size_t m = u.digits.size();
            size_t n = v.digits.size();
            if (m != n) {
                return (m < n);
            }
            for (--n; n != 0 && u.digits[n] == v.digits[n]; --n);
            return (u.digits[n] < v.digits[n]);
        }

        friend bool operator>(const BigNumber &u, const BigNumber &v) {
            return (v < u);
        }

        friend bool operator<=(const BigNumber &u, const BigNumber &v) {
            return !(v < u);
        }

        friend bool operator>=(const BigNumber &u, const BigNumber &v) {
            return !(u < v);
        }

        friend bool operator==(const BigNumber &u, const BigNumber &v) {
            return (u.digits == v.digits);
        }

        friend bool operator!=(const BigNumber &u, const BigNumber &v) {
            return (u.digits != v.digits);
        }

        Digit to_uint() const {
            return digits[0];
        }

        std::string to_string() const {
            std::ostringstream oss;
            BigNumber q(*this), r;
            do {
                q.divide(10, q, r);
                oss << r.digits[0];
            } while (q.digits.back() != 0);
            std::string s(oss.str());
            std::reverse(s.begin(), s.end());
            return s;
        }

        friend std::ostream &operator<<(std::ostream &os, const BigNumber &u) {
            os << u.to_string();
            return os;
        }

        friend std::istream &operator>>(std::istream &is, BigNumber &u) {
            char digit = '\0';
            is >> digit;
            if (is.good() && std::isdigit(digit)) {
                u = digit - '0';
                while (std::isdigit(is.peek())) {
                    is >> digit;
                    u = 10 * u + (digit - '0');
                }
            } else {
                is.setstate(std::ios_base::failbit);
            }
            return is;
        }

    private:
        std::vector<Digit> digits;

        void trim() {
            while (digits.back() == 0 && digits.size() > 1) {
                digits.pop_back();
            }
        }
    };
} // namespace math

#endif // MATH_UNSIGNED_H