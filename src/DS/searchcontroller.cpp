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

void SearchController::parseInfoToString(int docId, QList<QString>& words,std::ifstream &f, std::string &target)
{
    //TODO:定义解析规则
    std::string line;
    std::getline(f,line);
    
    target += "<a href='";
    target += std::to_string(docId);
    target += ".html' style='font-size: 20px;'>";
    target += highLight(words, line);
    target += "</a><br>";
    
    while(std::getline(f,line)) {
        target += highLight(words, line);
        target += "<br>";
    }
    target += "<br>";
}

std::string SearchController::highLight(QList<QString>& words, CharString str)
{
    for(auto each: words) {
        CharString sg = u8"<font color='red'>" + each.toStdString() + u8"</font>";
        str = CharString::join(str.split(CharString(each.toStdString()),true),sg);
    }
    return str.toStr();
}

void SearchController::generateHtml(int docId)
{
    std::string filename = "./output/" + std::to_string(docId) + ".info";
    std::ifstream f(filename);
    if (!f.is_open()) {
        return;
    }
    
    std::string line,target;
    std::getline(f,line);
    

    target += "<h1>";
    target += line;
    target += "</h1>";
    
    while(std::getline(f,line)) {
        target += line;
        target += "<br>";
    }
    target += "<br>";
    f.close();
    target += "<h1>推荐电影</h1>";
    
    DocList recommendlist;
    int count = 0;
    s->recommend(docId, recommendlist);
    
    auto currRecommend = recommendlist.headPtr();
    while(currRecommend && count < 5) {
        int currDocId = currRecommend->elem().docId;
        std::ifstream docFile("./output/" + std::to_string(currDocId) + ".info");
        assert(docFile.is_open());
        if (!docFile.is_open()) {
            currRecommend = currRecommend->next();
            continue;
        }
        count ++;
        std::getline(docFile,line);
        
        target += "<h2>";
        target += line;
        target += "</h2>";
        
        while(std::getline(docFile,line)) {
            target += line;
            target += "<br>";
        }
        target += "<br>";
        docFile.close();
        currRecommend = currRecommend->next();
    }
    
    std::ofstream out("./html/" + std::to_string(docId) + ".html",std::ios::trunc);
    out << char(0xEF) << char(0xBB) << char(0xBF)<< target;
    out.close();
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
    DocList queryResult;
    s->query(list,queryResult);
    auto currQuery = queryResult.headPtr();
    while(currQuery) {
        int docId = currQuery->elem().docId;
        std::string filename = "./output/" + std::to_string(docId) + ".info";
        
        std::ifstream f(filename);
        if (!f.is_open()) {
            currQuery = currQuery->next();
            continue;
        }

        parseInfoToString(docId, words, f, resultStr);
        //generateHtml(docId);
        f.close();
    
        currQuery = currQuery->next();
    }
    
    std::ofstream of("./html/temp.html",std::ios::out|std::ios::trunc);
    of << char(0xEF) << char(0xBB) << char(0xBF) << resultStr;
    of.close();
    emit sourceChanged(QUrl("./html/temp.html"));
}

