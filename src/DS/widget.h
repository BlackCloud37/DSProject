#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTextEdit>
#include <QDockWidget>
#include <QList>
#include <QLayout>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextBrowser>
#include "searchtextbrowser.h"
#include "searchtextedit.h"
#include "searchcontroller.h"
#include "../MovieRetrieve/Solve.h"

QT_BEGIN_NAMESPACE
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
public slots:

private:
    Solve* s;
    //QList<QDockWidget*> tabs;
    QVBoxLayout* allLayout;

    QHBoxLayout* searchLayout;
    SearchTextEdit* searchEdit;
    QPushButton* searchButton;
    
    QPushButton* backButton;
    QPushButton* forwardButton;
    QPushButton* homeButton;

    SearchTextBrowser* searchResult;

    SearchController* searchController;
};
#endif // WIDGET_H
