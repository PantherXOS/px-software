//
// Created by Reza Alizadeh Majd on 11/2/19.
//

#ifndef PX_SOFTWARE_GUIXPARSER_H
#define PX_SOFTWARE_GUIXPARSER_H

#include "DataAccessLayer.h"
#include <QVector>
#include <QStringList>

namespace PKG {

    class GuixParser {
    public:
        explicit GuixParser(DataAccessLayer *db);

    public:
        QVector<Package *> parseUpdateList(const QString &inputData);

    private:
        DataAccessLayer *m_db = nullptr;
    };

}


#endif //PX_SOFTWARE_GUIXPARSER_H
