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

#include "CategoryLayout.h"
#include "PxQListWidget.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private slots:
    void homeButtonHandler();
    void backButtonHandler();
    void forwardButtonHandler();
    void leftPanelItemHandler(QListWidgetItem *item);

private:
    void reloadLayout(string section);
    void loadWindow(string section);
    QHBoxLayout * loadRightTopMenu();
    QHBoxLayout * loadLeftTopMenu();
    QWidget * loadContent(string section);
    QListWidget *loadLeftPanel();
    QStringList getListStore();
//    QStackedWidget *window;
    QWidget *window;
    QStackedLayout *contentLayouts;
    QVBoxLayout *rightSideLayout;
    QVBoxLayout *leftSideLayout;
    QHBoxLayout *rightTopMenu;
    QHBoxLayout *mainLayout;
};

#endif //PX_SOFTWARE_MAINWINDOW_H
