#include "universalconverter.h"
#include "ui_universalconverter.h"

UniversalConverter::UniversalConverter(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UniversalConverter)
{
    ui->setupUi(this);
}

UniversalConverter::~UniversalConverter()
{
    delete ui;
}

BigInteger::BigInteger(long long x) {
    while (x != 0) {
        data_.push_back(x % BASE);
        x /= BASE;
    }
}

BigInteger::BigInteger(string s) {
    for (int i = s.length(); i > 0; i -= 9) {
        if (i < 9) {
            data_.push_back(atoi(s.substr(0, i). c_str()));
        } else {
            data_.push_back(atoi(s.substr(i - 9, 9). c_str()));
        }
    }
}

string BigInteger::convert_to_str(BigInteger x) {
    string s = "";
    for (int i = 0; i < x.size(); ++i) {
        s += std::to_string(data_[i]);
    }
    return s;
}

bool if_zero(BigInteger x) {
    return x.size() == 0;
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

BigInteger shift_left(BigInteger& num, int k) { // сдвиг числа на BASE^k
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
    for (int i = lhs.size(); i >= 0; --i) {
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
        BigInteger half = diff / 2;
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
    for (int i = lhs.size(); i >= 0; --i) {
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

BigFraction BigFraction::shrink() {
    BigInteger divisor = gcd(numerator_, denominator_);
    return BigFraction(numerator_ / divisor, denominator_ / divisor);
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

bool InputValidator::DigitValidator(string num) {
    for (int i = 0; i < (int)num.size();) {
        if (num[i] == '[') {
            size_t j = num.find(']', i + 1);
            if (j == string::npos) return false;
            string number = num.substr(i + 1, j - i - 1);
            if (number.empty()) return false;
            if (!all_of(number.begin(), number.end(), ::isdigit)) return false;

            int val = 0;
            for (char c : number) {
                int dig = c - '0';
                val = val * 10 + dig;
                if (val >= 500) return false;
            }
            if (val >= p_) return false;
            i = j + 1;
        } else {
            char c = num[i];
            int val = -1;
            if (c >= '0' && c <= '9') val = c - '0';
            else if (c >= 'A' && c <= 'Z') val = 10 + (c - 'A');
            else if (c >= 'a' && c <= 'z') val = 10 + (c - 'a');
            else return false;
            if (val >= p_) return false;
            ++i;
        }
    }
    return true;
}

bool InputValidator::is_valid() {
    if (p_ < 2 || p_ > 500 || q_ < 2 || q_ > 500) return false;
    int dots = count(data_.begin(), data_.end(), '.');
    if (dots > 1) {
        return false;
    }

    for (char c : data_) {
        if (!isalnum(c) && c != '.' && c != '(' && c != ')' && c != '[' && c != ']')
            return false;
    }

    if (dots == 0) {
        if (!DigitValidator(data_)) return false;
        if (data_.size() > 1 && data_[0] == '0') return false;
        return true;
    }

    size_t dot = data_.find('.');
    string int_part = data_.substr(0, dot);
    string frac_part = data_.substr(dot + 1);

    if (int_part.empty() || frac_part.empty()) return false;

    if (!DigitValidator(int_part)) return false;
    if (int_part.size() > 1 && int_part[0] == '0') return false;

    size_t open = frac_part.find('(');
    if (open != string::npos) {
        size_t close = frac_part.find(')', open + 1);
        if (close == string::npos ||
            frac_part.find('(', open + 1) != string::npos ||
            frac_part.find(')', close + 1) != string::npos ||
            close != frac_part.size() - 1)
            return false;

        string non_per = frac_part.substr(0, open);
        string per = frac_part.substr(open + 1, close - open - 1);
        if (per.empty()) return false;

        if (!DigitValidator(non_per) || !DigitValidator(per)) return false;
    } else {
        if (!DigitValidator(frac_part)) return false;
    }
    return true;
}

DigitTokenizer::DigitTokenizer(string s) {
    size_t it = s.find('.');
    if (it != string::npos) {
        whole_ = s.substr(0, it);
        size_t period_one = s.find('(');
        size_t period_two = s.find(')');
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

BaseParser::BaseParser(string s) {
    DigitTokenizer num(s);
    whole_p_ = num.get_whole();
    non_per_p_ = num.get_non_periodic();
    per_p_ = num.get_periodic();

}
