#pragma once //常用的C/C++预处理指令，保证头文件只被编译一次，防止头文件被重复引用

#include <iostream>
#include <string>
using std::string;

#include <ostream>
using std::ostream;

#include <vector>
#include <map>

/* Tips：
    1) 对类的非构造函数的其他成员函数的注释要写在函数声明的上方，这样在其他文件中当光标放在函数名上时也能够看到注释
*/

//创建新的声明区域，防止命名冲突
namespace yazi {
namespace json {

class Json
{
public:
    //使用枚举来定义符号常量
    enum Type
    {
        json_null = 0,  // null value
        json_bool,      // bool value
        //为了更具体且方便使用，对于number类型区分整数和浮点数
        json_int,       // integer value
        json_double,    // double value
        json_string,    // string value
        json_array,     // array value
        json_object     // object value
    };

    Json(); //不提供任何形参的默认构造函数
    Json(Type type); //根据枚举类型变量初始化Json对象
    Json(bool value);
    Json(int value);
    Json(double value);
    Json(const char *value); //支持C语言风格字符串初始化的构造函数
    Json(const string &value); //支持C++ string类型初始化的构造函数
    Json(const Json &other); //拷贝构造函数
    Json(Json && other);
    ~Json();

    Type type() const;

    //类型判断
    bool is_null() const;
    bool is_bool() const;
    bool is_int() const;
    bool is_double() const;
    bool is_string() const;
    bool is_array() const;
    bool is_object() const;

    //类型转换，实现Json类型向其他数据类型的转换
    bool as_bool() const;
    int as_int() const;
    double as_double() const;
    string as_string() const;

    // number of values in array or object(获取数组或对象容器中的元素个数)
    int size() const;

    // return true if empty array, empty object, or null, otherwise, false
    //(当数组为空、对象为空、或者为null时返回true，否则返回false)
    bool empty() const;

    //释放内存
    void clear();

    //判断vector中是否包含给定索引
    bool has(int index) const;
    //判断map中是否包含给定key
    bool has(const char * key) const;
    bool has(const string & key) const;

    Json get(int index) const;
    Json get(const char * key) const;
    Json get(const string & key) const;

    //删除vector中给定索引
    void remove(int index);
    //删除map中给定key
    void remove(const char * key);
    void remove(const string & key);

    // append value to array at the end(在数组末尾追加值)
    void append(const Json & value);
    void append(Json && value);

    Json & operator = (const Json & other);
    Json & operator = (Json && other);

    //判断两个Json对象是否相等
    bool operator == (const Json & other);
    bool operator != (const Json & other);

    Json & operator [] (int index);
    Json & operator [] (const char * key);
    Json & operator [] (const string & key);

    friend ostream & operator << (ostream & os, const Json & json)
    {
        os << json.str();
        return os;
    }

    //重载运算符，实现其他数据类型向Json类型的转换
    operator bool();
    operator int();
    operator double();
    operator string();
    operator string() const;

    void parse(const string & str);
    //以字符串的形式输出Json对象的内容
    string str() const;

    typedef std::vector<Json>::iterator iterator;
    iterator begin()
    {
        //内联函数，直接返回容器的迭代器
        return (m_value.m_array)->begin();
    }
    iterator end()
    {
        return (m_value.m_array)->end();
    }

private:
    //拷贝函数，对于指针类型成员变量全部执行深拷贝
    void copy(const Json & other);
    //交换函数
    void swap(Json & other);

private:
    //联合体中各成员变量共用内存空间，联合体内存空间的大小为最大成员变量占用内存空间的大小
    //因为联合体在某一时刻只能存储一种类型，因此可以节省内存空间(几乎是与结构体的唯一区别)
    //为了极致节省内存空间，对于容器类型，使用指针来存储，这样只需要24(=3*8)个字节(对于本系统而言)
    union Value
    {
        bool m_bool;
        int m_int;
        double m_double;
        std::string *m_string;
        std::vector<Json> *m_array;
        std::map<string, Json> *m_object;
    };
    //联合体对成员的索引方法同结构体，使用.运算符

    //对于任一JSON字段，只有类型与内容两种属性，因此定义枚举类型存储字段类型，用联合体类型存储字段内容
    Type m_type;
    Value m_value;
};

}}