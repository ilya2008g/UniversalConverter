#include "universalconverter.h"
#include "BigInt.h"

#ifndef BIGFRACTION_H
#define BIGFRACTION_H
void BigFraction::shrink() { // Сокращение с помощью алгоритма Евклида
    BigInteger divisor = gcd(numerator_, denominator_);
    numerator_ = numerator_ / divisor;
    denominator_ = denominator_ / divisor;
}

BigFraction operator+(BigFraction lhs, BigFraction rhs) {
    BigInteger multi = lcm(lhs.denominator_, rhs.denominator_);
    BigInteger multi_one = multi / lhs.denominator_;
    BigInteger multi_two = multi / rhs.denominator_;
    BigFraction res = lhs;
    rhs.numerator_ = rhs.numerator_ * multi_two;
    res.numerator_ = res.numerator_ * multi_one + rhs.numerator_;
    res.denominator_ = res.denominator_ * multi_one;
    res.shrink();
    return res;
}

BigFraction operator-(BigFraction lhs, BigFraction rhs) {
    BigInteger multi = lcm(lhs.denominator_, rhs.denominator_);
    BigInteger multi_one = multi / lhs.denominator_;
    BigInteger multi_two = multi / rhs.denominator_;
    BigFraction res = lhs;
    rhs.numerator_ = rhs.numerator_ * multi_two;
    res.numerator_ = res.numerator_ * multi_one - rhs.numerator_;
    res.denominator_ = res.denominator_ * multi_one;
    res.shrink();
    return res;
}

BigFraction operator*(BigFraction lhs, BigFraction rhs) {
    BigFraction res(lhs.numerator_ * rhs.numerator_, lhs.denominator_ * rhs.denominator_);
    res.shrink();
    return res;
}

BigFraction operator/(BigFraction lhs, BigFraction rhs) {
    BigFraction rev(rhs.denominator_, rhs.numerator_);
    return lhs * rev;
}

BigInteger BigFraction::get_whole() {
    return numerator_ / denominator_;
}

BigInteger BigFraction::get_remainder() {
    return numerator_ % denominator_;
}
#endif // BIGFRACTION_H
