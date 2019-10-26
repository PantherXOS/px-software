//
// Created by Reza Alizadeh Majd on 10/22/19.
//

#ifndef PX_SOFTWARE_RECDB_H
#define PX_SOFTWARE_RECDB_H

#include <QObject>
#include <QVector>
#include <QMap>
#include <QFile>
#include <QDir>
#include <QFileInfo>

extern "C" {
#include <rec.h>
};

typedef QMap<QString, QStringList> RecRecord;
typedef QVector<RecRecord> RecRecordList;

class RecDB {
public:
    explicit RecDB(const QString &path);
    virtual ~RecDB();

protected:
    static rec_db_t InitDB(const QString &path);
    static bool LoadDBFile(rec_db_t db, const QString &path);
    static RecRecord ParseRecord(rec_record_t record);

public:
    RecRecordList find(const QString &query = QString());


private:
    QString m_dbPath;
    rec_db_t m_db = nullptr;
    static int _instanceCount;
};

#endif //PX_SOFTWARE_RECDB_H
