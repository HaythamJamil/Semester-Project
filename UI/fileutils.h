#pragma once
#include <string>
#include <vector>

using namespace std;

vector<string> listFiles(const string &path);
string enterFolder(string folderName, string currentPath);
string goBack(string currentPath);
bool isFolder(string currentPath, string folderName);
long fileSize(string currentPath, string folderName);
string fileType(string fileName);
bool createFolder(string currentPath, string folderName);
bool deleteFile(string currentPath, string fileName);
void openFile(string fileName, string currentPath);