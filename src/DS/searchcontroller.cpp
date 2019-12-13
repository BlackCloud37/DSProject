#include "searchcontroller.h"
#include <QDebug>
SearchController::SearchController(QObject *parent) : QObject(parent)
{
    s = nullptr;
}

void SearchController::setSolve(Solve *solve)
{
    s = solve;
}

void SearchController::parseInfoToString(std::ifstream &f, std::string &target)
{
    //TODO:定义解析规则
    std::string line;
    std::getline(f,line);
    target += "<a href='1.html'>";
    //target += "<a href='http://www.baidu.com'>";
    target += line;
    target += "</a><br>";
    
    while(std::getline(f,line)) {
        target += line;
        target += "<br>";
    }
    target += "<br>";
    
}

void SearchController::getWords(QList<QString>& words)
{
    qDebug() << "SearchController::getWords";
    MyList<CharString> list;
    for(auto word : words) {
        list.add(CharString::UTF8ToGB(word.toStdString().c_str()));
        qDebug() << word;
    }
    
    if (!s)
        return;
    
    std::string resultStr;
    
    auto queryResult = s->query(list);
    auto currQuery = queryResult.headPtr();
    while(currQuery) {
        int docId = currQuery->elem().docId;
        std::string filename = "C:\\Users\\zml05\\Desktop\\output\\" + std::to_string(docId) + ".info";
        std::ifstream f(filename);
        if (!f.is_open()) {
            currQuery = currQuery->next();
            continue;
        }
        qDebug() << "file opened";

        parseInfoToString(f,resultStr);
        
        f.close();
        currQuery = currQuery->next();
    }
    
    std::ofstream of("./temp.html",std::ios::out|std::ios::trunc);
    of << char(0xEF) << char(0xBB) << char(0xBF) << resultStr;
    //of << resultStr;
    of.close();
    emit sourceChanged(QUrl("./temp.html"));
   //emit sendText(QString::fromStdString(resultStr));
}

