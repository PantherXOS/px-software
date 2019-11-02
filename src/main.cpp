//
// Created by Reza Alizadeh Majd on 10/16/19.
//

#include <QApplication>
#include "GUI/MainWindow.h"
#include <QDebug>
#include <QIcon>
#include <zlib.h>

#ifdef FORCE_ZLIB_USAGE
void workaroundForZlibConflict(){
    char a[50] = "test";
    char b[50];
    char c[50];
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
int main(int argc, char *argv[]) {
#ifdef FORCE_ZLIB_USAGE
    workaroundForZlibConflict();
#endif
    QApplication app(argc, argv);
    MainWindow w;
    w.show();
    return app.exec(); // NOLINT(readability-static-accessed-through-instance)
}
