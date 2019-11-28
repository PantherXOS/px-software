//
// Created by Reza Alizadeh Majd on 10/16/19.
//

#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent){
    showMaximized();
    setWindowIcon(QIcon(":images/general/src/GUI/resources/panther"));
    setWindowTitle("PantherX Software");
    loadWindow(CONTENT_SECTIONS::STORE_LATEST);
}

MainWindow::~MainWindow() {
}
// --------------------------------------------------------------------------- signal-slot handlers
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QWidget * const widget = childAt(event->pos());

    CategoryWidget * categoryWidget = qobject_cast<CategoryWidget*>(widget->parentWidget());
    PackageListWidgetItem *packageWidget = qobject_cast<PackageListWidgetItem*>(widget->parentWidget());
    if(widget){
//        if(!categoryWidget)
//            categoryWidget = qobject_cast<CategoryWidget*>(widget->parentWidget());
        if(categoryWidget){
            QScrollArea * packageList= categoryWidget->getPackageList();
            refreshContentLayouts(packageList);
            reloadTopBar();
            return;
        }
        if(packageWidget){
            if(PackageManagerTracker::Instance()->packageInProgress(packageWidget->getPackage()->name())){
                QScrollArea * terminal = packageWidget->getTerminal();
                refreshContentLayouts(terminal);
            } else {
                QScrollArea * package = new PackageDetails(packageWidget->getPackage(),0,packageWidget->getPackage()->name());
                refreshContentLayouts(package);
            }
        }
    }
}

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

void MainWindow::refreshContentLayouts(QWidget *item) {
    int current = contentLayouts->currentIndex();
    int max = contentLayouts->count()-1;
    while(current < max){
        QWidget *_item = contentLayouts->widget(max);
        qDebug() << " delete index: " << max << ", max: " << contentLayouts->count() << " = " << _item;
        contentLayouts->removeWidget(_item);
        max=contentLayouts->count()-1;
        if(!qobject_cast<InProgressPackageListView*>(_item) &&
           !qobject_cast<InstalledPackageListView*>(_item)&&
           !qobject_cast<UserUpdatablePackageListView*>(_item)&&
           !qobject_cast<SystemUpdatablePackageListView*>(_item)) {
//            delete _item; // TODO Should be check for old view deletion
        }
    }
    contentLayouts->addWidget(item);
    contentLayouts->setCurrentIndex(contentLayouts->count()-1);
    qDebug() << " add    index: " << contentLayouts->currentIndex() << ", max: " << contentLayouts->count() << " = " << + item;
}

void MainWindow::leftPanelItemHandler(QListWidgetItem *item) {
    PxQListWidgetItem *listWidgetItem = (PxQListWidgetItem *) item;
    QWidget *_item = contentList->getItem(listWidgetItem->getId());
    refreshContentLayouts(_item);
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
    connect(contentList, SIGNAL (itemClicked(QListWidgetItem*)), this, SLOT (leftPanelItemHandler(QListWidgetItem*)));

    contentLayouts = new QStackedWidget;
    contentLayouts->showMaximized();
    contentLayouts->addWidget(contentList->getItem(id));
    contentLayouts->setCurrentIndex(0);

    QHBoxLayout *downLayout = new QHBoxLayout;
    downLayout->addWidget(contentList);
    downLayout->addWidget(contentLayouts);

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