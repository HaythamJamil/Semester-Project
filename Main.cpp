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
int main()
{
    vector<string> result = listFiles("C:\\Users\\Haytham\\Desktop");
    for (int i = 0; i < result.size(); i++)
    {
        cout << result[i] << endl;
    }
}