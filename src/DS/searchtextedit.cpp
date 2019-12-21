#include "searchtextedit.h"
#include <QDebug>
SearchTextEdit::SearchTextEdit(QWidget *parent) : QLineEdit(parent) {
    setPlaceholderText(tr("输入搜索关键词,必须以空格分隔"));
}

void SearchTextEdit::onSearchbuttonClicked()
{
    qDebug() << "SearchTextEdit::onSearchbuttonClicked";
    auto text = this->text();
    auto list = text.split(" ");
    emit sendWords(list);
}
