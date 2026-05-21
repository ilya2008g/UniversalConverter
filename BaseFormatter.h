#include "universalconverter.h"
#include "BigFraction.h"

#ifndef BASEFORMATTER_H
#define BASEFORMATTER_H
std::string BaseFormatter::digit_(int val, int base) {
    if (base <= 36) {
        if (val < 10) return std::string(1, '0' + val);
        else          return std::string(1, 'A' + val - 10);
    } else {
        if (val < 10) return std::string(1, '0' + val);
        else if (val < 36) return std::string(1, 'A' + val - 10);
        else return "[" + std::to_string(val) + "]";
    }
}

string BaseFormatter::int_to_q_(BigInteger num, int q) { // перевод отдельной части в q-ичную СС
    if (num == BigInteger(0)) return "0";

    std::vector<std::string> digits;
    while (num != BigInteger(0)) {
        BigInteger rem = num % q;
        digits.push_back(digit_(rem.convert_to_int(), q));
        num = num / q;
    }

    string res;
    for (auto it = digits.rbegin(); it != digits.rend(); ++it)
        res += *it;
    return res;
}

string BaseFormatter::convert(BigFraction value, int q) { // перевод в q-ичную СС
    value.shrink();
    BigInteger whole = value.get_whole();
    BigInteger remainder = value.get_remainder();

    string int_part = int_to_q_(whole, q);

    if (remainder == BigInteger(0)) {
        return int_part;
    }

    BigInteger denominator = value.get_den();
    vector<int> digits;
    std::map<BigInteger, size_t> seen; // сохранение остатков
    seen[remainder] = 0;

    while (remainder != BigInteger(0)) { // алгоритм перевода
        remainder = remainder * q;
        int digit = (remainder / denominator).convert_to_int();
        digits.push_back(digit);
        remainder = remainder % denominator;

        auto it = seen.find(remainder);
        if (it != seen.end()) { // обнаружение периода
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
        if (digits.size() > MAX_OUTPUT_LENGTH) break; // обрезка числа при слишком длинном результате
    }

    string frac_part;
    for (int d : digits) { // сборка числа
        frac_part += digit_(d, q);
    }
    return int_part + "." + frac_part;
}
#endif // BASEFORMATTER_H
