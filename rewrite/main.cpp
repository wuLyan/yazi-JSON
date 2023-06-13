#include <iostream>
#include "json\json.h" //包含不在当前目录下的头文件时，要使用相对路径或绝对路径

using namespace std;
using namespace yazi::json;

/* Tips:
    1) g++ main.cpp json/json.cpp json/json.h -o main
*/

int main()
{
    cout << "JSON main.cpp" << endl;

    Json v1;
    Json v2 = true;
    Json v3 = 123;
    Json v4 = 1.23;
    Json v5 = "Hello World!";

    return 0;
}