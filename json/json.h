#pragma once //常用的C/C++预处理指令，保证头文件只被编译一次，防止头文件被重复引用
#include <iostream>
#include <string>
using std::string;

#include <ostream>
using std::ostream;

#include <vector>
#include <map>

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

    Json();
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

    bool is_null() const;
    bool is_bool() const;
    bool is_int() const;
    bool is_double() const;
    bool is_string() const;
    bool is_array() const;
    bool is_object() const;

    bool as_bool() const;
    int as_int() const;
    double as_double() const;
    string as_string() const;

    // number of values in array or object
    int size() const;

    // return true if empty array, empty object, or null, otherwise, false.
    bool empty() const;

    void clear();

    bool has(int index) const;
    bool has(const char * key) const;
    bool has(const string & key) const;

    Json get(int index) const;
    Json get(const char * key) const;
    Json get(const string & key) const;

    void remove(int index);
    void remove(const char * key);
    void remove(const string & key);

    // append value to array at the end.
    void append(const Json & value);
    void append(Json && value);

    Json & operator = (const Json & other);
    Json & operator = (Json && other);

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

    operator bool();
    operator int();
    operator double();
    operator string();
    operator string() const;

    void parse(const string & str);
    string str() const;

    typedef std::vector<Json>::iterator iterator;
    iterator begin()
    {
        return (m_value.m_array)->begin();
    }
    iterator end()
    {
        return (m_value.m_array)->end();
    }

private:
    void copy(const Json & other); //拷贝函数
    void swap(Json & other); //交换函数

private:
    //结构体中各成员变量共用内存空间，结构体内存空间的大小为最大成员变量占用内存空间的大小
    //因为结构体在某一时刻只能存储一种类型，因此可以节省内存空间(几乎是与结构体的唯一区别)
    //为了极致节省内存空间，对于容器类型，使用指针来存储，这样只需要8个字节(对于本系统而言)
    union Value
    {
        bool m_bool;
        int m_int;
        double m_double;
        std::string *m_string;
        std::vector<Json> *m_array;
        std::map<string, Json> *m_object;
    };

    //对于任一JSON字段，只有类型与值两种属性，因此定义枚举类型存储字段类型，用联合体类型存储字段值
    Type m_type;
    Value m_value;
};

}}