#include <iostream>
#include "json_rewrite/json_rewrite.h" //包含不在当前目录下的头文件时，要使用相对路径或绝对路径

using namespace std;
using namespace yazi_rewrite::json_rewrite;

/* Tips:
    1) g++ main_rewrite.cpp json_rewrite/json_rewrite.cpp json_rewrite/json_rewrite.h json_rewrite/parser_rewrite.h json_rewrite/parser_rewrite.cpp -o main
    2) 为了防止类名称空间冲突，将类改名为JsonRe，文件名添加后缀_rewrite
*/

int main()
{
    cout << "JsonRe main.cpp" << endl;

    // Json v1;
    // Json v2 = true;
    // Json v3 = 123;
    // Json v4 = 1.23;
    // Json v5 = "Hello World!";

    // bool b = v2;
    // int i = v3;
    // double f = v4;
    // const string &s = v5;

    // JsonRe arr;
    // arr[0] = true; 
    // arr[1] = 123;
    // 先调用转换构造函数将左边的内置类型转化为Json类型
    // 再调用下标访问运算符返回该位置的对象，最后调用默认的赋值运算符进行赋值
    // arr.append(1.23);
    // arr.append("Hello World!");
    // cout << arr.str() << endl;
    // for (auto it = arr.begin(); it != arr.end(); it++)
    // {
    //     cout << (*it).str() << endl;
    // }

    // cout << arr.has(0) << endl;
    // arr.remove(0);
    // cout << arr.str() << endl;

    // bool b = arr[0];
    // int i = arr[1];
    // double f = arr[2];
    // const string &s = arr[3];

    // JsonRe obj;
    // obj["bool"] = true;
    // obj["int"] = 123;
    // obj["double"] = 1.23;
    // obj["str"] = "Hello World!";
    // cout << obj.str() << endl;
    // cout << obj.has("bool") << endl;
    // obj.remove("int");
    // cout << obj.str() << endl;
    // obj.clear();

    const string & str = "null";
    JsonRe v;
    v.parse(str);

    return 0;
}