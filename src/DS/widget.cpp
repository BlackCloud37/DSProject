#include "widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    
    s = new Solve();
    this->resize(800,600);

    searchLayout = new QHBoxLayout();
    searchEdit = new SearchTextEdit(this);
    searchButton = new QPushButton(this);
    searchButton->setText("Search");
    searchButton->setShortcut(Qt::Key_Enter);
    backButton = new QPushButton(this);
    backButton->setText("Back");
    forwardButton = new QPushButton(this);
    forwardButton->setText("Forward");

    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(backButton);
    searchLayout->addWidget(forwardButton);


    
    searchResult = new SearchTextBrowser(this);
    searchResult->setSearchPaths(QStringList() << "./html");

    allLayout = new QVBoxLayout(this);
    allLayout->addLayout(searchLayout,1);
    allLayout->addWidget(searchResult,20);


    searchController = new SearchController(this);
    searchController->setSolve(s);
    s->createAVLDictionary();
    s->createRecommendTree();
    
    connect(searchButton, &QPushButton::clicked, searchEdit, &SearchTextEdit::onSearchbuttonClicked);
    connect(backButton, &QPushButton::clicked, searchResult, &SearchTextBrowser::backward);
    connect(forwardButton, &QPushButton::clicked, searchResult, &SearchTextBrowser::forward);
 
    
    connect(searchEdit, &SearchTextEdit::sendWords, searchController, &SearchController::getWords);
    connect(searchController, &SearchController::sendText , searchResult, &SearchTextBrowser::getText);
    connect(searchController, &SearchController::sourceChanged , searchResult, &SearchTextBrowser::getSource);
    
    
}

Widget::~Widget()
{
    if (s)
        delete s;
}

