#ifndef SEARCHCONTROLLER_H
#define SEARCHCONTROLLER_H

#include <QObject>
#include <QUrl>
#include "../MovieRetrieve/Solve.cpp"
#include "../MovieRetrieve/DocList.h"
class SearchController : public QObject
{
    Q_OBJECT
public:
    explicit SearchController(QObject *parent = nullptr);
    void setSolve(Solve* solve);
    void parseInfoToString(std::ifstream& f, std::string& target);
signals:
    void sendText(QString);
    void sourceChanged(QUrl);
public slots:
    void getWords(QList<QString>& words);
    
private:
    Solve* s;

};

#endif // SEARCHCONTROLLER_H
