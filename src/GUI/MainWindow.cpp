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
    reloadContent(contentList->getItem(listWidgetItem->getId()));
}

void MainWindow::searchBoxHandler(){
    cout << searchBox->text().toStdString() << endl;
}
// -------------------------------------------------------------------------------- ui form objects
QListWidget *MainWindow::loadLeftPanel() {
    contentList = new ContentList(width(),height());
    QListWidget *list= contentList->getItemList();
    connect(list, SIGNAL (itemClicked(QListWidgetItem*)), this, SLOT (leftPanelItemHandler(QListWidgetItem*)));
    return list;
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
void MainWindow::reloadContent(QWidget *section) {
    contentLayouts->addWidget(section);
    contentLayouts->setCurrentWidget(section);
    reloadTopMenuStatus();
}

void MainWindow::loadWindow(string section) {
    QHBoxLayout *downLayout = new QHBoxLayout;
    downLayout->addWidget(loadLeftPanel());
    contentLayouts = new QStackedLayout;
    contentLayouts->addWidget(contentList->getItem(CONTENT_SECTIONS::STORE_LATEST));
    contentLayouts->setCurrentIndex(0);
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