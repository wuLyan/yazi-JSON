#ifndef __JSON_REWRITE_H__
#define __JSON_REWRITE_H__

#include <string>
#include <vector>
#include <map>

namespace yazi_rewrite {
namespace json_rewrite {

class JsonRe
{
public:
    enum Type
    {
        json_null = 0,
        json_bool,
        json_int,
        json_double,
        json_string,
        json_array,
        json_object
    };

    JsonRe();
    JsonRe(bool value);
    JsonRe(int value);
    JsonRe(double value);
    JsonRe(const char *value);
    JsonRe(const std::string & value);
    JsonRe(Type type);
    JsonRe(const JsonRe & other);

    // 类型转换运算符
    operator bool() const;
    operator int() const;
    operator double() const;
    operator std::string() const;

    // 下标访问运算符
    JsonRe & operator [] (int index);
    JsonRe & operator [] (const char * key);
    JsonRe & operator [] (const std::string & key);

    void operator = (const JsonRe & other); //视频版本中重载赋值运算符的返回值居然不是类类型的引用？？？
    bool operator == (const JsonRe & other);
    bool operator != (const JsonRe & other);
    
    void append(const JsonRe &other);

    std::string str() const;

    void copy(const JsonRe & other);
    void clear();

    typedef std::vector<JsonRe>::iterator iteratorJsonRe;
    iteratorJsonRe begin()
    {
        return (m_value.m_array)->begin();
    }
    iteratorJsonRe end()
    {
        return (m_value.m_array)->end();
    }

    bool is_null() const { return m_type == json_null; }
    bool is_bool() const { return m_type == json_bool; }
    bool is_int() const {return m_type == json_int; }
    bool is_double() const { return m_type == json_double; }
    bool is_string() const { return m_type == json_string; }
    bool is_array() const { return m_type == json_array; }
    bool is_object() const { return m_type == json_object; }

    bool as_bool() const;
    int as_int() const;
    double as_double() const;
    std::string as_string() const;

    bool has(int index) const;
    bool has(const char * key) const;
    bool has(const std::string & key) const;

    void remove(int index);
    void remove(const char * key);
    void remove(const std::string & key);

    void parse(const std::string & str);

private:
    union Value
    {
        bool m_bool;
        int m_int;
        double m_double;
        std::string *m_string;
        std::vector<JsonRe> *m_array;
        std::map<std::string, JsonRe> *m_object;
    };

    Type m_type;
    Value m_value;
};

}}

#endif