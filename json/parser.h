#pragma once

#include <json/json.h>

namespace yazi {
namespace json {

class Parser
{
public:
    Parser(); //构造函数
    ~Parser(); //析构函数

    // 加载待解析的字符串
    void load(const string & str);

    // 返回解析完毕的Json对象
    Json parse();

private:
    // 忽略空白字符
    void skip_white_space();
    // 获取下一个字符
    char get_next_token();

    // 分别按照字段类型进行解析的私有成员函数
    Json parse_null();
    Json parse_bool();
    Json parse_number();
    // NOTE：这个解析函数的返回值与其他解析函数不同，因为它的功能要在不同地方使用到
    string parse_string();
    Json parse_array();
    Json parse_object();

    bool in_range(int x, int lower, int upper)
    {
        return (x >= lower && x <= upper);
    }

private:
    string m_str; //待解析的字符串
    size_t m_idx; //当前解析的位置/下标索引
};

}}
