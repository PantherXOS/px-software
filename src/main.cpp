//
// Created by Reza Alizadeh Majd on 10/16/19.
//

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QIcon>
#include <zlib.h>
#include "GUI/MainWindow.h"
#include "PKG/PackageManager.h"

#ifdef FORCE_ZLIB_USAGE
void workaroundForZlibConflict(){
    char a[50] = "test";
    char b[50];
    // zlib struct
    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;

    defstream.avail_in = (uInt)strlen(a)+1; // size of input, string + terminator
    defstream.next_in = (Bytef *)a; // input char array
    defstream.avail_out = (uInt)sizeof(b); // size of output
    defstream.next_out = (Bytef *)b; // output char array

    // the actual compression work.
    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);
}
#endif

QString SearchDBPath(const QString &basePath) {
    QDir *dbDir = nullptr;
    QDir checkoutDir(basePath + "/checkouts");
    QDir pullDir(basePath + "/pull");
    if (checkoutDir.exists()) {
        dbDir = &checkoutDir;
    } else if (pullDir.exists()) {
        dbDir = &pullDir;
    }
    if (dbDir != nullptr) {
        for (const auto &entry : dbDir->entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            QDir internalDBDir(dbDir->absoluteFilePath(entry) + "/px/software/database");
            if (internalDBDir.exists()) {
                return internalDBDir.absolutePath();
            }
        }
    }
    return QString();
}

int main(int argc, char *argv[]) {
#ifdef FORCE_ZLIB_USAGE
    workaroundForZlibConflict();
#endif
    QApplication app(argc, argv);
    QApplication::setApplicationName("px-software");
    QApplication::setApplicationVersion("0.0.2");

    QCommandLineParser parser;
    parser.setApplicationDescription("PantherX Software Center");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption dbPathOption(QStringList() << "p" << "database-path",
                                    "custom base path for internal DB.");
    parser.addOption(dbPathOption);
    parser.process(app);

    QString dbPath = parser.value(dbPathOption);
    if (dbPath.isEmpty()) {
        QString dbBasePath = QDir::homePath() + "/.cache/guix/";
        dbPath = SearchDBPath(dbBasePath);
    }
#ifdef DEV_DB
    dbPath = DEV_DB;
#endif
    if (dbPath.isEmpty()) {
        qDebug() << "Invalid Database Path!";
        return -1;
    }
    qDebug() << "Database loaded from: " << dbPath;

    PKG::PackageManager::Init(dbPath, nullptr);
    MainWindow w;
    w.show();
    return app.exec(); // NOLINT(readability-static-accessed-through-instance)
}
