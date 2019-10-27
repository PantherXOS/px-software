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
    PxQListWidgetItem(QString title, QFont font, QWidget *content){
        this->title = title;
        this->content = content;
        setText(title);
        setFont(font);
    }

    PxQListWidgetItem(QString title, QFont font, QWidget *content, QIcon icon){
        this->title = title;
        this->content = content;
        setText(title);
        setFont(font);
        setIcon(icon);
    }

    QString getTitle() { return this->title; }
    QWidget *getContent() {return this->content; }
private:
    QString title;
    QWidget *content;
};

#endif //PX_SETTINGS_UI_PXQLISTWIDGETITEM_H
