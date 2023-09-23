# yazi-json

a tiny c++ json parser

## example

main.json
```json
{
    "name": "jack",
    "age": 30,
    "area": ["c++", "php", "python", "go"]
}
```

main.cpp
```C++
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include <json/json.h>
using namespace yazi::json;

int main()
{
    try
    {
        ifstream fin("./main.json");
        stringstream ss;
        ss << fin.rdbuf();
        const string & data = ss.str();

        Json json;
        json.parse(data);
        std::cout << json.str() << std::endl;

        const string & name = json["name"];
        int type = json["age"];
        const string & area = json["area"][0];
        json.clear();
    }
    catch (std::exception & e)
    {
        std::cout << "catch exception: " << e.what() << std::endl;
    }
    return 0;
}
```

## 项目简介

- JSON是一种轻量级的数据交换格式，其规定了七种数据类型，分别是空值、布尔、数值(整型、浮点型)、字符、数组、对象。**在程序中，空值、布尔、数值、字符都可以用基本数据类型表示，而数组使用vector、对象使用map(键为string类型，值为Json类型)表示**，因此在解析Json时，主要是解析对象和数组类型的数据，其中使用到了一些类设计时的基础操作，比如构造函数、转换构造函数、析构函数、运算符重载函数等等，在代码的重构方面，在Json对象的拷贝、内存的释放方面实现了代码的复用，整体的代码行数在600行左右。**同时也与几个开源库进行十万次解析的对比，计算平均每次解析耗时**
- 首先定义了名为Json的类，**用枚举类型定义字段类型，用联合体类型定义字段内容**，Json类型的对象使用联合体类型可以节省内存空间。**其中因为字符、数组、对象占用的内存空间不确定，所以联合体中存放的是指向它们的指针，而空值、布尔、数值占用的内存空间确定，所以联合体中存放的是它们的值**
- 因为.json文件以`{ `开头，以`} `结尾，所以整个文件本身就是一个字段类型为json_object的Json对象，然后其中的每个元素的类型都是json_string或json_number或json_array或json_object，**因此在解析字段内容、释放字段所占用的内存空间或以字符串形式输出Json对象的内容时，都采用相应函数的递归调用，达到在实现相同功能的前提下缩减代码行数的目的**
- 程序可能存在的改进空间是，由于联合体中存放的是指向字符、数组、对象的指针，所以在释放内存空间时，需要对每个元素的类型进行判断，然后再释放，这样会增加代码的复杂度，**可以考虑使用智能指针来管理内存空间，这样就不需要对每个元素的类型进行判断，直接释放即可**；另外，在拷贝时执行的是浅拷贝，因此在内存释放时需要手动释放原有内存，这进一步增加了用户使用的负担，**可以考虑使用深拷贝，这样在内存释放时就不需要手动释放原有内存，减少了用户使用的负担**；最后，**可以考虑使用模板类，这样就可以实现对任意类型的Json对象的解析，而不仅仅是字符串类型的Json对象的解析**