#include "universalconverter.h"
#include "BigFraction.h"
#include "InputValidator.h"
#include "DigitTokenizer.h"
#include "BaseParser.h"
#include "BaseFormatter.h"
#include "ui_universalconverter.h"

UniversalConverter::UniversalConverter(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::UniversalConverter)
{
    ui->setupUi(this);
    ui->output->setReadOnly(true);
    ui->error_line->setReadOnly(true);
    ui->output->setAlignment(Qt::AlignLeft);
    ui->error_line->setAlignment(Qt::AlignLeft);

    connect(ui->convert, &QPushButton::clicked, this, &UniversalConverter::on_convert_clicked);
    connect(ui->file_input, &QPushButton::clicked, this, &UniversalConverter::on_file_input_clicked);
}

UniversalConverter::~UniversalConverter()
{
    delete ui;
}

void UniversalConverter::show_error_(string msg) {
    ui->error_line->setText(QString::fromStdString(msg));
    ui->error_line->setCursorPosition(0);
    ui->output->clear();
}

void UniversalConverter::on_convert_clicked() {
    bool p_ok = false, q_ok = false;
    int p = ui->p->text().toInt(&p_ok);
    int q = ui->q->text().toInt(&q_ok);
    if (!p_ok || !q_ok) {
        show_error_("Основания p и q должны быть целыми числами");
        return;
    }

    string input = ui->input->text().toStdString();

    InputValidator validator(input, p, q);
    if (!validator.is_valid()) {
        show_error_(validator.get_error());
        return;
    }

    string res = BaseConverter::convert(input, p, q);
    ui->output->setText(QString::fromStdString(res));
    ui->output->setCursorPosition(0);
    ui->error_line->clear();

    if (ui->checkBox->isChecked()) {
        save_to_file_(res);
    }
}

void UniversalConverter::on_file_input_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, "Выберите файл с исходными данными",
                                                    "", "Текстовые файлы (*.txt)");
    if (fileName.isEmpty()) return;

    std::ifstream file(fileName.toStdString());
    if (!file.is_open()) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл");
        return;
    }

    string p_str, q_str, input_str;
    if (!std::getline(file, p_str) || !std::getline(file, q_str) || !std::getline(file, input_str)) {
        QMessageBox::warning(this, "Ошибка", "Файл должен содержать три строки: p, q, input");
        return;
    }

    for (auto* s : {&p_str, &q_str, &input_str}) {
        s->erase(s->find_last_not_of(" \t\r\n") + 1);
    }

    ui->p->setText(QString::fromStdString(p_str));
    ui->q->setText(QString::fromStdString(q_str));
    ui->input->setText(QString::fromStdString(input_str));

    on_convert_clicked();
}

void UniversalConverter::save_to_file_(string result) {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить результат",
                                                    "output.txt", "Текстовые файлы (*.txt)");
    if (fileName.isEmpty()) return;

    std::ofstream out(fileName.toStdString());
    if (!out.is_open()) {
        show_error_("Не удалось сохранить файл");
        return;
    }
    out << result;
    out.close();
    ui->error_line->setText(QString("Результат сохранён в ") + fileName);
    ui->error_line->setCursorPosition(0);
}
