#include "json_rewrite.h"
#include "parser_rewrite.h"
#include <string>
#include <sstream>

using namespace yazi_rewrite::json_rewrite;

JsonRe::JsonRe() : m_type(json_null)
{

}

JsonRe::JsonRe(bool value) : m_type(json_bool)
{
    m_value.m_bool = value;
}

JsonRe::JsonRe(int value) : m_type(json_int)
{
    m_value.m_int = value;
}

JsonRe::JsonRe(double value) : m_type(json_double)
{
    m_value.m_double = value;
}

JsonRe::JsonRe(const char * value) : m_type(json_string)
{
    m_value.m_string = new std::string(value);
}

JsonRe::JsonRe(const std::string & value) : m_type(json_string)
{
    m_value.m_string = new std::string(value);
}

JsonRe::JsonRe(Type type) : m_type(type)
{
    switch (m_type)
    {
        case json_null:
            break;
        case json_bool:
            m_value.m_bool = false;
        case json_int:
            m_value.m_int = 0;
            break;
        case json_double:
            m_value.m_double = 0.0;\
            break;
        case json_string:
            m_value.m_string = new std::string(" ");
            break;
        case json_array:
            m_value.m_array = new std::vector<JsonRe>();
            break;
        case json_object:
            m_value.m_object = new std::map<std::string, JsonRe>();
            break;
        default:
            break;
    }
}

JsonRe::JsonRe(const JsonRe & other)
{
   copy(other);
}

JsonRe::operator bool() const
{
    if (m_type != json_bool)
    {
        throw new std::logic_error("type error, not bool value");
    }
    return m_value.m_bool;
}

JsonRe::operator int() const
{
    if (m_type != json_int)
    {
        throw new std::logic_error("type error, not int value");
    }
    return m_value.m_int;
}

JsonRe::operator double() const
{
    if (m_type != json_double)
    {
        throw new std::logic_error("type error, not double value");
    }
    return m_value.m_double;
}

JsonRe::operator std::string() const
{
    if (m_type != json_string)
    {
        throw new std::logic_error("type error, not string value");
    }
    return *(m_value.m_string);
}

JsonRe & JsonRe::operator[](int index)
{
    if (m_type != json_array)
    {
        m_type = json_array; //默认构造函数中对于空Json对象设置其字段类型m_type = json_null
        m_value.m_array = new std::vector<JsonRe>();
    }
    if (index < 0)
    {
        throw new std::logic_error("array[] index < 0");
    }
    int size = (m_value.m_array)->size();
    if (index >= size)
    {
        for (int i = size; i <= index; i++)
        {
            (m_value.m_array)->push_back(JsonRe());
        }
    }
    // 这是视频版本的代码，感觉上面代码逻辑存在问题，索引超出长度之后应该直接抛异常
    return (m_value.m_array)->at(index);
}

JsonRe & JsonRe::operator [] (const char * key)
{
    std::string name(key);
    return (*(this))[name];
}

JsonRe & JsonRe::operator [] (const std::string & key)
{
    if (m_type != json_object)
    {
        clear();
        m_type = json_object;
        m_value.m_object = new std::map<std::string, JsonRe>();
    }
    return (*(m_value.m_object))[key]; 
}

// 重载赋值运算符，函数的返回类型居然不是类型的引用，很迷！！！
void JsonRe::operator = (const JsonRe & other)
{
    clear(); //避免内存泄漏
    copy(other);
}

bool JsonRe::operator == (const JsonRe & other)
{
    if (m_type != other.m_type)
        return false;
    // return m_type == other.m_type;
    switch (m_type)
    {
        case json_null:
            return true;
        case json_bool:
            return m_value.m_bool == other.m_value.m_bool;
        case json_int:
            return m_value.m_int == other.m_value.m_int;
        case json_double:
            return m_value.m_double == other.m_value.m_double;
        case json_string:
            return *(m_value.m_string) == *(other.m_value.m_string);
        case json_array:
            return m_value.m_array == other.m_value.m_array;
        case json_object:
            return m_value.m_object == other.m_value.m_object;
        default:
            break;
    }
    return false;
}

bool JsonRe::operator != (const JsonRe & other)
{
    return !((*this) == other);
}

void JsonRe::append(const JsonRe &other)
{
    if (m_type != json_array)
    {
        clear();
        m_type = json_array;
        m_value.m_array = new std::vector<JsonRe>();
    }
    (m_value.m_array)->push_back(other);
}

std::string JsonRe::str() const
{
    std::stringstream ss;
    switch (m_type)
    {
        case json_null:
            ss << "null";
            break;
        case json_bool:
            if (m_value.m_bool)
            {
                ss << "true";
            }
            else
            {
                ss << "false";
            }
            break;
        case json_int:
            ss << m_value.m_int;
            break;
        case json_double:
            ss << m_value.m_double;
            break;
        case json_string:
            ss << "\"" << *(m_value.m_string) << "\"";
            break;
        case json_array:
            ss << "[";
            for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++)
            {
                if (it != (m_value.m_array)->begin())
                {
                    ss << ", ";
                }
                ss << (*it).str();
            }
            ss << "]";
            break;
        case json_object:
            ss << "{";
            for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++)
            {
                if (it != (m_value.m_object)->begin())
                {
                    ss << ", ";
                }
                ss << "\"" << it->first << "\": " << it->second.str();
            }
            ss << "}";
            break;
        default:
            break;
    }
    return ss.str();
}

// 这视频版本的代码中全部执行的是指针的浅拷贝，因此在释放内存时如果有Json对象的嵌套也只需要释放一次内存，千万注意不要重复释放
void JsonRe::copy(const JsonRe & other)
{
    m_type = other.m_type;
    switch (m_type)
    {
        case json_null:
            break;
        case json_bool:
            m_value.m_bool = other.m_value.m_bool;
        case json_int:
            m_value.m_int = other.m_value.m_int;
            break;
        case json_double:
            m_value.m_double = other.m_value.m_double;
            break;
        case json_string:
            m_value.m_string = other.m_value.m_string; //浅拷贝
            break;
        case json_array:
            m_value.m_array = other.m_value.m_array; //浅拷贝
            break;
        case json_object:
            m_value.m_object = other.m_value.m_object; //浅拷贝
            break;
        default:
            break;
    }
}

void JsonRe::clear()
{
    switch (m_type)
    {
        case json_null:
            break;
        case json_bool:
            m_value.m_bool = false;
            break;
        case json_int:
            m_value.m_int = 0;
            break;
        case json_double:
            m_value.m_double = 0.0;
            break;
        case json_string:
            {
                delete m_value.m_string;
                break;
            }
        case json_array:
            {
                for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++)
                {
                    it->clear();
                }
                delete m_value.m_array;
                break;
            }
        case json_object:
            {
                for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++)
                {
                    (it->second).clear();
                }
                delete m_value.m_object;
                break;
            }
        default:
            break;
    }
    m_type = json_null;
}

bool JsonRe::as_bool() const
{
    if (m_type != json_bool)
    {
        throw new std::logic_error("type error, not bool value");
    }
    return m_value.m_bool;
        
}
int JsonRe::as_int() const
{
    if (m_type != json_int)
    {
        throw new std::logic_error("type error, not int value");
    }
    return m_value.m_int;
}

double JsonRe::as_double() const
{
    if (m_type != json_double)
    {
        throw new std::logic_error("type error, not double value");
    }
    return m_value.m_double;
}

std::string JsonRe::as_string() const
{
    if (m_type != json_string)
    {
        throw new std::logic_error("type error, not string value");
    }
    return *(m_value.m_string);
}

bool JsonRe::has(int index) const
{
    if (m_type != json_array)
    {
        return false;
    }   
    int size = (m_value.m_array)->size();
    return (index >= 0 && index < size);
}

bool JsonRe::has(const char * key) const
{
    std::string name(key);
    return has(name);
}

bool JsonRe::has(const std::string & key) const
{
    if (m_type != json_object)
    {
        return false;
    }
    return ((m_value.m_object)->find(key) != (m_value.m_object)->end());
}

void JsonRe::remove(int index)
{
    if (m_type != json_array)
    {
        return;
    }
    int size = (m_value.m_array)->size();
    if (index < 0 || index >= size)
    {
        return;
    }
    (m_value.m_array)->at(index).clear();
    (m_value.m_array)->erase((m_value.m_array)->begin() + index);
}

void JsonRe::remove(const char * key)
{
    std::string name(key);
    return remove(name);
}

void JsonRe::remove(const std::string & key)
{
    if (m_type != json_object)
    {
        return;
    }
    auto it = (m_value.m_object)->find(key);
    if (it == (m_value.m_object)->end())
    {
        return;
    }
    (*(m_value.m_object))[key].clear();
    (m_value.m_object)->erase(key);
}

void JsonRe::parse(const std::string & str)
{
    ParserRe p;
    p.load(str);
    *this = p.parse();
}