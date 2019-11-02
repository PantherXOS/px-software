//
// Created by Reza Alizadeh Majd on 11/2/19.
//

#include "GuixParser.h"

namespace PKG {

    GuixParser::GuixParser(DataAccessLayer *db) : m_db(db) {
    }

    QVector<Package *> GuixParser::parseUpdateList(const QString &inputData) {
        QStringList guixPackages;
        for (const auto &line : inputData.split('\n')) {
            if (line.startsWith("   ")) {
                auto parts = line.trimmed().split('\t');
                if (parts.size() == 3) {
                    guixPackages << parts[0];
                }
            }
        }
        auto packageList = m_db->packageList(guixPackages);
        for (auto *pkg : packageList) {
            pkg->setInstalled(true);
            pkg->setUpdateAvailable(true);
        }
        return packageList;
    }
}