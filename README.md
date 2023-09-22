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

- JSON是一种轻量级的数据交换格式，其规定了七种数据类型，分别是布尔、数值、对象、数组、字符、空值。首先整个文件本身就是一个Json对象，因为文件以 { 开头，以 } 结尾，这个Json对象的字段类型为json_object
- 首先定义了名为Json的类，**用枚举类型定义字段类型，用联合体类型定义字段内容**，Json类型的对象使用联合体类型可以节省内存空间
- 在数组或对象类型的解析时，因为数组元素或者对象的值都有可能是字符串或数值，因此在解析字段内容或释放字段所占用的内存空间时，都实现了字段类型解析函数或内存释放函数的递归调用
- 其中使用到了一些比较类的基础操作，比如构造函数、转换构造函数、析构函数等等，在代码的重构方面，在Json对象的拷贝、内存的释放方面实现了代码的复用