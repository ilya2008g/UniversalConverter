#include "universalconverter.h"
#include "BigFraction.h"

#ifndef BASEFORMATTER_H
#define BASEFORMATTER_H
string BaseFormatter::digit_(int val, int base) {
    if (base <= 36) {
        if (val < 10) {
            return string(1, '0' + val);
        } else {
            return string(1, 'A' + val - 10);
        }
    } else {
        return "[" + std::to_string(val) + "]";
    }
}

string BaseFormatter::int_to_q_(BigInteger num, int q) {
    if (num == BigInteger(0)) return "0";
    string res;
    while (num != BigInteger(0)) {
        BigInteger rem = num % q;
        res += digit_(rem.convert_to_int(), q);
        num = num / q;
    }
    std::reverse(res.begin(), res.end());
    return res;
}

string BaseFormatter::convert(BigFraction value, int q) {
    value.shrink();
    BigInteger whole = value.get_whole();
    BigInteger remainder = value.get_remainder();

    string int_part = int_to_q_(whole, q);

    if (remainder == BigInteger(0)) {
        return int_part;
    }

    BigInteger denominator = value.get_den();
    vector<int> digits;
    std::map<BigInteger, size_t> seen;
    seen[remainder] = 0;

    while (remainder != BigInteger(0)) {
        remainder = remainder * q;
        int digit = (remainder / denominator).convert_to_int();
        digits.push_back(digit);
        remainder = remainder % denominator;

        auto it = seen.find(remainder);
        if (it != seen.end()) {
            size_t start = it->second;
            string frac_part;
            for (size_t i = 0; i < start; ++i) {
                frac_part += digit_(digits[i], q);
            }
            frac_part += "(";
            for (size_t i = start; i < digits.size(); ++i) {
                frac_part += digit_(digits[i], q);
            }
            frac_part += ")";
            return int_part + "." + frac_part;
        }
        seen[remainder] = digits.size();
        if (digits.size() > MAX_OUTPUT_LENGTH) break;
    }

    string frac_part;
    for (int d : digits) {
        frac_part += digit_(d, q);
    }
    return int_part + "." + frac_part;
}
#endif // BASEFORMATTER_H
