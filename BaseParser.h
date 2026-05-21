#include "universalconverter.h"
#include "BigFraction.h"
#include "DigitTokenizer.h"

#ifndef BASEPARSER_H
#define BASEPARSER_H

BigInteger BaseParser::get_digits_(string s) {
    BigInteger res(0);
    for (size_t i = 0; i < s.size(); ) {
        int val = 0;
        if (s[i] == '[') {
            size_t j = s.find(']', i + 1);
            string num = s.substr(i + 1, j - i - 1);
            val = stoi(num);
            i = j + 1;
        } else {
            char c = s[i];
            if (c >= '0' && c <= '9') val = c - '0';
            else if (c >= 'A' && c <= 'Z') val = 10 + c - 'A';
            else if (c >= 'a' && c <= 'z') val = 10 + c - 'a';
            ++i;
        }
        res = res * p_ + BigInteger(val);
    }
    return res;
}

BigInteger BaseParser::pow_p_(int exp) {
    BigInteger res(1);
    for (int i = 0; i < exp; ++i) res = res * p_;
    return res;
}

int BaseParser::get_len_(string s) {
    int cnt = 0;
    for (size_t i = 0; i < s.size();) {
        if (s[i] == '[') {
            size_t j = s.find(']', i + 1);
            if (j != string::npos) {
                ++cnt;
                i = j + 1;
            } else ++i;
        } else {
            ++cnt;
            ++i;
        }
    }
    return cnt;
}

BaseParser::BaseParser(string s, int p) : p_(p) {
    DigitTokenizer tok(s);

    BigInteger whole = get_digits_(tok.get_whole());
    BigInteger non_per = get_digits_(tok.get_non_periodic());
    BigInteger per = get_digits_(tok.get_periodic());

    int len_non_per = get_len_(tok.get_non_periodic());
    int len_per = get_len_(tok.get_periodic());

    BigFraction frac_part;
    if (len_per == 0) {
        BigInteger pK = pow_p_(len_non_per);
        frac_part = BigFraction(non_per, pK);
    } else {
        BigInteger pK = pow_p_(len_non_per);
        BigInteger pM_minus_one = pow_p_(len_per) - BigInteger(1);
        BigInteger num = non_per * pM_minus_one + per;
        BigInteger den = pK * pM_minus_one;
        frac_part = BigFraction(num, den);
    }

    BigFraction whole_part(whole, BigInteger(1));
    BigFraction sum = whole_part + frac_part;
    number_ = sum;
}
#endif // BASEPARSER_H
