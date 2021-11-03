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
#include <QDir>

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
    ts << txt << Qt::endl;
}

QMap<QString, QString> parseUrlScheme(const QString &url) {
    QStringList elements = url.split(':');
    QMap<QString, QString> map;
    if(elements.size() == 2){
        QStringList entries = elements[1].split("?");
        for(const auto &entry : entries) {
            QStringList parts = entry.split('=');
            if(parts.size() == 2) {
                QString result = parts[1];
                if(parts[0] == APPLIST_ARG_TILTE)
                    map[APPLIST_ARG_TILTE] = result;
                else if(parts[0] == APP_ARG_TITLE)
                    map[APP_ARG_TITLE] = result;
            }
        }
    }
    return map;
}

int main(int argc, char *argv[]) {
    QMap<QString, QString> urlArgs;
    if(argc > 1) urlArgs = parseUrlScheme(argv[1]);
#ifdef FORCE_ZLIB_USAGE
    workaroundForZlibConflict();
#endif
    QApplication app(argc, argv);
    QApplication::setApplicationName("px-software");
    QApplication::setApplicationVersion("0.2.9");
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
    QCommandLineOption itemOption(QStringList() << "i" << "item",
                                    "open specific item.","item");
    parser.addOption(itemOption);
    parser.process(app);
    
    QString outputlog = parser.value(targetLogOption);
    if(outputlog!="console"){
        QDir logDir(LOG_FILE_PATH);
        if(!logDir.exists()){
            QDir().mkpath(LOG_FILE_PATH);
        }
        qInstallMessageHandler(messageOutput);
    }

    QString dbPath = parser.value(dbPathOption);
    QString item = parser.value(itemOption);
    if (urlArgs[APPLIST_ARG_TILTE].isEmpty()) {
        urlArgs[APPLIST_ARG_TILTE] = item;
    }

    MainWindow w(urlArgs,dbPath);
    w.showMaximized();
    return app.exec(); // NOLINT(readability-static-accessed-through-instance)
}
