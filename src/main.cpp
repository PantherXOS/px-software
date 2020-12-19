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

#include <QApplication>
#include <QCommandLineParser>
#include <QTranslator>
#include <QDebug>
#include <QIcon>
#include <zlib.h>
#include <QStandardPaths>
#include <filesystem>

#include "GUI/MainWindow.h"
#include "PKG/PackageManager.h"

#define  LOG_FILE_PATH  (QDir::homePath() + QString("/.var/log/px/"))
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

void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QString txt;
    switch (type) {
        case QtDebugMsg:
            txt = QString("%1 \t dbg : %2").arg(QDateTime::currentDateTime().toString()).arg(msg);
            break;
        case QtInfoMsg:
            txt = QString("%1 \t inf : %2").arg(QDateTime::currentDateTime().toString()).arg(msg);
            break;
        case QtWarningMsg:
            txt = QString("%1 \t war : %2").arg(QDateTime::currentDateTime().toString()).arg(msg);
            break;
        case QtCriticalMsg:
            txt = QString("%1 \t crt : %2").arg(QDateTime::currentDateTime().toString()).arg(msg);
            break;
        case QtFatalMsg:
            txt = QString("%1 \t fat : %2").arg(QDateTime::currentDateTime().toString()).arg(msg);
            break;
    }
    QFile outFile(LOG_FILE_PATH+"software");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

int main(int argc, char *argv[]) {
#ifdef FORCE_ZLIB_USAGE
    workaroundForZlibConflict();
#endif
    QApplication app(argc, argv);
    QApplication::setApplicationName("px-software");
    QApplication::setApplicationVersion("0.1.1");
    // get locale and set language
    QString defaultLocale = QLocale::system().name(); // e.g. "de_DE"
    defaultLocale.truncate(defaultLocale.lastIndexOf('_'));
    QString m_langPath;
#ifdef DEV_TR
    m_langPath = QApplication::applicationDirPath() + "/translations/px-software_"+defaultLocale;
    qDebug() << "Translation file path: " << m_langPath;
#else
    m_langPath = QApplication::applicationDirPath() + "/../share/px-software/translations/px-software_"+defaultLocale;
#endif
    QTranslator translator;
    translator.load(m_langPath);
    app.installTranslator(&translator);

    QCommandLineParser parser;
    parser.setApplicationDescription("PantherX Software Center");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption dbPathOption(QStringList() << "p" << "database-path",
                                    "custom base path for internal DB.","DB");
    parser.addOption(dbPathOption);
    QCommandLineOption targetLogOption(QStringList() << "t" << "target-log",
                                    "changing the output log to console or file","target");
    parser.addOption(targetLogOption);
    parser.process(app);
    
    QString outputlog = parser.value(targetLogOption);
    if(outputlog!="console"){
        std::filesystem::create_directories(LOG_FILE_PATH.toStdString());
        qInstallMessageHandler(messageOutput);
    }
    QString dbPath = parser.value(dbPathOption);
    if (dbPath.isEmpty()) {
        QString dbBasePath = QDir::homePath() + "/.cache/guix/";
        dbPath = SearchDBPath(dbBasePath);
    }
#ifdef DEV_DB
    dbPath = DEV_DB;
#endif
    MainWindow w(dbPath);
    w.showMaximized();
    return app.exec(); // NOLINT(readability-static-accessed-through-instance)
}
