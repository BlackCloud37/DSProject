#include "searchtextbrowser.h"
#include <QDebug>
SearchTextBrowser::SearchTextBrowser(QWidget *parent) : QTextBrowser(parent)
{
    
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
/*
 * back/forward只支持setSource，对于setText的页面则不可。
 * 方法1，搜索结果保存到一个临时html
 * 方法2，再说8
*/
