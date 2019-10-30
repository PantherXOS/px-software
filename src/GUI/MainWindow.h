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
    void          reloadContent(QWidget *section);
    void          loadWindow(int id);
    QHBoxLayout * loadTopMenu();
    QListWidget * loadLeftPanel();

    QWidget *window;
    QStackedLayout *contentLayouts;
    QPushButton *settingsButton;
    QPushButton *backButton;
    QPushButton *forwardButton;
    ContentList *contentList;
    QPushButton *helpButton;
    QLineEdit *addressBar;
};

#endif //PX_SOFTWARE_MAINWINDOW_H
