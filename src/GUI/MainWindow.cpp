//
// Created by Reza Alizadeh Majd on 10/16/19.
//

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent){
    setFixedWidth(800);
    setFixedHeight(600);
    setWindowIcon(QIcon::fromTheme("software-store"));
    setWindowTitle("PantherX Software Store");
//    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    loadWindow("todo");
}

MainWindow::~MainWindow() {
//    delete window;
}

QHBoxLayout *MainWindow::loadLeftTopMenu() {
    QHBoxLayout * menu = new QHBoxLayout();
    QLabel *temp = new QLabel();
//    menu->addWidget(temp);
    return menu;
}

QHBoxLayout *MainWindow::loadRightTopMenu() {
    homeButton = new QPushButton();
    backButton = new QPushButton();
    forwardButton = new QPushButton();
    addressBar = new QLabel();
    const QSize buttonSize = QSize(32, 32);
    homeButton->setFixedSize(buttonSize);
    backButton->setFixedSize(buttonSize);
    forwardButton->setFixedSize(buttonSize);
    reloadTopMenuStatus();
    homeButton->setIcon(QIcon::fromTheme("go-home"));
    backButton->setIcon(QIcon::fromTheme("go-previous"));
    forwardButton->setIcon(QIcon::fromTheme("go-next"));
    addressBar->setText("Home");
    /// Connect the "released" signal of buttons to it's slots (signal handler)
    connect(homeButton, SIGNAL(released()), this, SLOT(homeButtonHandler()));
    connect(backButton, SIGNAL (released()), this, SLOT (backButtonHandler()));
    connect(forwardButton, SIGNAL (released()), this, SLOT (forwardButtonHandler()));
    /// Create layout + add buttons
    QHBoxLayout *topMenuLayout = new QHBoxLayout();
    topMenuLayout->addWidget(homeButton);
    topMenuLayout->addWidget(backButton);
    topMenuLayout->addWidget(forwardButton);
    topMenuLayout->addWidget(addressBar);
    topMenuLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    topMenuLayout->setSpacing(5);
    return topMenuLayout;
}

void MainWindow::homeButtonHandler() {
    contentLayouts->setCurrentIndex(0);
    int index = contentLayouts->count();
    for(int i=1; i<index ;i++) {
        QLayoutItem *item = contentLayouts->takeAt(i);
        contentLayouts->removeItem(item);
        delete item;
    }
    reloadTopMenuStatus();
}

void MainWindow::backButtonHandler() {
    int index = contentLayouts->currentIndex();
    if(index) index--;
    contentLayouts->setCurrentIndex(index);
    reloadTopMenuStatus();
}

void MainWindow::forwardButtonHandler() {
    int index = contentLayouts->currentIndex();
    if(index < contentLayouts->count()) index++;
    contentLayouts->setCurrentIndex(index);
    reloadTopMenuStatus();
}

QListWidget *MainWindow::loadLeftPanel() {
    QListWidget *list = new QListWidget();
    list->setFixedWidth(width()/4);
    QFont itemFonts("default", 12,QFont::Bold);
    QFont subitemFonts("default", 11);
    QSize seperatorSize = QSize(64, 6);
    /// Create and add a seperator to list
    QListWidgetItem *seperatorItem1= new QListWidgetItem();
    seperatorItem1->setSizeHint(seperatorSize);
    seperatorItem1->setFlags(Qt::NoItemFlags);
    list->addItem(seperatorItem1);
    list->setSpacing(4);
    //-----------------------------------------------------------------
    PxQListWidgetItem *storeItem= new PxQListWidgetItem(QString("STORE"),itemFonts);
    storeItem->setFlags(Qt::NoItemFlags);
    list->addItem(storeItem);

    QStringList storeList = getListStore();
    for (auto itemName : storeList){
        PxQListWidgetItem *item=new PxQListWidgetItem(itemName,subitemFonts);
        item->setIcon(QIcon(":images/general/src/GUI/resources/items"));
        list->addItem(item);
    }
    QListWidgetItem *seperatorItem2= new QListWidgetItem();
    seperatorItem2->setSizeHint(seperatorSize);
    seperatorItem2->setFlags(Qt::NoItemFlags);
    list->addItem(seperatorItem2);
    //-----------------------------------------------------------------
    PxQListWidgetItem *yourAppsItem= new PxQListWidgetItem(QString("YOURS APPS"),itemFonts);
    yourAppsItem->setFlags(Qt::NoItemFlags);
    list->addItem(yourAppsItem);

    PxQListWidgetItem *installedAppItem= new PxQListWidgetItem(QString("Installed"),subitemFonts);
    list->addItem(installedAppItem);

    PxQListWidgetItem *updateAppItem= new PxQListWidgetItem(QString("Updates"),subitemFonts);
    list->addItem(updateAppItem);
    QListWidgetItem *seperatorItem3= new QListWidgetItem();
    seperatorItem3->setSizeHint(seperatorSize);
    seperatorItem3->setFlags(Qt::NoItemFlags);
    list->addItem(seperatorItem3);
    //-----------------------------------------------------------------
    PxQListWidgetItem *systemItem= new PxQListWidgetItem(QString("SYSTEM"),itemFonts);
    systemItem->setFlags(Qt::NoItemFlags);
    list->addItem(systemItem);

    PxQListWidgetItem *systemUpdateItem= new PxQListWidgetItem(QString("Updates"),subitemFonts);
    list->addItem(systemUpdateItem);

    connect(list, SIGNAL (itemClicked(QListWidgetItem*)), this, SLOT (leftPanelItemHandler(QListWidgetItem*)));

    list->setAutoFillBackground(false);
    list->setStyleSheet("background-color: transparent;");

    return list;
}

void MainWindow::leftPanelItemHandler(QListWidgetItem *item) {
    PxQListWidgetItem *listWidgetItem = (PxQListWidgetItem *) item;
    reloadLayout(listWidgetItem->getTitle().toStdString());
}

QWidget * MainWindow::loadContent(string section) {
    QWidget * widget = new QWidget;
    QGridLayout *layout = new QGridLayout;
    if(section == "Categories"){
        for(int i=0; i<4 ; i++){
            CategoryLayout *cat1 =  new CategoryLayout();
            layout->addWidget(cat1,int(i/2),i%2);
        }
    }
    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    widget->setLayout(layout);
    return widget;
}

QStringList MainWindow::getListStore() {
    QStringList list = {"Latest", "Recommended", "Categories"};
    return list;
}

void MainWindow::reloadLayout(string section) {
    QWidget *newContent = loadContent(section);
    contentLayouts->addWidget(newContent);
    contentLayouts->setCurrentWidget(newContent);
    reloadTopMenuStatus();
}

void MainWindow::loadWindow(string section) {
    QVBoxLayout *rightSideLayout = new QVBoxLayout();
    contentLayouts = new QStackedLayout;
    contentLayouts->addWidget(loadContent(section));
    contentLayouts->setCurrentIndex(0);

    QHBoxLayout *rightTopMenu = loadRightTopMenu();
    rightSideLayout->addLayout(rightTopMenu);
    rightSideLayout->addLayout(contentLayouts);

    QVBoxLayout *leftSideLayout = new QVBoxLayout();
    leftSideLayout->addLayout(loadLeftTopMenu());
    leftSideLayout->addWidget(loadLeftPanel());


    QHBoxLayout *mainLayout = new QHBoxLayout();
    mainLayout->addLayout(leftSideLayout);
    mainLayout->addLayout(rightSideLayout);

//    window = new QStackedWidget;
//    window->insertWidget(0,homePage);
//    window->setCurrentIndex(0);
    window = new QWidget;
    QScrollArea *scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(window);
    window->setLayout(mainLayout);
    setCentralWidget(window);
}

void MainWindow::reloadTopMenuStatus(){
    if(contentLayouts->count()==1){
        homeButton->setDisabled(true);
        backButton->setDisabled(true);
        forwardButton->setDisabled(true);
    } else {
        homeButton->setDisabled(false);
        backButton->setDisabled(false);
        forwardButton->setDisabled(false);
    }
}