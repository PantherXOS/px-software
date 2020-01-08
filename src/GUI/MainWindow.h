//
// Created by Reza Alizadeh Majd on 10/16/19.
//

#ifndef PX_SOFTWARE_MAINWINDOW_H
#define PX_SOFTWARE_MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QGridLayout>
#include <QScrollArea>
#include <QListWidget>
#include <QStringList>
#include <QStackedLayout>
#include <QLineEdit>
#include <QTimerEvent>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QToolBar>

#include "PxQListWidgetItem.h"
#include "ContentList.h"
#include "PackageManager.h"
#include "PackageListWidgetItem.h"
#include "PackageDetails.h"
#include "SearchPackagesList.h"
#include "PxSearchBar.h"
#include "PackageManagerTracker.h"
#include "Settings.h"
#include "ScreenshotItem.h"
#include "ScreenShotViewer.h"
#include "UserUpdateNotification.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QString dbPath, QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void mousePressEvent(QMouseEvent *event) override;
    void settingsButtonHandler();
    void backButtonHandler();
    void forwardButtonHandler();
    void helpButtonHandler();
    void leftPanelItemHandler(QListWidgetItem *item);
    void searchBoxHandler(const QString &);
    void screenshotItemClickedHandler(ScreenshotItem *item);

private:
    void          reloadTopBar();
    void          loadWindow(int id);
    QToolBar * loadTopMenu();
    void refreshContentLayouts(QWidget *item);

    QWidget *window;
    QStackedWidget *contentLayouts;

    ContentList *contentList;
    QString viewName;
    QString packageName;
    QPushButton *settingsButton;
    QPushButton *backButton;
    QPushButton *forwardButton;
    PxSearchBar *addressBar;
    QPushButton *helpButton;
};

#endif //PX_SOFTWARE_MAINWINDOW_H
