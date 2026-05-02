#include <sys/stat.h>
#include <cstdio>
#include <direct.h>
#include <dirent.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <shellapi.h>
#include "fileutils.h"
using namespace std;

string enterFolder(string folderName, string currentPath)
{
    currentPath = currentPath + "\\" + folderName;
    return currentPath;
}
string goBack(string currentPath)
{
    char lastDoubleSlash = '\\';
    size_t position;
    position = currentPath.find_last_of(lastDoubleSlash);
    currentPath = currentPath.substr(0, position);
    return currentPath;
}
bool isFolder(string currentPath, string folderName)
{
    currentPath = currentPath + "\\" + folderName;
    struct stat info;
    stat(currentPath.c_str(), &info);
    return S_ISDIR(info.st_mode);
}
long fileSize(string currentPath, string folderName)
{
    currentPath = currentPath + "\\" + folderName;
    struct stat info;
    stat(currentPath.c_str(), &info);
    return info.st_size;
}
string fileType(string fileName)
{
    string type;
    char extensionDot = '.';
    size_t position;
    position = fileName.find_last_of(extensionDot);
    fileName = fileName.substr(position);
    return fileName;
}
bool createFolder(string currentPath, string folderName)
{
    string newPath = currentPath + "\\" + folderName;
    return mkdir(newPath.c_str()) == 0;
}
bool deleteFile(string currentPath, string fileName)
{
    string fullPath = currentPath + "\\" + fileName;
    if (isFolder(currentPath, fileName))
    {
        vector<string> result = listFiles(fullPath);
        for (int i = 0; i < result.size(); i++)
        {
            if (isFolder(fullPath, result[i]))
            {
                deleteFile(fullPath, result[i]);
            }
            else
            {
                deleteFile(fullPath, result[i]);
            }
        }
        rmdir(fullPath.c_str());
    }
    else
    {
        remove(fullPath.c_str());
    }
    return true;
}
vector<string> listFiles(const string &path)
{
    vector<string> files;

    DIR *dir = opendir(path.c_str());
    if (dir == nullptr)
        return files;

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
void openFile(string fileName, string currentPath)
{
    currentPath = currentPath + "\\" + fileName;
    ShellExecute(0, TEXT("open"), currentPath.c_str(), 0, 0, SW_SHOW);
}