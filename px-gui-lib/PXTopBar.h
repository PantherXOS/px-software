//
// Created by hamzeh on 12/31/19.
//

#ifndef PX_SOFTWARE_PXTOPBAR_H
#define PX_SOFTWARE_PXTOPBAR_H
#include <QWidget>
#include <QPushButton>
#include <QToolBar>
#include <QToolButton>

#include "PXSearchBox.h"
#include "PXParamSettings.h"

class PXTopBar : public QToolBar{
Q_OBJECT
public:
    PXTopBar(const QString &searchBoxPrefix, QWidget *parent = nullptr) : QToolBar(parent){
        auto settingsButton = new QPushButton(this);
        auto backButton = new QPushButton(this);
        auto forwardButton = new QPushButton(this);
        auto helpButton = new QPushButton(this);
        pSearchBox = new PXSearchBox(searchBoxPrefix, this);

        const QSize buttonSize = QSize(TOP_BAR_BUTTON_SIZE, TOP_BAR_BUTTON_SIZE);
        const QSize iconSize = QSize(TOP_BAR_ICON_SIZE, TOP_BAR_ICON_SIZE);
        settingsButton->setFixedSize(buttonSize);
        backButton->setFixedSize(buttonSize);
        forwardButton->setFixedSize(buttonSize);
        helpButton->setFixedSize(buttonSize);

        settingsButton->setIcon(QIcon::fromTheme("px-settings"));
        settingsButton->setIconSize(iconSize);

        backButton->setIcon(QIcon::fromTheme("go-previous"));
        backButton->setIconSize(iconSize);

        forwardButton->setIcon(QIcon::fromTheme("go-next"));
        forwardButton->setIconSize(iconSize);

        helpButton->setIcon(QIcon::fromTheme("help-about"));
        helpButton->setIconSize(iconSize);

        /// todo completer
        /// Connect the "released" signal of buttons to it's slots (signal handler)
        connect(settingsButton, SIGNAL(released()), this, SLOT(settingsButtonHandler()));
        connect(backButton, SIGNAL (released()), this, SLOT (backButtonHandler()));
        connect(forwardButton, SIGNAL (released()), this, SLOT (forwardButtonHandler()));
        connect(helpButton, SIGNAL (released()), this, SLOT (helpButtonHandler()));
        connect(pSearchBox, SIGNAL(newUserInputReceived(const QString&)), this, SLOT(searchBoxHandler(const QString &)));
        /// Create layout + add buttons
        addWidget(settingsButton);
        addWidget(backButton);
        addWidget(forwardButton);
        addWidget(pSearchBox);
        addWidget(helpButton);
    }

    void setAddress(const QString & address){
        pSearchBox->setAddress(address);
    }

private slots:
    void settingsButtonHandler(){
        emit settingsButtonClicked();
    }

    void backButtonHandler(){
        emit backButtonClicked();
    }

    void forwardButtonHandler(){
        emit forwardButtonClicked();
    }

    void helpButtonHandler(){
        emit helpButtonClicked();
    }

    void searchBoxHandler(const QString &text){
        emit searchKeywordEntered(text);
    }

signals:
    void backButtonClicked();
    void forwardButtonClicked();
    void settingsButtonClicked();
    void helpButtonClicked();
    void searchKeywordEntered(const QString& text);

private:
    PXSearchBox *pSearchBox;
};


#endif //PX_SOFTWARE_PXTOPBAR_H
