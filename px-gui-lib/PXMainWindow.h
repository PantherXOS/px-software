//
// Created by hamzeh on 12/31/19.
//

#ifndef PX_SOFTWARE_PXMAINWINDOW_H
#define PX_SOFTWARE_PXMAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QMouseEvent>
#include <QDebug>
#include <QToolBar>

#include "PXSideBar.h"
#include "PXSideBarItem.h"
#include "PXTopBar.h"
#include "PXWindowManager.h"
#include "PXStatusBar.h"

class PXMainWindow : public QMainWindow {
    Q_OBJECT
public:
    PXMainWindow            (const QString& title, const QIcon& icon, QWidget *parent = nullptr);
    ~PXMainWindow           () override;
    void addItemToSideBar   (PXSideBarItem *item);
    void addListToSideBar   (QVector<PXSideBarItem *> list);
    void setDefaultView     (PXSideBarItem *item);
    void loadContent        (PXContentWidget *widget);
    QVector<PXSideBarItem>  sideBarItems();
    PXSearchBox *           searchBox();
    PXStatusBar *           statusBar();

private slots:
    void sideBarItemHandler     (QListWidgetItem*);
    void backButtonPressed      ();
    void forwardButtonPressed   ();
    void settingsButtonPressed  ();
    void helpButtonPressed      ();
    void searchBoxTextEdited    (const QString &text);
    void addWindowSignalHandler (const PXContentWidget *);

private:
    PXSideBar       *sideBar;
    PXTopBar        *topBar;
    PXStatusBar     *pstatusBar;
    QStackedWidget  *contentWidget;
    QWidget         *window;

    void            buildWindow();

    virtual void    searchBoxTextEditedHandler(PXContentWidget *currentWidget, const QString &text) = 0;
    virtual void    settingsButtonHandler() = 0;
    virtual void    helpButtonHandler() = 0;

    PXContentWidget *settingsPage = nullptr;
    PXContentWidget *helpPage = nullptr;
};


#endif //PX_SOFTWARE_PXMAINWINDOW_H
