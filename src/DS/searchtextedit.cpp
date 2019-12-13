#include "searchtextedit.h"
#include <QDebug>
SearchTextEdit::SearchTextEdit(QWidget *parent) : QLineEdit(parent) {

}

void SearchTextEdit::onSearchbuttonClicked()
{
    qDebug() << "SearchTextEdit::onSearchbuttonClicked";
    auto text = this->text();
    auto list = text.split(" ");
    emit sendWords(list);
}
