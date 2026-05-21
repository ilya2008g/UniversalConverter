#include "universalconverter.h"

#ifndef BIGINT_H
#define BIGINT_H
BigInteger::BigInteger(long long x) {
    if (x == 0) data_.push_back(0);
    else {
        while (x != 0) {
            data_.push_back(x % BASE);
            x /= BASE;
        }
    }
}

BigInteger::BigInteger(string s) {
    for (int i = s.length(); i > 0; i -= 9) {
        if (i < 9) {
            data_.push_back(atoi(s.substr(0, i).c_str()));
        } else {
            data_.push_back(atoi(s.substr(i - 9, 9).c_str()));
        }
    }
}


bool if_zero(BigInteger x) {
    return x.size() == 1 && x.data_[0] == 0;
}

string BigInteger::convert_to_str() {
    if (if_zero(*this)) return "0";
    string s;
    s.reserve(size() * 9);
    s += std::to_string(data_.back());
    for (int i = (int)size() - 2; i >= 0; --i) {
        string block = std::to_string(data_[i]);
        s += string(9 - block.length(), '0') + block;
    }
    return s;
}

int BigInteger::convert_to_int() {
    if (data_.empty()) return 0;
    long long val = 0;
    for (int i = (int)size() - 1; i >= 0; --i)
        val = val * BASE + data_[i];
    if (val > INT_MAX) {
        throw std::overflow_error("BigInteger too large for int");
    }
    return (int)val;
}

BigInteger operator+(BigInteger lhs, BigInteger rhs) {
    int carry = 0;
    BigInteger res = lhs;
    for (int i = 0; i < std::max(lhs.size(), rhs.size()) || carry; ++i) {
        if (i == res.size()) {
            res.data_.push_back(0);
        }
        res.data_[i] += carry + (i < rhs.size() ? rhs.data_[i] : 0);
        carry = res.data_[i] >= BASE;
        if (carry) res.data_[i] -= BASE;
    }
    return res;
}

bool operator<(BigInteger lhs, BigInteger rhs) {
    if (lhs.size() != rhs.size())
        return lhs.size() < rhs.size();
    for (int i = lhs.size() - 1; i >= 0; --i) {
        if (lhs.data_[i] != rhs.data_[i])
            return lhs.data_[i] < rhs.data_[i];
    }
    return false;
}

bool operator==(BigInteger lhs, BigInteger rhs) {
    return lhs.data_ == rhs.data_;
}

bool operator!=(BigInteger lhs, BigInteger rhs) {
    return !(lhs == rhs);
}

bool operator<=(BigInteger lhs, BigInteger rhs) {
    return (lhs < rhs) || (lhs == rhs);
}

bool operator>(BigInteger lhs, BigInteger rhs) {
    return rhs < lhs;
}

bool operator>=(BigInteger lhs, BigInteger rhs) {
    return !(lhs < rhs);
}

BigInteger operator-(BigInteger lhs, BigInteger rhs) {
    assert(lhs >= rhs);
    int carry = 0;
    BigInteger res = lhs;
    for (int i = 0; i < rhs.size() || carry; ++i) {
        res.data_[i] -= carry + (i < rhs.size() ? rhs.data_[i] : 0);
        carry = res.data_[i] < 0;
        if (carry) res.data_[i] += BASE;
    }
    while (res.size() > 1 && res.data_.back() == 0) {
        res.data_.pop_back();
    }
    return res;
}

BigInteger operator*(BigInteger lhs, int rhs) { // умножение на число < BASE
    int carry = 0;
    BigInteger res = lhs;
    for (int i = 0; i < lhs.size() || carry; ++i) {
        if (i == res.size()) {
            res.data_.push_back(0);
        }
        long long cur = carry + res.data_[i] * 1ll * rhs;
        res.data_[i] = int(cur % BASE);
        carry = int(cur / BASE);
    }
    while (res.size() > 1 && res.data_.back() == 0) {
        res.data_.pop_back();
    }
    return res;
}

BigInteger shift_left(BigInteger num, int k) { // сдвиг числа на BASE^k
    if (num.size() == 1 && num.data_[0] == 0) return num;
    BigInteger res = num;
    res.data_.insert(res.data_.begin(), k, 0);
    return res;
}

BigInteger operator*(BigInteger lhs, BigInteger rhs) { // Алгоритм Карацубы
    if (lhs.size() == 1) { // базовые случаи
        return rhs * lhs.data_[0];
    }
    if (rhs.size() == 1) {
        return lhs * rhs.data_[0];
    }
    int n = std::max(lhs.size(), rhs.size());
    int mid = n / 2;
    BigInteger lhs_L, lhs_R, rhs_L, rhs_R;

    int lhs_L_len = std::min(mid, lhs.size()); // разбиение lhs
    lhs_L.data_.assign(lhs.data_.begin(), lhs.data_.begin() + lhs_L_len);
    if (lhs.size() > mid)
        lhs_R.data_.assign(lhs.data_.begin() + mid, lhs.data_.end());
    else
        lhs_R.data_ = {0};

    int rhs_L_len = std::min(mid, rhs.size()); // разбиение rhs
    rhs_L.data_.assign(rhs.data_.begin(), rhs.data_.begin() + rhs_L_len);
    if (rhs.size() > mid)
        rhs_R.data_.assign(rhs.data_.begin() + mid, rhs.data_.end());
    else
        rhs_R.data_ = {0};

    BigInteger res1 = lhs_L * rhs_L;
    BigInteger res2 = lhs_R * rhs_R;
    BigInteger res3 = (lhs_L + lhs_R) * (rhs_L + rhs_R);
    BigInteger res4 = res3 - res2 - res1;
    return shift_left(res2, 2 * mid) + shift_left(res4, mid) + res1;
}

BigInteger operator/(BigInteger lhs, int rhs) {
    int carry = 0;
    BigInteger res = lhs;
    for (int i = lhs.size() - 1; i >= 0; --i) {
        long long cur = lhs.data_[i] + carry * 1ll * BASE;
        res.data_[i] =  int(cur / rhs);
        carry = int(cur % rhs);
    }
    while (res.size() > 1 && res.data_.back() == 0) {
        res.data_.pop_back();
    }
    return res;
}

BigInteger operator/(BigInteger lhs, BigInteger rhs) {
    if (if_zero(rhs)) {
        throw string("Деление на ноль");
    }
    if (rhs > lhs) return BigInteger(0);

    size_t len_diff = lhs.size() - rhs.size() + 1;
    BigInteger one(1);
    BigInteger high = shift_left(one, len_diff) - one;
    BigInteger low(0);
    BigInteger q(0);

    while (low <= high) {
        BigInteger diff = high - low;
        BigInteger mid = low + (high - low) / 2;

        BigInteger prod = mid * rhs;
        if (prod == lhs) {
            return mid;
        } else if (prod < lhs) {
            q = mid;
            low = mid + one;
        } else {
            high = mid - one;
        }
    }
    return q;
}

BigInteger operator%(BigInteger lhs, int rhs) {
    int carry = 0;
    BigInteger res = lhs;
    for (int i = lhs.size() - 1; i >= 0; --i) {
        long long cur = lhs.data_[i] + carry * 1ll * BASE;
        res.data_[i] =  int(cur / rhs);
        carry = int(cur % rhs);
    }
    BigInteger remainder(carry);
    return remainder;
}

BigInteger operator%(BigInteger lhs, BigInteger rhs) {
    BigInteger divide = lhs / rhs;
    return lhs - rhs * divide;
}

BigInteger gcd(BigInteger lhs, BigInteger rhs) {
    if (if_zero(lhs)) {
        return rhs;
    }
    return gcd(rhs % lhs, lhs);
}

BigInteger lcm(BigInteger lhs, BigInteger rhs) {
    return (lhs * rhs) / gcd(lhs, rhs);
}
#endif // BIGINT_H
