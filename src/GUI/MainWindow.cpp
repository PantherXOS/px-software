//
// Created by Reza Alizadeh Majd on 10/16/19.
//

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent){
    setFixedWidth(800);
    setFixedHeight(600);
    setWindowIcon(QIcon(":images/general/src/GUI/resources/panther"));
    setWindowTitle("PantherX Software Store");
//    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    loadWindow("todo");
}

MainWindow::~MainWindow() {
}
// --------------------------------------------------------------------------- signal-slot handlers
void MainWindow::homeButtonHandler() {
    contentLayouts->setCurrentIndex(0);
    int index = contentLayouts->count();
    while(index){
        QLayoutItem *item = contentLayouts->takeAt(index--);
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

void MainWindow::leftPanelItemHandler(QListWidgetItem *item) {
    PxQListWidgetItem *listWidgetItem = (PxQListWidgetItem *) item;
    reloadLayout(listWidgetItem->getTitle().toStdString());
}

void MainWindow::searchBoxHandler(){
    cout << searchBox->text().toStdString() << endl;
}
// -------------------------------------------------------------------------------- ui form objects
QListWidget *MainWindow::loadLeftPanel() {
    QListWidget *list = new QListWidget();
    list->setFixedSize(width()/4,height()-16); /// todo
    QFont itemFonts("default", 12,QFont::Bold);
    QFont subitemFonts("default", 11);
    QSize seperatorSize = QSize(64, 6);
    QSize iconSize = QSize(16,16);
    /// Create and add a seperator to list
    QListWidgetItem *seperatorItem1= new QListWidgetItem();
    seperatorItem1->setSizeHint(seperatorSize);
    seperatorItem1->setFlags(Qt::NoItemFlags);
    list->addItem(seperatorItem1);
    list->setSpacing(4);
    list->setIconSize(iconSize);
    //-----------------------------------------------------------------
    PxQListWidgetItem *storeItem= new PxQListWidgetItem(QString("STORE"),itemFonts);
    storeItem->setFlags(Qt::NoItemFlags);
    list->addItem(storeItem);

    QStringList storeList = getListStore();
    for (auto itemName : storeList){
        PxQListWidgetItem *item=new PxQListWidgetItem(itemName,subitemFonts,QIcon(":images/general/src/GUI/resources/items"));
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

    PxQListWidgetItem *installedAppItem= new PxQListWidgetItem(QString("Installed"),subitemFonts,QIcon(":images/general/src/GUI/resources/items"));
    list->addItem(installedAppItem);

    PxQListWidgetItem *updateAppItem= new PxQListWidgetItem(QString("Updates"),subitemFonts,QIcon(":images/general/src/GUI/resources/update"));
    list->addItem(updateAppItem);
    QListWidgetItem *seperatorItem3= new QListWidgetItem();
    seperatorItem3->setSizeHint(seperatorSize);
    seperatorItem3->setFlags(Qt::NoItemFlags);
    list->addItem(seperatorItem3);
    //-----------------------------------------------------------------
    PxQListWidgetItem *systemItem= new PxQListWidgetItem(QString("SYSTEM"),itemFonts);
    systemItem->setFlags(Qt::NoItemFlags);
    list->addItem(systemItem);

    PxQListWidgetItem *systemUpdateItem= new PxQListWidgetItem(QString("Updates"),subitemFonts,QIcon(":images/general/src/GUI/resources/update"));
    list->addItem(systemUpdateItem);

    connect(list, SIGNAL (itemClicked(QListWidgetItem*)), this, SLOT (leftPanelItemHandler(QListWidgetItem*)));

    list->setAutoFillBackground(false);
    list->setStyleSheet("background-color: transparent;");
    return list;
}

QWidget * MainWindow::loadContent(string section) {
    QWidget * widget = new QWidget;
    QGridLayout *layout = new QGridLayout;
    if(section == "Categories"){
        cout << "Categories" << endl;
    }
    layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    widget->setLayout(layout);
    return widget;
}

QHBoxLayout *MainWindow::loadTopMenu() {
    homeButton = new QPushButton();
    backButton = new QPushButton();
    forwardButton = new QPushButton();
    searchButton = new QPushButton();
    addressBar = new QLabel();
    searchBox = new QLineEdit();

    const QSize buttonSize = QSize(32, 32);
    homeButton->setFixedSize(buttonSize);
    backButton->setFixedSize(buttonSize);
    forwardButton->setFixedSize(buttonSize);
    searchButton->setFixedSize(buttonSize);
    reloadTopMenuStatus();
    homeButton->setIcon(QIcon::fromTheme(":images/general/src/GUI/resources/home"));
    backButton->setIcon(QIcon::fromTheme(":images/general/src/GUI/resources/back"));
    forwardButton->setIcon(QIcon::fromTheme(":images/general/src/GUI/resources/forward"));
    searchButton->setIcon(QIcon::fromTheme(":images/general/src/GUI/resources/search"));
    addressBar->setText("Home");
    searchBox->setPlaceholderText("Search ...");
    addressBar->showMaximized();
    int w = width() - homeButton->width() - backButton->width() - forwardButton->width() - addressBar->width() - 35; /// todo
    searchBox->setFixedWidth(w);
    /// Connect the "released" signal of buttons to it's slots (signal handler)
    connect(homeButton, SIGNAL(released()), this, SLOT(homeButtonHandler()));
    connect(backButton, SIGNAL (released()), this, SLOT (backButtonHandler()));
    connect(forwardButton, SIGNAL (released()), this, SLOT (forwardButtonHandler()));
    connect(searchBox, SIGNAL(returnPressed()), this, SLOT(searchBoxHandler()));

    /// Create layout + add buttons
    QHBoxLayout *topMenuLayout = new QHBoxLayout();
    topMenuLayout->addWidget(homeButton);
    topMenuLayout->addWidget(backButton);
    topMenuLayout->addWidget(forwardButton);
    topMenuLayout->addWidget(addressBar);
    topMenuLayout->addWidget(searchButton);
    topMenuLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    topMenuLayout->setSpacing(5);
    return topMenuLayout;
}
// ------------------------------------------------------------------------------ reload ui objects
void MainWindow::reloadLayout(string section) {
    QWidget *newContent = loadContent(section);
    contentLayouts->addWidget(newContent);
    contentLayouts->setCurrentWidget(newContent);
    reloadTopMenuStatus();
}

void MainWindow::loadWindow(string section) {
    contentLayouts = new QStackedLayout;
    contentLayouts->addWidget(loadContent(section));
    contentLayouts->setCurrentIndex(0);

    QHBoxLayout *downLayout = new QHBoxLayout;
    downLayout->addWidget(loadLeftPanel());
    downLayout->addLayout(contentLayouts);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(loadTopMenu());
    mainLayout->addLayout(downLayout);

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
// ------------------------------------------------------------------------------------------------
QStringList MainWindow::getListStore() {
    QStringList list = {"Latest", "Recommended", "Categories"};
    return list;
}