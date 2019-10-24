//
// Created by Reza Alizadeh Majd on 10/22/19.
//

#include "RecDB.h"
#include <QDebug>

int RecDB::_instanceCount = 0;

RecDB::RecDB(const QString &path) {
    RecDB::_instanceCount++;
    if (RecDB::_instanceCount == 1) {
        rec_init();
    }
    m_dbPath = path;
    m_db = RecDB::InitDB(m_dbPath);
    if (m_db == nullptr) {
        qDebug() << "unable to init db: " << path;
    }
}

RecDB::~RecDB() {
    if (m_db != nullptr) {
        rec_db_destroy(m_db);
    }
    RecDB::_instanceCount--;
    if (RecDB::_instanceCount == 0) {
        rec_fini();
    }
}

rec_db_t RecDB::InitDB(const QString &path) {
    rec_db_t db = rec_db_new();
    if (!db) {
        return nullptr;
    }
    QFileInfo fInfo(path);
    QStringList dbFilePaths;
    if (fInfo.exists() && fInfo.isDir()) {
        QDir dbDir(path);
        auto dbFiles = dbDir.entryList(QStringList() << "*.rec", QDir::Files);
        for (auto &dbFile : dbFiles) {
            dbFilePaths << dbDir.absoluteFilePath(dbFile);
        }
    } else if (fInfo.exists() && fInfo.isFile()) {
        dbFilePaths << fInfo.absoluteFilePath();
    }
    bool isDBLoaded = !dbFilePaths.empty();
    for (const auto &dbFile : dbFilePaths) {
        isDBLoaded = isDBLoaded && RecDB::LoadDBFile(db, dbFile);
    }
    if (!isDBLoaded) {
        rec_db_destroy(db);
        db = nullptr;
    }
    return db;
}

bool RecDB::LoadDBFile(rec_db_t db, const QString &path) {
    const char *fileName = path.toStdString().c_str();
    FILE *in;
    if (!(in = fopen(fileName, "r"))) {
        qDebug() << "unable to open db file: " << path;
        return false;
    }

    bool result = true;
    rec_rset_t recSet;
    rec_parser_t parser = rec_parser_new(in, fileName);
    while (result && rec_parse_rset(parser, &recSet)) {
        char *recSetType = rec_rset_type(recSet);
        if (rec_db_type_p(db, recSetType)) {                        // record set already existed in DB.
            rec_rset_t existingRecSet = rec_db_get_rset_by_type(db, recSetType);
            rec_mset_iterator_t iter = rec_mset_iterator(rec_rset_mset(recSet));
            rec_mset_elem_t elem;
            while (rec_mset_iterator_next(&iter, MSET_ANY, nullptr, &elem)) {
                void *data = rec_mset_elem_dup_data(elem);
                if (!data || !rec_mset_append(rec_rset_mset(existingRecSet),
                                              rec_mset_elem_type(elem),
                                              data,
                                              rec_mset_elem_type(elem))) {
                    qDebug() << "error on append record set";
                    result = false;
                    break;
                }
            }
            rec_mset_iterator_free(&iter);
        } else {
            if (!rec_db_insert_rset(db, recSet, rec_db_size(db))) {
                qDebug() << "rec db insert error";
                result = false;
                break;
            }
        }
    }
    if (rec_parser_error(parser)) {
        qDebug() << "rec parser error: ";
        rec_parser_perror(parser, "%s", fileName);
        result = false;
    }
    rec_parser_destroy(parser);
    return result;
}

RecRecordList RecDB::find(const QString &query) {
    RecRecordList result;
    if (m_db == nullptr) {
        qWarning() << "db is not initialized";
        return result;
    }
    int flags = REC_F_ICASE | REC_F_DESCRIPTOR | REC_F_UNIQ; // NOLINT(hicpp-signed-bitwise)
    rec_sex_t sExpr = nullptr;
    if (!query.isEmpty()) {
        sExpr = rec_sex_new(true);
        if (!rec_sex_compile(sExpr, query.toStdString().c_str())) {
            qWarning() << "invalid s-expression: " << query;
            sExpr = nullptr;
        }
    }
    rec_rset_t recordSet = rec_db_query(m_db,
                                        nullptr, //type
                                        nullptr, //join
                                        nullptr, //index
                                        sExpr, //sex
                                        nullptr, //fast_string
                                        0,       //random
                                        nullptr, //FEX
                                        nullptr, //password
                                        nullptr, //group by
                                        nullptr, //sort
                                        flags       //flags
    );
    rec_sex_destroy(sExpr);

    void *data;
    rec_mset_elem_t elem;
    rec_mset_iterator_t iter = rec_mset_iterator(rec_rset_mset(recordSet));
    while (rec_mset_iterator_next(&iter, MSET_ANY, (const void **) &data, &elem)) {
        if (rec_mset_elem_type(elem) == MSET_RECORD) {
            RecRecord curRec = RecDB::ParseRecord((rec_record_t) data);
            result.append(curRec);
        }
    }
    rec_mset_iterator_free(&iter);
    rec_rset_destroy(recordSet);
    return result;
}

RecRecord RecDB::ParseRecord(rec_record_t record) {
    RecRecord result;
    rec_mset_iterator_t iter;
    rec_mset_elem_t elem;
    char *data;
    iter = rec_mset_iterator(rec_record_mset(record));
    while (rec_mset_iterator_next(&iter, MSET_ANY, (const void **) &data, &elem)) {
        if (rec_mset_elem_type(elem) == MSET_FIELD) {
            auto field = (rec_field_t) data;
            const char *fieldName = rec_field_name(field);
            const char *fieldValue = rec_field_value(field);
            if (strlen(fieldName) > 0 && strlen(fieldValue) > 0) {
                result[fieldName].append(fieldValue);
            }
        }
    }
    rec_mset_iterator_free(&iter);
    return result;
}
