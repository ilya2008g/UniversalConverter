#ifndef UNIVERSALCONVERTER_H
#define UNIVERSALCONVERTER_H

#include <QMainWindow>
#include <vector>
#include <string>

using std::string, std::vector;
static const int BASE = 1e9;
const size_t MAX_OUTPUT_LENGTH = 100000;

QT_BEGIN_NAMESPACE
namespace Ui {
class UniversalConverter;
}
QT_END_NAMESPACE

class UniversalConverter : public QMainWindow
{
    Q_OBJECT

public:
    UniversalConverter(QWidget *parent = nullptr);
    ~UniversalConverter();

private:
    Ui::UniversalConverter *ui;
};

class BigInteger {
public:
    BigInteger(): data_(0) {}
    explicit BigInteger(long long x);
    explicit BigInteger(string s);
    int size() {
        return data_.size();
    }
    friend BigInteger gcd(BigInteger lhs, BigInteger rhs);
    bool if_zero(BigInteger x);
    string convert_to_str(BigInteger x);
    friend BigInteger operator+(BigInteger lhs, BigInteger rhs);
    friend BigInteger operator-(BigInteger lhs, BigInteger rhs);
    friend BigInteger operator*(BigInteger lhs, BigInteger rhs);
    friend BigInteger operator*(BigInteger lhs, int rhs);
    friend BigInteger shift_left(BigInteger& num, int k);
    friend BigInteger operator/(BigInteger lhs, BigInteger rhs);
    friend BigInteger operator/(BigInteger lhs, int rhs);
    friend BigInteger operator%(BigInteger lhs, BigInteger rhs);
    friend BigInteger operator%(BigInteger lhs, int rhs);
    friend bool operator<(BigInteger lhs, BigInteger rhs);
    friend bool operator<=(BigInteger lhs, BigInteger rhs);
    friend bool operator>(BigInteger lhs, BigInteger rhs);
    friend bool operator>=(BigInteger lhs, BigInteger rhs);
    friend bool operator==(BigInteger lhs, BigInteger rhs);
    friend bool operator!=(BigInteger lhs, BigInteger rhs);

private:
    vector<long long> data_;
};

class BigFraction {
public:
    BigFraction(): numerator_(0), denominator_(1) {}
    BigFraction(BigInteger num, BigInteger den): numerator_(num), denominator_(den) {}
    BigFraction shrink();
    friend BigFraction operator+(BigFraction lhs, BigFraction rhs);
    friend BigFraction operator-(BigFraction lhs, BigFraction rhs);
    friend BigFraction operator*(BigFraction lhs, BigFraction rhs);
    friend BigFraction operator/(BigFraction lhs, BigFraction rhs);
    BigInteger get_whole();
    BigInteger get_remainder();

private:
    BigInteger numerator_;
    BigInteger denominator_;
};

class InputValidator {
public:
    InputValidator(string in, int p, int q): data_(in), p_(p), q_(q) {}
    bool is_valid();
private:
    bool DigitValidator(string num);
    string data_;
    int p_;
    int q_;
};

class DigitTokenizer {
public:
    explicit DigitTokenizer(string s);
    string get_whole() {return whole_;}
    string get_non_periodic() {return non_per_;}
    string get_periodic() {return per_;}
private:
    string whole_;
    string non_per_;
    string per_;
};

class BaseParser {
public:
    explicit BaseParser(string s);
    BigFraction get_num() {return number_;}
private:
    string whole_p_;
    string non_per_p_;
    string per_p_;
    BigFraction number_;
};

#endif // UNIVERSALCONVERTER_H
