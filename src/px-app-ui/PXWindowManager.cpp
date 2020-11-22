//
// Created by hamzeh on 1/14/20.
//

#include "PXWindowManager.h"

PXWindowManager *PXWindowManager::_instance = nullptr;

PXWindowManager *PXWindowManager::instance(){
    if(_instance == nullptr)
        _instance = new PXWindowManager();
    return _instance;
}


PXWindowManager::PXWindowManager(QObject *parent) : QObject(parent) {

}

void PXWindowManager::add(const PXContentWidget *widget) {
    emit addPxWindow(widget);
}
