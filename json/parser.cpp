#include <string.h>
#include <stdexcept>
#include <cstdlib>

#include <json/parser.h>
using namespace yazi::json;

Parser::Parser() : m_idx(0)
{
}

Parser::~Parser()
{
}

void Parser::load(const string & str)
{
    m_str = str;
    m_idx = 0; //默认从待解析字符串的第一个字符开始解析
}

void Parser::skip_white_space()
{
    while (m_str[m_idx] == ' ' || m_str[m_idx] == '\r' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t')
        m_idx++;
}

char Parser::get_next_token()
{
    skip_white_space();
    if (m_idx == m_str.size())
        throw std::logic_error("unexpected end of input");
    return m_str[m_idx++]; //在函数结束时实际上m_idx指向待解析字符串中第二个非空白字符
}

Json Parser::parse()
{
    char ch = get_next_token();
    switch (ch)
    {
        // HINT：通过待解析字符串的第一个非空白字符判断字段类型
        case 'n':
            m_idx--; //将m_idx回退到待解析字符串中的第一个非空白字符，方便后续分字段类型解析的比较操作
            return parse_null();
        case 't':
        case 'f':
            m_idx--;
            return parse_bool();
        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            m_idx--;
            return parse_number();
        case '"':
            return Json(parse_string()); //通过解析函数的返回值构造一个匿名Json对象 
        case '[':
            return parse_array();
        case '{':
            return parse_object();
        default:
            break;
    }
    throw std::logic_error("unexpected character in parse json");
}

// NOTE：当Json对象的字段类型为json_null、json_bool、json_int、json_double、json_string时，解析函数可以直接在 return 语句中调用构造函数创建匿名对象返回
// NOTE：但是当Json对象的字段类型为json_array、json_object时，因为这些容器之中存放的还是Json对象，也就是实现了Json对象的嵌套，所以必须先通过字段类型创建空对象，然后逐个解析其中的元素向容器中插入

Json Parser::parse_null()
{
    // HINT：当与被比较的字符串完全相等时，成员函数compare()返回0(下同)
    // HINT：每解析完成一次都要将m_idx向后移动以跳过比较完成的Json对象(下同)
    // HINT：当Json对象的字段类型为json_null、json_bool、json_int、json_double、json_string时，比较完成后m_idx可以不向后移动，因为这个Json对象已经解析完成了
    // HINT：但是当Json对象的字段类型为json_array、json_object时，因为这些容器之中存放的还是Json对象，也就是实现了Json对象的嵌套，所以在比较完成之后m_idx必须向后移动
    if (m_str.compare(m_idx, 4, "null") == 0)
    {
        m_idx += 4;
        return Json();
    }
    throw std::logic_error("parse null error");
}

Json Parser::parse_bool()
{
    if (m_str.compare(m_idx, 4, "true") == 0)
    {
        m_idx += 4;
        return Json(true);
    }
    if (m_str.compare(m_idx, 5, "false") == 0)
    {
        m_idx += 5;
        return Json(false);
    }
    throw std::logic_error("parse bool error");
}

Json Parser::parse_number()
{
    size_t pos = m_idx; //保存开始解析的位置

    if (m_str[m_idx] == '-')
        m_idx++;

    // integer part(整数部分)
    if (m_str[m_idx] == '0') //跳过整数部分的前导0
    {
        m_idx++;
    }
    else if (in_range(m_str[m_idx], '1', '9'))
    {
        // HINT：整数部分的第一个有效数字只能在1~9之间，不能是0，但之后的数字可以是0，所以要加两重判断
        m_idx++;
        while (in_range(m_str[m_idx], '0', '9'))
        {
            m_idx++;
        }
    }
    else
    {
        throw std::logic_error("invalid character in number");
    }

    if (m_str[m_idx] != '.')
    {
        return Json(std::atoi(m_str.c_str() + pos));
        // HINT：c_str()函数将C++的string对象转化为C的字符串数组，即最后返回const char *类型的变量
        // HINT：但是因为可能前面跳过了某些空白字符，需要加上解析开始的位置，直到'\0'才是真正需要进行转换的数字(含正负号)
    }

    // decimal part(小数部分)
    // HINT：对于double类型的字段内容，分整数部分与小数部分两步进行处理
    m_idx++; //跳过小数点
    if (!in_range(m_str[m_idx], '0', '9'))
    {
        throw std::logic_error("at least one digit required in fractional part");
    }
    while (in_range(m_str[m_idx], '0', '9'))
    {
        m_idx++;
    }
    return Json(std::atof(m_str.c_str() + pos));
    // return Json(std::atod(m_str.c_str() + pos));
}

string Parser::parse_string()
{
    int pos = m_idx;
    while (true) //循环的主要作用是处理特殊字符，比如转义字符，并判断是否到达字符串结尾
    {
        if (m_idx == m_str.size())
        {
            throw std::logic_error("unexpected end of input in string");
        }

        char ch = m_str[m_idx++];
        if (ch == '"')
        {
            break; //跳出循环
        }

        // The usual case: non-escaped characters
        if (ch == '\\')
        {
            ch = m_str[m_idx++];
            switch (ch)
            {
                case 'b':
                case 't':
                case 'n':
                case 'f':
                case 'r':
                case '"':
                case '\\':
                    break; //跳出switch语句
                case 'u':
                    m_idx += 4;
                    break;
                default:
                    break;
            }
        }
    }
    return m_str.substr(pos, m_idx - pos - 1);
}

Json Parser::parse_array()
{
    // NOTE：为什么在函数parse_string()中使用 ch = m_str[m_idx++]; 获取下一个待解析字符，而在函数parse_array()中使用 char ch = get_next_token(); 获取下一个待解析字符？
    // NOTE：因为一旦判定为待解析Json对象是一个字符串，那么其中不管有多少空白字符都必须算作是字符串的一部分，而对于vector容器，两个元素之间的空白字符必须跳过，
    // NOTE：否则会使得空白字符算入下一个待解析字符的一部分，进而造成解析逻辑错误，在函数parse_object()中同理
    Json arr(Json::json_array);
    char ch = get_next_token();
    if (ch == ']')
    {
        return arr;
    }
    m_idx--; //回退到第一个待处理字符
    while (true)
    {
        arr.append(parse()); //arr中存放的是Json对象，所以递归调用函数进行解析
        ch = get_next_token();
        if (ch == ']') //判断这次循环解析是否到达了 vector 的结尾，若条件成立，则结束解析(break跳出当前循环)
        {
            break;
        }
        if (ch != ',') //若这次循环解析没有到达结尾，则期望元素之间使用逗号进行分隔
        {
            throw std::logic_error("expected ',' in array");
        }
    }
    return arr;
}

Json Parser::parse_object()
{
    Json obj(Json::json_object);
    char ch = get_next_token();
    if (ch == '}')
    {
        return obj;
    }
    m_idx--; //回退到第一个待处理字符
    while (true)
    {
        ch = get_next_token();
        if (ch != '"')
        {
            throw std::logic_error("expected '\"' in object");
            // key的数据类型是string，所以要求以 " 开头
        }
        string key = parse_string(); //解析key，这个用法就是决定为什么函数parse_string()不直接返回Json对象的原因
        // key 与 :、 ： 与 value 之间可能存在一些空白字符，因此调用函数get_next_token()获取下一个字符
        ch = get_next_token();
        if (ch != ':')
        {
            throw std::logic_error("expected ':' in object");
        }
        obj[key] = parse(); //解析value并向map中插入
        ch = get_next_token();
        if (ch == '}') //判断这次循环解析是否到达了 map 的结尾，若条件成立，则结束解析(break跳出当前循环)
        {
            break;
        }
        if (ch != ',') //若这次循环解析没有到达结尾，则键值对pair之间应该使用逗号进行分隔
        {
            throw std::logic_error("expected ',' in object");
        }
    }
    return obj;
}