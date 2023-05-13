#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include <json/json.h>
using namespace yazi::json;

/* Tips：
    1) 先按Ctrl + k，再按Ctrl + c，可以注释选中的多行代码
    2) 先按Ctrl + k，再按Ctrl + u，可以取消注释选中的多行代码(Ctrl + k称为先导按键)
    3) 按下Tab键，可以自动缩进选中的多行代码；按下Shift + Tab键，可以反向缩进选中的多行代码
    4) 规范的代码写法：关键字for、if、while后面的左括号前面留空格，与一般的函数调用区分开
    5) 本项目代码不支持Windows环境下运行，需要在Linux环境下使用makefile编译后运行
*/

int main()
{
    /* 对*.json文件的理解：
        1) 首先整个文件本身就是一个Json对象，因为文件以 { 开头，以 } 结尾，这个Json对象的字段类型为json_object
        2) Json文件中可以实现多层嵌套，所以这就决定了当处理字段类型为json_object和json_vector的Json对象时，成员函数的实现要依靠递归调用
        3) 看了*.json文件格式就会明白为什么在进行解析时要对空白字符(空格、回车、换行、指标)进行处理，为什么不同的解析函数获取下一个待解析字符的操作不同
    */
    ifstream fin("./test.json"); //创建一个文件输入流对象，并将其与main.json文件关联起来
    stringstream ss; //创建一个字符串流对象

    // HINT：ifstream类的成员函数rdbuf()返回一个指向内部 filebuf 对象的指针，将文件输入流对象fin中的数据读取到字符串流对象ss中
    ss << fin.rdbuf();
    const string & data = ss.str(); //返回字符串流对象ss所保存的string对象的拷贝

    Json v;
    v.parse(data);
    std::cout << v.str() << std::endl;

    // 这里要用到Json对象向基本数据类型的转换，从索引方式可以看到Json对象存在多层嵌套
    bool isLogin = v["data"]["isLogin"];
    int current_level = v["data"]["level_info"]["current_level"];

    return 0;

    // NOTE：因为构造函数在声明时并没有使用关键字explicit，所以可以进行拷贝初始化来构造Json对象
    // Json v1 = true;
    // Json v2 = 123;
    // Json v3 = 1.23;
    // Json v4 = "hello world";
    // std::cout << v1.str() << std::endl;
    // std::cout << v2.str() << std::endl;
    // std::cout << v3.str() << std::endl;
    // std::cout << v4.str() << std::endl;
    // return 0;

    // Json类型转换为基本数据类型
    // bool b = v1;
    // int i = v2;
    // double f = v3;
    // const string & s = v4;

    // Json null;
    // std::cout << null.str() << std::endl;
    // return 0;

    // array value(数组类型)
    // Json arr;
    // HINT：这里在调用append()函数时，会设置Json类型的对象arr的成员变量 mtype = json_array
    // HINT：这里在调用append()函数时，会调用Json的转换构造函数，因为append()函数的参数是Json类型的引用，而不是内置数据类型
    // arr.append(123); //这里的123会被转换为Json对象，等价于arr.append(Json(123));
    // arr.append(1.23);
    // arr.append("hello world");
    // std::cout << arr.str() << std::endl;
    // std::cout << arr[0] << "," << arr[1] << "," << arr[2] << std::endl;
    // return 0;

    // object value(对象类型)
    // Json obj;
    // obj["name"] = "jun";
    // obj["age"] = 18;
    // obj["arr"] = arr; //Json对象的嵌套，注意不要重复释放内存，只需释放一次：obj.clear()，函数内部会自己递归调用
    // std::cout << obj.str() << std::endl;
    // obj.clear();
    // return 0;
}
