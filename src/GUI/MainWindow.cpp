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
void MainWindow::settingsButtonHandler() {
    cout << "TBD - settingsButtonHandler" << endl;
//
//    contentLayouts->setCurrentIndex(0);
//    int index = contentLayouts->count();
//    while(index){
//        QLayoutItem *item = contentLayouts->takeAt(index--);
//        contentLayouts->removeItem(item);
//        delete item;
//    }
//    reloadTopMenuStatus();
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

void MainWindow::helpButtonHandler() {
    cout << "TBD - helpButtonHandler" << endl;
}

void MainWindow::leftPanelItemHandler(QListWidgetItem *item) {
    PxQListWidgetItem *listWidgetItem = (PxQListWidgetItem *) item;
    reloadLayout(listWidgetItem->getTitle().toStdString());
}

void MainWindow::searchBoxHandler(){
    cout << "TBD - " << addressBar->text().toStdString() << endl;
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
    settingsButton = new QPushButton();
    backButton = new QPushButton();
    forwardButton = new QPushButton();
    helpButton = new QPushButton();
    addressBar =new QLineEdit();

    const QSize buttonSize = QSize(32, 32);
    settingsButton->setFixedSize(buttonSize);
    backButton->setFixedSize(buttonSize);
    forwardButton->setFixedSize(buttonSize);
    helpButton->setFixedSize(buttonSize);
    reloadTopMenuStatus();
    settingsButton->setIcon(QIcon::fromTheme(":images/general/src/GUI/resources/settings"));
    backButton->setIcon(QIcon::fromTheme(":images/general/src/GUI/resources/back"));
    forwardButton->setIcon(QIcon::fromTheme(":images/general/src/GUI/resources/forward"));
    helpButton->setIcon(QIcon::fromTheme(":images/general/src/GUI/resources/help"));
    addressBar->setPlaceholderText("Home");
    int w = width() - settingsButton->width() - backButton->width() - forwardButton->width() - helpButton->width() - 35; /// todo
    addressBar->setFixedWidth(w);
    /// Connect the "released" signal of buttons to it's slots (signal handler)
    connect(settingsButton, SIGNAL(released()), this, SLOT(settingsButtonHandler()));
    connect(backButton, SIGNAL (released()), this, SLOT (backButtonHandler()));
    connect(forwardButton, SIGNAL (released()), this, SLOT (forwardButtonHandler()));
    connect(helpButton, SIGNAL (released()), this, SLOT (helpButtonHandler()));
    connect(addressBar, SIGNAL(returnPressed()), this, SLOT(searchBoxHandler()));

    /// Create layout + add buttons
    QHBoxLayout *topMenuLayout = new QHBoxLayout();
    topMenuLayout->addWidget(settingsButton);
    topMenuLayout->addWidget(backButton);
    topMenuLayout->addWidget(forwardButton);
    topMenuLayout->addWidget(addressBar);
    topMenuLayout->addWidget(helpButton);
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
        backButton->setDisabled(true);
        forwardButton->setDisabled(true);
    } else {
        backButton->setDisabled(false);
        forwardButton->setDisabled(false);
    }
}
// ------------------------------------------------------------------------------------------------
QStringList MainWindow::getListStore() {
    QStringList list = {"Latest", "Recommended", "Categories"};
    return list;
}