//
// Created by hamzeh on 1/9/20.
//

#ifndef PX_SOFTWARE_PXICONPACK_H
#define PX_SOFTWARE_PXICONPACK_H

#include <string>
#include <iostream>

#include <QIcon>

using namespace std;
class PxIconPack {
public:
    static PxIconPack &instance(){
        static PxIconPack _instance;
        return _instance;
    }

    QIcon getIcon(QString name){
        QIcon icon(path.absolutePath() + QString("/") + name);
        return icon;
    }

    QString iconFile(QString icon){
        return path.absolutePath() + QString("/") + icon;
    }

private:
    PxIconPack(){
        if(const char* env_p = std::getenv("XCURSOR_PATH")){
            QString iconPath = env_p;
            auto list = iconPath.split(":");
            for(auto const &l : list){
                QDir dir(l + "/px-icon-pack/icons/black");
                if(dir.exists()){
                    path = dir;
                    qDebug() << path.absolutePath();
                }
            }
        }
    }

    QDir path;
};
#endif //PX_SOFTWARE_PXICONPACK_H
