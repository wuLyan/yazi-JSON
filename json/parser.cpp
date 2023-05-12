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
    m_idx = 0;
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
    return m_str[m_idx++]; //在函数结束时实际上m_idx是待解析字符串中第二个非空白字符
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

Json Parser::parse_null()
{
    // HINT：当与被比较的字符串完全相等时，成员函数compare()返回0
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
        // HINT：整数部分的第一个有效数字只能在1~9之间，不能是0，但之后的数字可以是0，所以要加两重循环判断
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
    while (true)
    {
        if (m_idx == m_str.size())
        {
            throw std::logic_error("unexpected end of input in string");
        }

        char ch = m_str[m_idx++];
        if (ch == '"')
        {
            break;
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
                    break;
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
    Json arr(Json::json_array);
    char ch = get_next_token();
    if (ch == ']')
    {
        return arr;
    }
    m_idx--;
    while (true)
    {
        arr.append(parse());
        ch = get_next_token();
        if (ch == ']')
        {
            break;
        }
        if (ch != ',')
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
    m_idx--;
    while (true)
    {
        ch = get_next_token();
        if (ch != '"')
        {
            throw std::logic_error("expected '\"' in object");
        }
        string key = parse_string();
        ch = get_next_token();
        if (ch != ':')
        {
            throw std::logic_error("expected ':' in object");
        }
        obj[key] = parse();
        ch = get_next_token();
        if (ch == '}')
        {
            break;
        }
        if (ch != ',')
        {
            throw std::logic_error("expected ',' in object");
        }
    }
    return obj;
}