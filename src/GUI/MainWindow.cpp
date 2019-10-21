//
// Created by Reza Alizadeh Majd on 10/16/19.
//

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent){


    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addItem(loadTopMenu());

    QWidget *homePage = new QWidget();
    homePage->setWindowState(Qt::WindowMaximized);
    homePage->setLayout(mainLayout);

    window = new QStackedWidget;
    window->insertWidget(0,homePage);
    window->setCurrentIndex(0);
    setCentralWidget(window);
}

MainWindow::~MainWindow() {
    delete window;
}

QHBoxLayout *MainWindow::loadTopMenu() {
    QPushButton *homeButton = new QPushButton();
    QPushButton *backButton = new QPushButton();
    QPushButton *forwardButton = new QPushButton();
    const QSize buttonSize = QSize(32, 32);
    homeButton->setFixedSize(buttonSize);
    backButton->setFixedSize(buttonSize);
    forwardButton->setFixedSize(buttonSize);
    homeButton->setDisabled(true);
    backButton->setDisabled(true);
    forwardButton->setDisabled(true);
    homeButton->setIcon(QIcon::fromTheme("go-home"));
    backButton->setIcon(QIcon::fromTheme("go-previous"));
    forwardButton->setIcon(QIcon::fromTheme("go-next"));
    /// Connect the "released" signal of buttons to it's slots (signal handler)
    connect(homeButton, SIGNAL(released()), this, SLOT(homeButtonHandler()));
    connect(backButton, SIGNAL (released()), this, SLOT (backButtonHandler()));
    connect(forwardButton, SIGNAL (released()), this, SLOT (forwardButtonHandler()));
    /// Create layout + add buttons
    QHBoxLayout *topMenuLayout = new QHBoxLayout();
    topMenuLayout->addWidget(homeButton);
    topMenuLayout->addWidget(backButton);
    topMenuLayout->addWidget(forwardButton);
    topMenuLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    topMenuLayout->setSpacing(5);
    return topMenuLayout;
}

void MainWindow::homeButtonHandler() {
    window->setCurrentIndex(0);
}

void MainWindow::backButtonHandler() {
    int index = window->currentIndex();
    if(index) index--;
    window->setCurrentIndex(index);
}

void MainWindow::forwardButtonHandler() {
    int index = window->currentIndex();
    if(index < window->count()) index++;
    window->setCurrentIndex(index);
}
