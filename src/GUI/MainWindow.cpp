//
// Created by Reza Alizadeh Majd on 10/16/19.
//

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent){
    showMaximized();
    setWindowIcon(QIcon(":images/general/src/GUI/resources/panther"));
    setWindowTitle("PantherX Software Store");
    loadWindow(CONTENT_SECTIONS::STORE_LATEST);
}

MainWindow::~MainWindow() {
}
// --------------------------------------------------------------------------- signal-slot handlers
void MainWindow::settingsButtonHandler() {
    cout << "TBD - settingsButtonHandler" << endl;
}

void MainWindow::backButtonHandler() {
    int index = contentLayouts->currentIndex();
    if(index) {
        index--;
        contentLayouts->setCurrentIndex(index);
        reloadTopBar();
    }
}

void MainWindow::forwardButtonHandler() {
    int index = contentLayouts->currentIndex();
    if(index < contentLayouts->count()) {
        index++;
        contentLayouts->setCurrentIndex(index);
        reloadTopBar();
    }
}

void MainWindow::helpButtonHandler() {
    cout << "TBD - helpButtonHandler" << endl;
}

void MainWindow::leftPanelItemHandler(QListWidgetItem *item) {
    PxQListWidgetItem *listWidgetItem = (PxQListWidgetItem *) item;
    if(contentLayouts->currentIndex()==0){
        // if is in home clear all stacked widget
        int index = contentLayouts->count();
        while(index){
            QLayoutItem *item = contentLayouts->takeAt(index--);
            contentLayouts->removeItem(item);
            delete item;
        }
    }
    contentLayouts->addWidget(contentList->getItem(listWidgetItem->getId()));
    contentLayouts->setCurrentWidget(contentList->getItem(listWidgetItem->getId()));
    reloadTopBar();
}

void MainWindow::searchBoxHandler(){
    cout << "TBD - " << addressBar->text().toStdString() << endl;
}
// -------------------------------------------------------------------------------- ui form objects
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
    settingsButton->setIcon(QIcon(":/images/general/src/GUI/resources/settings"));
    backButton->setIcon(QIcon(":/images/general/src/GUI/resources/back"));
    forwardButton->setIcon(QIcon(":/images/general/src/GUI/resources/forward"));
    helpButton->setIcon(QIcon(":/images/general/src/GUI/resources/help"));
    addressBar->setPlaceholderText("Software/");
    addressBar->clearFocus();
    addressBar->showMaximized();

    QHBoxLayout *addressBarLayout = new QHBoxLayout;
    addressBarLayout->addWidget(addressBar);
    /// todo completer
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
    topMenuLayout->addLayout(addressBarLayout);
    topMenuLayout->addWidget(helpButton);
    topMenuLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    topMenuLayout->setSpacing(5);
    return topMenuLayout;
}
// ------------------------------------------------------------------------------ reload ui objects
void MainWindow::loadWindow(int id) {
    contentList = new ContentList();
    QListWidget *itemsList= contentList->getItemList();
    itemsList->setMaximumWidth(200);
    connect(itemsList, SIGNAL (itemClicked(QListWidgetItem*)), this, SLOT (leftPanelItemHandler(QListWidgetItem*)));

    contentLayouts = new QStackedLayout;
    contentLayouts->addWidget(contentList->getItem(id));
    contentLayouts->setCurrentIndex(0);

    QHBoxLayout *downLayout = new QHBoxLayout;
    downLayout->addWidget(itemsList);
    downLayout->addLayout(contentLayouts);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(loadTopMenu());
    mainLayout->addLayout(downLayout);

    window = new QWidget;
    window->setLayout(mainLayout);
    setCentralWidget(window);
    reloadTopBar();
}

void MainWindow::reloadTopBar(){
    QString address =   QString("Software/") +                              // home
                        ((PxQScrollArea *)(contentLayouts->currentWidget()))->getTitle() + QString("/");     // category
    addressBar->setPlaceholderText(address);
    if(contentLayouts->count()==1) {
        backButton->setDisabled(true);
        forwardButton->setDisabled(true);
    } else {
        backButton->setDisabled(false);
        forwardButton->setDisabled(false);
    }
}