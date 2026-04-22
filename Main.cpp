#include <iostream>
#include <string>
#include <dirent.h>
#include <windows.h>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <cstdio>
using namespace std;
vector<string> listFiles(string path)
{
    vector<string> files;
    DIR *dir = opendir(path.c_str());
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (string(entry->d_name) == "." || string(entry->d_name) == "..")
        {
            // does nothing
        }
        else
        {
            files.push_back(string(entry->d_name));
        }
    }
    closedir(dir);
    return files;
}
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
    currentPath.substr(0, position);
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
        rmdir(fullPath.c_str());
    }
    else
    {
        remove(fullPath.c_str());
    }
    return rmdir(fullPath.c_str()) == 0;
}
int main()
{
    vector<string> result = listFiles("C:\\Users\\Haytham\\Desktop");
    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i] << endl;
    }
    string currentPath = "C:\\Users\\Haytham\\Desktop";
    string folderName = "Kindle Create.lnk";
    cout << isFolder(currentPath, folderName) << endl;
    cout << fileSize(currentPath, "desktop.ini") << endl;
    cout << fileType("desktop.ini") << endl;
    createFolder(currentPath, "I made this in VSCode");
    deleteFile(currentPath, "I made this in VSCode");
}