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
    if (position == string::npos)
    {
        return "Folder";
    }
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

string sizeSorter(long long bytes)
{
    if (bytes < 1024)
    {
        return to_string(bytes) + " B";
    }
    else if (bytes < 1048576)
    {
        return to_string(bytes / 1024) + " kB";
    }
    else if (bytes < 1073741824)
    {
        return to_string(bytes / 1048576) + " MB";
    }
    else if (bytes < 1099511627776)
    {
        return to_string(bytes / 1073741824) + " GB";
    }
    else
    {
        return to_string(bytes / 1099511627776) + " TB";
    }
}

string sizeSorter(long bytes)
{
    if (bytes < 1024)
    {
        return to_string(bytes) + " B";
    }
    else if (bytes < 1048576)
    {
        return to_string(bytes / 1024) + " kB";
    }
    else if (bytes < 1073741824)
    {
        return to_string(bytes / 1048576) + " MB";
    }
    else if (bytes < 1099511627776)
    {
        return to_string(bytes / 1073741824) + " GB";
    }
}
bool renameFile(string currentPath, string oldName, string newName)
{
    string oldPath = currentPath + "\\" + oldName;
    string newPath = currentPath + "\\" + newName;
    return rename(oldPath.c_str(), newPath.c_str()) == 0;
}
long long getDriveTotal(string drive)
{
    ULARGE_INTEGER freeBytes, totalBytes, totalFreeBytes;
    GetDiskFreeSpaceEx(drive.c_str(), &freeBytes, &totalBytes, &totalFreeBytes);
    return totalBytes.QuadPart;
}
long long getDriveFree(string drive)
{
    ULARGE_INTEGER freeBytes, totalBytes, totalFreeBytes;
    GetDiskFreeSpaceEx(drive.c_str(), &freeBytes, &totalBytes, &totalFreeBytes);
    return freeBytes.QuadPart;
}