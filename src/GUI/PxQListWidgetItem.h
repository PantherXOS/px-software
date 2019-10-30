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
    PxQListWidgetItem(int id, QString title, QFont font){
        this->id = id;
        setText(title);
        setFont(font);
    }

    PxQListWidgetItem(int id, QString title, QFont font, QIcon icon){
        this->id = id;
        setText(title);
        setFont(font);
        setIcon(icon);
    }

    int getId() { return this->id; }

private:
    int id;
};

#endif //PX_SETTINGS_UI_PXQLISTWIDGETITEM_H
