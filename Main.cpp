#include <iostream>
#include <string>
#include <dirent.h>
#include <windows.h>
#include <vector>
#include <sstream>
#include <sys/stat.h>
using namespace std;
vector<string> listFiles(string path)
{
    vector<string> files;
    DIR *dir = opendir(path.c_str());
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        files.push_back(string(entry->d_name));
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
}