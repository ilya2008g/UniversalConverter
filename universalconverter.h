#ifndef UNIVERSALCONVERTER_H
#define UNIVERSALCONVERTER_H

#include <QMainWindow>
#include <vector>
#include <string>
#include <QFileDialog>
#include <QMessageBox>
#include <fstream>

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

private slots:
    void on_convert_clicked();
    void on_file_input_clicked();

private:
    Ui::UniversalConverter *ui;

    void show_error_(string msg);
    void convert_and_display_();
    void save_to_file_(string res);
};

class BigInteger {
public:
    BigInteger(): data_(0) {}
    explicit BigInteger(long long x);
    explicit BigInteger(string s);
    explicit BigInteger(vector<long long> num): data_(num) {}
    int size() {
        return data_.size();
    }
    friend void convert_to_ten(BigInteger& num, int p);
    friend BigInteger gcd(BigInteger lhs, BigInteger rhs);
    friend bool if_zero(BigInteger x);
    string convert_to_str();
    int convert_to_int();
    friend BigInteger operator+(BigInteger lhs, BigInteger rhs);
    friend BigInteger operator-(BigInteger lhs, BigInteger rhs);
    friend BigInteger operator*(BigInteger lhs, BigInteger rhs);
    friend BigInteger operator*(BigInteger lhs, int rhs);
    friend BigInteger shift_left(BigInteger num, int k);
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
    void shrink();
    friend BigFraction operator+(BigFraction lhs, BigFraction rhs);
    friend BigFraction operator-(BigFraction lhs, BigFraction rhs);
    friend BigFraction operator*(BigFraction lhs, BigFraction rhs);
    friend BigFraction operator/(BigFraction lhs, BigFraction rhs);
    BigInteger get_whole();
    BigInteger get_remainder();
    BigInteger get_den() {return denominator_;}
    BigInteger get_num() {return numerator_;}

private:
    BigInteger numerator_;
    BigInteger denominator_;
};

class InputValidator {
public:
    InputValidator(string in, int p, int q): data_(in), p_(p), q_(q) {}
    bool is_valid();
    string get_error() {return error_msg_;}
private:
    bool DigitValidator(string num);
    string error_msg_;
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
    BaseParser(string s, int p);
    BigFraction get_num() {return number_;}
private:
    BigInteger get_digits_(string s);
    int get_len_(string s);
    BigInteger pow_p_(int exp);
    int p_;
    BigFraction number_;
};

class BaseFormatter {
public:
    static string convert(BigFraction value, int q);

private:
    static string digit_(int val, int base);
    static string int_to_q_(BigInteger num, int q);
};

class BaseConverter {
public:
    static string convert(string input, int p, int q) {
        BaseParser parser(input, p);
        BigFraction value = parser.get_num();
        return BaseFormatter::convert(value, q);
    }
};

#endif // UNIVERSALCONVERTER_H
