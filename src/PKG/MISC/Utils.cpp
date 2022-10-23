// Copyright © 2020 Hamzeh Nasajpour <h.nasajpour@pantherx.org> | PantherX.DEV

#include "Utils.h"
#include <QDebug>
#include "../PKG/MISC/LxqtSessionInterface.h"

int    PXUTILS::DATETIME::ConvertToTimeStamp(string datetime){
    struct tm tm;
    time_t unixtime;
    strptime(datetime.c_str(),"%a, %e %h %Y %H:%M:%S %z",&tm);
    unixtime = mktime(&tm);
    return unixtime;
}


string PXUTILS::PXFILE::abspath(const string &path) {
    string res;
    wordexp_t w;
    if (wordexp(path.c_str(), &w, 0) == 0) {
        res = string(*w.we_wordv);
        wordfree(&w);
    }
    return res;
}

vector<string> PXUTILS::PXFILE::dirfiles(const string &path, string ext) {
    vector<string> result;
    if (!ext.empty() && ext[0] == '.') {
        ext = ext.substr(1);
    }

    DIR* dirp = opendir(path.c_str());
    if (dirp != nullptr) {
        struct dirent * dp;
        while ((dp = readdir(dirp)) != nullptr) {
            if (strcmp(dp->d_name, ".") == 0)
                continue;
            if (strcmp(dp->d_name, "..") == 0)
                continue;

            if (ext.empty() || PXUTILS::PXFILE::extpart(dp->d_name) == ext) {
                result.emplace_back(dp->d_name);
            }
        }
        closedir(dirp);
    }
    return result;
}

vector<string> PXUTILS::PXFILE::findfiles(const string &path, string filter){
    auto files = dirfiles(path,"");
    vector<string> result;
    for(auto f: files){
        if(f.find(filter) != std::string::npos ) result.emplace_back(f);
    }
    return result;
}


string PXUTILS::PXFILE::extpart(const string &fname) {
    const char *fpointer = fname.c_str();
    const char *d = strrchr(fpointer, '.');
    if (!d || d == fpointer) {
        return  string();
    }
    return string(d+1);
}

bool PXUTILS::PXFILE::exists(const string &fpath) {
    struct stat buff{};
    return (stat(fpath.c_str(), &buff) == 0);
}

bool PXUTILS::PXFILE::remove(const string &path) {
    return (::remove(path.c_str()) == 0);
}

bool PXUTILS::PXFILE::write(const string &path, const string &data) {
    ofstream of(path.c_str(), ios::out);
    of << data;
    of.close();
    return true;
}

string PXUTILS::PXFILE::read(const string &path){
    std::ifstream hostFile(path);
    std::string content((std::istreambuf_iterator<char>(hostFile)),
                        (std::istreambuf_iterator<char>()));
    return content;
}

vector<string> PXUTILS::STRING::split(string _str, string delimiter){
    vector<string> array;
    std::stringstream  data(_str);
    std::string line;
    char del[2];
    strcpy(del, delimiter.c_str());
    while(std::getline(data,line,del[0]))
        array.push_back(line);
    return array;
}

int PXUTILS::COMMAND::ExecuteWithExitCode(const char* cmd) {
    int ret = system(cmd);
    if (WEXITSTATUS(ret) == 0x10)
        return 0;
    else
        return 1;
}

string PXUTILS::COMMAND::Execute(const char* cmd, bool &result) {
    std::array<char, 10240> buffer;
    std::string resultStr;
    auto pipe = popen(cmd, "r");
    result = true;

    if (!pipe) throw std::runtime_error("popen() failed!");
    
    while (!feof(pipe))
    {
        if (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
            resultStr += buffer.data();
    }
    
    auto rc = pclose(pipe);
    
    if (rc != EXIT_SUCCESS)
    {
        qWarning() << "Running failed:" << cmd;
        result = false;
    }
    return resultStr;
}


string PXUTILS::PATH::getHomePath(){
    return string(getpwuid(getuid())->pw_dir);
}

string PXUTILS::PATH::getExecPath(){
    char *arg1;
    arg1=(char *)malloc(1000*sizeof(char));
    char exepath[PATH_MAX + 1] = {0};

    sprintf( arg1, "/proc/%d/exe", getpid() );
    readlink( arg1, exepath, 1024 );
    string ret = string( exepath );
    std::size_t found = ret.find_last_of("//");
    ret=ret.substr(0,found+1);
    free(arg1);
    return ret;
}

string PXUTILS::PATH::getUserName()
{
  uid_t uid = geteuid();
  struct passwd *pw = getpwuid(uid);
  if (pw)
  {
    return string(pw->pw_name);
  }

  return "";
}

int PXUTILS::PROCCESS::GetPIDbyName(string pidName)
{
    FILE *fp;
    int pidValue=-1;
    string pidofCmd = "pidof " + pidName + "> /tmp/pidof";
    system("> /tmp/pidof"); 
    system(pidofCmd.c_str()); 
    fp = fopen("/tmp/pidof", "r");
    fscanf(fp, "%d", &pidValue);
    fclose(fp);
    return pidValue;
}

int PXUTILS::LXQT::refreshDesktopApplications(){
    // The desktop files will be installed in STORE/share/applications
    // since the STORE is readonly, all file timestamps is set to `1970/01/01`
    // so the lxqt-panel can't find out new desktop files installed in the paths
    // Our Solution: Update the access/modification time of `temp-software.desktop` file 
    // in the `~/.local/share/application/` after each install/remove
    QString path = QStandardPaths::standardLocations(QStandardPaths::ApplicationsLocation)[0];
    QDir dir(path);
    if(!dir.exists()){
        dir.mkpath(path);
        auto sessionInterface = new LxqtSessionInterface();
        QObject::connect(sessionInterface, &LxqtSessionInterface::panelRestarted, sessionInterface, &QObject::deleteLater);
        sessionInterface->restartPanel();
    }
    string command = "touch " + path.toStdString() + "/temp-software.desktop";
    return PXUTILS::COMMAND::ExecuteWithExitCode(command.c_str());
}
