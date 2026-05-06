#include <iostream>
#include "fileutils.h"
using namespace std;
int main()
{
    cout << getDriveTotal("C:\\") << endl;
    cout << getDriveFree("C:\\") << endl;
    cout << getDriveTotal("D:\\") << endl;
    cout << getDriveFree("D:\\") << endl;
}