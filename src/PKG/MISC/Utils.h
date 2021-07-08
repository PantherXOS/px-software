// Copyright © 2020 Hamzeh Nasajpour <h.nasajpour@pantherx.org> | PantherX.DEV

#ifndef PX_SETTINGS_SERVICE_UTILS_H
#define PX_SETTINGS_SERVICE_UTILS_H
using namespace std;

#include <vector>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <string>
#include <array>
#include <bits/unique_ptr.h>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
#include <sys/types.h>
#include <grp.h>
#include <wordexp.h>
#include <dirent.h>
#include <stdarg.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <unistd.h>
#include <limits.h>
#include <pwd.h>
#include <QStandardPaths>
#include <QFile>
#include <QDir>

namespace PXUTILS {

    namespace DATETIME {
        int    ConvertToTimeStamp(string datetime);
    }

    namespace PXFILE {

        string abspath(const string &path);

        vector<string> dirfiles(const string &path, string ext);

        vector<string> findfiles(const string &path, string prefix);

        string extpart(const string &fname);

        bool exists(const string &path);

        bool remove(const string &path);

        bool write(const string &path, const string &data);

        string read(const string &path);
    }

    namespace COMMAND {
        int ExecuteWithExitCode(const char* cmd);
        string Execute(const char* cmd, bool &result);
    }


    namespace PATH
    {
	    string getHomePath();
        string getExecPath();
        string getUserName();
    }


    namespace STRING
    {
        vector<string> split(string _str, string delimiter);
    }

    namespace PROCCESS
    {
        int GetPIDbyName(string pidName);
    }

    namespace LXQT 
    {
        int refreshDesktopApplications();
    }
}
#endif //PX_SETTINGS_SERVICE_UTILS_H
