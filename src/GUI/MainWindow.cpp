//
// Created by Reza Alizadeh Majd on 10/16/19.
//

#include "MainWindow.h"

MainWindow::MainWindow(QString dbPath, QWidget *parent) :
        QMainWindow(parent){
    PackageManagerTracker::init(dbPath);
    showMaximized();
    setWindowIcon(QIcon(":images/general/src/GUI/resources/panther"));
    setWindowTitle("Software");
    UserUpdateNotification::instance();

    loadWindow(CONTENT_SECTIONS::STORE_LATEST);
}

MainWindow::~MainWindow() {
}
// --------------------------------------------------------------------------- signal-slot handlers
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QWidget * const widget = childAt(event->pos());

    auto categoryWidget = qobject_cast<CategoryWidget*>(widget->parentWidget());
    auto packageWidget = qobject_cast<PackageListWidgetItem*>(widget->parentWidget());

    if(widget){
//        if(!categoryWidget)
//            categoryWidget = qobject_cast<CategoryWidget*>(widget->parentWidget());
        if(categoryWidget){
            PackageListWidget *packageListWidget = new PackageListWidget(false, categoryWidget->getCategory()->name(),
                                                                         nullptr);
            refreshContentLayouts(packageListWidget);
        } else if(packageWidget){
            auto inProgressParent = qobject_cast<InProgressPackageListView*>(contentLayouts->currentWidget());
            if(inProgressParent && PackageManagerTracker::Instance()->packageInProgress(packageWidget->getPackage()->name())) {
                QScrollArea * terminal = packageWidget->getTerminal();
                refreshContentLayouts(terminal);
            } else {
                QScrollArea * package = new PackageDetails(packageWidget->getPackage(),
                                                           packageWidget->getPackage()->name(), nullptr);
                connect(package, SIGNAL(screenshotItemClicked(ScreenshotItem *)), this, SLOT(screenshotItemClickedHandler(ScreenshotItem *)));
                refreshContentLayouts(package);
            }
        }
    }
}

void MainWindow::screenshotItemClickedHandler(ScreenshotItem *item) {
    ScreenShotViewer *screenShotViewer = new ScreenShotViewer(item);
    refreshContentLayouts(screenShotViewer);
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
    if (item) {
        int current = contentLayouts->currentIndex();
        int max = contentLayouts->count() - 1;
        while (current < max) {
            QWidget *_item = contentLayouts->widget(max);
//        qDebug() << " delete index: " << max << ", max: " << contentLayouts->count() << " = " << _item;
            contentLayouts->removeWidget(_item);
            max = contentLayouts->count() - 1;
            if (!qobject_cast<InProgressPackageListView *>(_item) &&
                !qobject_cast<InstalledPackageListView *>(_item) &&
                !qobject_cast<TerminalWidget *>(_item) &&
                !qobject_cast<UserUpdatablePackageListView *>(_item) &&
                !qobject_cast<SystemUpdatablePackageListView *>(_item)) {
                delete _item; // TODO Should be check for old view deletion
            }
        }
//        item->setStyleSheet(CONTENT_WIDGET_STYLE);
        contentLayouts->addWidget(item);
        contentLayouts->setCurrentIndex(contentLayouts->count() - 1);
        reloadTopBar();
//    qDebug() << " add    index: " << contentLayouts->currentIndex() << ", max: " << contentLayouts->count() << " = " << + item;
    }
}

void MainWindow::leftPanelItemHandler(QListWidgetItem *item) {
    auto listWidgetItem = (PxQListWidgetItem *) item;
    if(listWidgetItem){
        auto view = listWidgetItem->getView();
        refreshContentLayouts(view);
    }
}

void MainWindow::searchBoxHandler(const QString &text){
    auto currentWidget = contentLayouts->currentWidget();

    SearchPackagesList::SearchFilter filter;
    if(qobject_cast<InstalledPackageListView*>(currentWidget)){
        filter = SearchPackagesList::SearchFilter::Installed;
    } else if (qobject_cast<SearchPackagesList*>(currentWidget)) {
        filter = ((SearchPackagesList *)currentWidget)->currentFilter();
    } else {
        filter = SearchPackagesList::SearchFilter::All;
    }

    auto searchPackageList = new SearchPackagesList(text, filter , nullptr);
    refreshContentLayouts(searchPackageList);
}

// -------------------------------------------------------------------------------- ui form objects
QHBoxLayout *MainWindow::loadTopMenu() {
    settingsButton = new QPushButton(this);
    backButton = new QPushButton(this);
    forwardButton = new QPushButton(this);
    helpButton = new QPushButton(this);
    addressBar =new PxSearchBar(this);

    const QSize buttonSize = QSize(TOP_MENU_BUTTON_SIZE, TOP_MENU_BUTTON_SIZE);
    const QSize iconSize = QSize(TOP_MENU_ICON_SIZE, TOP_MENU_ICON_SIZE);
    settingsButton->setFixedSize(buttonSize);
    backButton->setFixedSize(buttonSize);
    forwardButton->setFixedSize(buttonSize);
    helpButton->setFixedSize(buttonSize);
    settingsButton->setIcon(QIcon::fromTheme("preferences-system"));
    settingsButton->setIconSize(iconSize);

    backButton->setIcon(QIcon::fromTheme("go-previous"));
    backButton->setIconSize(iconSize);

    forwardButton->setIcon(QIcon::fromTheme("go-next"));
    forwardButton->setIconSize(iconSize);

    helpButton->setIcon(QIcon::fromTheme("help-about"));
    helpButton->setIconSize(iconSize);
    addressBar->setAddress("Software/", "");

    QHBoxLayout *addressBarLayout = new QHBoxLayout;
    addressBarLayout->addWidget(addressBar);
    /// todo completer
    /// Connect the "released" signal of buttons to it's slots (signal handler)
    connect(settingsButton, SIGNAL(released()), this, SLOT(settingsButtonHandler()));
    connect(backButton, SIGNAL (released()), this, SLOT (backButtonHandler()));
    connect(forwardButton, SIGNAL (released()), this, SLOT (forwardButtonHandler()));
    connect(helpButton, SIGNAL (released()), this, SLOT (helpButtonHandler()));
    connect(addressBar, SIGNAL(newUserInputReceived(const QString&)), this, SLOT(searchBoxHandler(const QString &)));

    /// Create layout + add buttons
    QHBoxLayout *topMenuLayout = new QHBoxLayout();
    topMenuLayout->addWidget(settingsButton);
    topMenuLayout->addWidget(backButton);
    topMenuLayout->addWidget(forwardButton);
    topMenuLayout->addLayout(addressBarLayout);
    topMenuLayout->addWidget(helpButton);
    topMenuLayout->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    return topMenuLayout;
}
// ------------------------------------------------------------------------------ reload ui objects
void MainWindow::loadWindow(int id) {
    contentList = new ContentList();
    connect(contentList, SIGNAL (itemClicked(QListWidgetItem*)), this, SLOT (leftPanelItemHandler(QListWidgetItem*)));

    contentLayouts = new QStackedWidget;
    contentLayouts->showMaximized();

    QHBoxLayout *downLayout = new QHBoxLayout;
    downLayout->addWidget(contentList);
    downLayout->addWidget(contentLayouts);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(loadTopMenu());
    mainLayout->addLayout(downLayout);

    window = new QWidget;
    window->setLayout(mainLayout);
    setCentralWidget(window);

    refreshContentLayouts(contentList->getItem(id));
}

void MainWindow::reloadTopBar(){
    auto categoryWidget = qobject_cast<CategoryWidget*>(contentLayouts->currentWidget());
    auto packageWidget = qobject_cast<PackageListWidgetItem*>(contentLayouts->currentWidget());
    auto packageDetailsWidget = qobject_cast<PackageDetails*>(contentLayouts->currentWidget());
    auto searchPackageWidget = qobject_cast<SearchPackagesList*>(contentLayouts->currentWidget());
    auto screenshotWidget = qobject_cast<ScreenShotViewer*>(contentLayouts->currentWidget());

    if(categoryWidget){
        packageName = "";
        viewName=((CategoryWidget*)categoryWidget)->getCategory()->name();
    }
    else if(packageWidget) {
        packageName = ((PackageListWidgetItem *) packageWidget)->getPackage()->title();
    }
    else if(packageDetailsWidget) {
        packageName = ((PackageDetails *) packageDetailsWidget)->getTitle();
    } else if(searchPackageWidget) {
        packageName = ((SearchPackagesList *) searchPackageWidget)->getTitle();
    } else if(screenshotWidget){
        packageName = ((ScreenShotViewer *) screenshotWidget)->getTitle();
    }
    else {
        packageName = "";
        viewName = ((PxQScrollArea *)(contentLayouts->currentWidget()))->getTitle();
    }
    contentList->setSelectedItem(viewName);
    addressBar->setAddress(QString("Software/") + viewName + QString("/") , packageName);
    if(contentLayouts->count()==1) {
        backButton->setDisabled(true);
        forwardButton->setDisabled(true);
    } else {
        backButton->setDisabled(false);
        forwardButton->setDisabled(false);
    }
}

