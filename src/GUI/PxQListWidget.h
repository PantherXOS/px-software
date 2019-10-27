//
// Created by hamzeh on 10/25/19.
//

#ifndef PX_SETTINGS_UI_PXQLISTWIDGETITEM_H
#define PX_SETTINGS_UI_PXQLISTWIDGETITEM_H

#include <string>
#include <QObject>
#include <QWidget>
#include <QListWidgetItem>
#include <QFont>
#include <QIcon>

using namespace std;
class PxQListWidgetItem : public QListWidgetItem
{
public:
    PxQListWidgetItem(QString title, QFont font){
        this->title = title;
        setText(title);
        setFont(font);
    }

    PxQListWidgetItem(QString title, QFont font, QIcon icon){
        this->title = title;
        setText(title);
        setFont(font);
        setIcon(icon);
    }

    QString getTitle() { return this->title; }

private:
    QString title;
};

#endif //PX_SETTINGS_UI_PXQLISTWIDGETITEM_H
