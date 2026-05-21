#include "universalconverter.h"

#ifndef DIGITTOKENIZER_H
#define DIGITTOKENIZER_H
DigitTokenizer::DigitTokenizer(string s) {
    size_t it = s.find('.');
    if (it != string::npos) {
        whole_ = s.substr(0, it);
        size_t period_one = s.find('(');
        size_t period_two = s.find(')', period_one + 1);
        if (period_one != string::npos) {
            non_per_ = s.substr(it + 1, period_one - it - 1);
            per_ = s.substr(period_one + 1, period_two - period_one - 1);
        } else {
            non_per_ = s.substr(it + 1);
        }
    } else {
        whole_ = s;
    }
}
#endif // DIGITTOKENIZER_H
