#ifndef SEARCHTEXTBROWSER_H
#define SEARCHTEXTBROWSER_H

#include <QWidget>
#include <QTextBrowser>
class SearchTextBrowser : public QTextBrowser
{
    Q_OBJECT
public:
    explicit SearchTextBrowser(QWidget *parent = nullptr);

signals:

public slots:
    void getText(QString text);
    void getSource(QUrl url);
};

#endif // SEARCHTEXTBROWSER_H
