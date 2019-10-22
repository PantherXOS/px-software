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

#include "CategoryLayout.h"

class MainWindow : public QMainWindow {
Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
private slots:
    void homeButtonHandler();
    void backButtonHandler();
    void forwardButtonHandler();

private:
    QHBoxLayout * loadTopMenu();
    QGridLayout * loadCategoryList();

    QStackedWidget *window;
};

#endif //PX_SOFTWARE_MAINWINDOW_H
