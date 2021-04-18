/*
 * px-software - Qt-GUI package manager for guix on PantherX OS
 * Copyright (C) 2019, 2020  Hamzeh Nasajpour <h.nasajpour@pantherx.org>
 * Copyright (C) 2019, 2020  Reza Alizadeh Majd <r.majd@pantherx.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

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
    explicit RecDB();
    virtual ~RecDB();

protected:
    static rec_db_t InitDB(const QString &path);
    static bool LoadDBFile(rec_db_t db, const QString &path);
    static bool LoadText(rec_db_t db, const QString &text);
    static RecRecord ParseRecord(rec_record_t record);

public:
    RecRecordList find(const QString &query = QString());
    RecRecordList findFromText(const QString &text = QString(), const QString &query = QString());
    bool isInitted();

private:
    QString m_dbPath;
    rec_db_t m_db = nullptr;
    static int _instanceCount;
};

#endif //PX_SOFTWARE_RECDB_H
