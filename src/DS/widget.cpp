#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    
    s = new Solve();
    this->resize(800,600);

    searchLayout = new QHBoxLayout();
    searchEdit = new SearchTextEdit(this);
    searchButton = new QPushButton(this);
    searchButton->setText("Search");
    backButton = new QPushButton(this);
    backButton->setText("back");
    forwardButton = new QPushButton(this);
    forwardButton->setText("forward");
    homeButton = new QPushButton(this);
    homeButton->setText("home");
    
    searchLayout->addWidget(searchEdit);
    searchLayout->addWidget(searchButton);
    searchLayout->addWidget(backButton);
    searchLayout->addWidget(forwardButton);
    searchLayout->addWidget(homeButton);


    
    searchResult = new SearchTextBrowser(this);
    searchResult->setText("<h1>helloworld</h1><p style='color:red;padding-top:50px;'>测试</p>");
    searchResult->setSearchPaths(QStringList() << ":/html/html/");
    searchResult->setOpenLinks(true);
    //searchResult->setOpenExternalLinks(true);
    
    allLayout = new QVBoxLayout(this);
    allLayout->addLayout(searchLayout,1);
    allLayout->addWidget(searchResult,20);


    searchController = new SearchController(this);
    searchController->setSolve(s);
    s->createAVLDictionary();
    
    connect(searchButton, &QPushButton::clicked, searchEdit, &SearchTextEdit::onSearchbuttonClicked);
    connect(backButton, &QPushButton::clicked, searchResult, &SearchTextBrowser::backward);
    connect(forwardButton, &QPushButton::clicked, searchResult, &SearchTextBrowser::forward);
    connect(homeButton, &QPushButton::clicked, searchResult, &SearchTextBrowser::home);
    
    
    connect(searchEdit, &SearchTextEdit::sendWords, searchController, &SearchController::getWords);
    connect(searchController, &SearchController::sendText , searchResult, &SearchTextBrowser::getText);
    connect(searchController, &SearchController::sourceChanged , searchResult, &SearchTextBrowser::getSource);
    
    
}

Widget::~Widget()
{
    if (s)
        delete s;
}

