//
// Created by Reza Alizadeh Majd on 10/16/19.
//

#include <QApplication>
#include "GUI/MainWindow.h"
#include <QDebug>
#include <QIcon>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec(); // NOLINT(readability-static-accessed-through-instance)
}
