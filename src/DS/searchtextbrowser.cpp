#include "searchtextbrowser.h"
#include <QDebug>
#include <QTextCodec>
SearchTextBrowser::SearchTextBrowser(QWidget *parent) : QTextBrowser(parent)
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));
}

void SearchTextBrowser::getText(QString text)
{
    qDebug() << "SearchTextBrowser::getText";
    this->setText(text);
}

void SearchTextBrowser::getSource(QUrl url)
{
    this->clear();
    this->setSource(url);
}
