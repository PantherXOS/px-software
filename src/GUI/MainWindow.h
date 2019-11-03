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

#include "PxQListWidgetItem.h"
#include "ContentList.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void settingsButtonHandler();
    void backButtonHandler();
    void forwardButtonHandler();
    void helpButtonHandler();
    void leftPanelItemHandler(QListWidgetItem *item);
    void searchBoxHandler();

private:
    void          reloadTopBar();
    void          loadWindow(int id);
    QHBoxLayout * loadTopMenu();

    QWidget *window;
    QStackedLayout *contentLayouts;

    ContentList *contentList;

    QPushButton *settingsButton;
    QPushButton *backButton;
    QPushButton *forwardButton;
    QLineEdit *addressBar;
    QPushButton *helpButton;
};

#endif //PX_SOFTWARE_MAINWINDOW_H
