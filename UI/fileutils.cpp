#include "fileutils.h"
#include <dirent.h>
using namespace std;

vector<string> listFiles(const string &path)
{
    vector<string> files;

    DIR *dir = opendir(path.c_str());
    if (dir == nullptr)
        return files; // return empty if path is invalid

    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr)
    {
        string name = entry->d_name;
        if (name == "." || name == "..")
            continue;

        files.push_back(name);
    }

    closedir(dir);
    return files;
}