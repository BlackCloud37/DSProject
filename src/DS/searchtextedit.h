#ifndef SEARCHTEXTEDIT_H
#define SEARCHTEXTEDIT_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QLineEdit>
class SearchTextEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit SearchTextEdit(QWidget *parent = nullptr);

signals:
    void sendWords(QList<QString>&);
public slots:
   void onSearchbuttonClicked();
   // void on_searchTextEdit_clicked();
};

#endif // SEARCHTEXTEDIT_H
