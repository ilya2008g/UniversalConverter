#include "BigFraction.h"

#ifndef INPUTVALIDATOR_H
#define INPUTVALIDATOR_H
bool InputValidator::DigitValidator(string num) {
    for (int i = 0; i < (int)num.size();) {
        if (num[i] == '[') {
            size_t j = num.find(']', i + 1);
            if (j == string::npos) {
                error_msg_ = "Найдена незакрытая квадратная скобка.";
                return false;
            }
            string number = num.substr(i + 1, j - i - 1);
            if (number.empty()) {
                error_msg_ = "Квадратные скобки не должны быть пустыми.";
                return false;
            }
            if (!all_of(number.begin(), number.end(), ::isdigit)) {
                error_msg_ = "Внутри квадратных скобок должны быть только цифры 0-9.";
                return false;
            }

            int val = 0;
            for (char c : number) {
                int dig = c - '0';
                val = val * 10 + dig;
                if (val >= 500) {
                    error_msg_ = "Слишком большое число в кв. скобках.";
                    return false;
                }
            }
            if (val >= p_) {
                error_msg_ = "Цифра [" + number + "] недопустима в СС с основанием " + std::to_string(p_);
                return false;
            }
            i = j + 1;
        } else {
            char c = num[i];
            int val = -1;
            if (c >= '0' && c <= '9')
                val = c - '0';
            else if (c >= 'A' && c <= 'Z')
                val = 10 + (c - 'A');
            else if (c >= 'a' && c <= 'z')
                val = 10 + (c - 'a');
            else {
                error_msg_ = std::string("Недопустимый символ '") + c + "' в цифровой последовательности";
                return false;
            }
            if (val >= p_) {
                error_msg_ = std::string("Цифра '") + c + "' недопустима в CC с основанием " + std::to_string(p_);
                return false;
            }
            ++i;
        }
    }
    return true;
}

bool InputValidator::is_valid() {
    if (p_ < 2 || p_ > 500 || q_ < 2 || q_ > 500) {
        error_msg_ = "Основания систем счислений должны быть целым числом от 2 до 500.";
        return false;
    }
    int dots = count(data_.begin(), data_.end(), '.');
    if (dots > 1) {
        error_msg_ = "В числе не должно присутствовать более 1 точки.";
        return false;
    }

    for (char c : data_) {
        if (!isalnum(c) && c != '.' && c != '(' && c != ')' && c != '[' && c != ']') {
            error_msg_ = std::string("Недопустимый символ '") + c + "' в строке";
            return false;
        }
    }

    if (dots == 0) {
        if (!DigitValidator(data_)) {
            return false;
        }
        if (data_.size() > 1 && data_[0] == '0') {
            error_msg_ = "Ведущие нули в целой части недопустимы";
            return false;
        }
        return true;
    }

    size_t dot = data_.find('.');
    string int_part = data_.substr(0, dot);
    string frac_part = data_.substr(dot + 1);

    if (int_part.empty()) {
        error_msg_ = "Целая часть отсутствует (ожидается хотя бы одна цифра до точки)";
        return false;
    }
    if (frac_part.empty()) {
        error_msg_ = "Дробная часть отсутствует (ожидается хотя бы одна цифра после точки)";
        return false;
    }

    if (!DigitValidator(int_part)) {
        return false;
    }
    if (int_part.size() > 1 && int_part[0] == '0') {
        error_msg_ = "Ведущие нули в целой части недопустимы";
        return false;
    }

    size_t open = frac_part.find('(');
    if (open != string::npos) {
        size_t close = frac_part.find(')', open + 1);
        if (close == std::string::npos) {
            error_msg_ = "Не найдена закрывающая скобка периода ')'";
            return false;
        }
        if (frac_part.find('(', open + 1) != std::string::npos) {
            error_msg_ = "Период не может содержать более одной открывающей скобки";
            return false;
        }
        if (frac_part.find(')', close + 1) != std::string::npos) {
            error_msg_ = "После закрывающей скобки периода не может быть других скобок или цифр";
            return false;
        }
        if (close != frac_part.size() - 1) {
            error_msg_ = "После закрывающей скобки периода не должно быть других символов";
            return false;
        }

        string non_per = frac_part.substr(0, open);
        string per = frac_part.substr(open + 1, close - open - 1);
        if (per.empty()) {
            error_msg_ = "Период не может быть пустым";
            return false;
        }

        if (!DigitValidator(non_per) || !DigitValidator(per)) return false;
    } else {
        if (!DigitValidator(frac_part)) return false;
    }
    return true;
}
#endif // INPUTVALIDATOR_H
