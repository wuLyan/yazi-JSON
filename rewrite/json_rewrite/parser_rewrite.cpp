#include "parser_rewrite.h"
#include  <string>

using namespace yazi_rewrite::json_rewrite;

ParserRe::ParserRe() : m_str(" "), m_idx(0)
{

}
ParserRe::~ParserRe()
{

}

void ParserRe::load(const std::string & str)
{
    m_str = str;
    m_idx = 0;
}

JsonRe ParserRe::parse()
{
    skip_white_space();
    char ch = get_next_token();
    switch (ch)
    {
        case 'n':
            m_idx --;
            return parse_null();
        case 't':
        case 'f':
            m_idx --;
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
            m_idx --;
            return parse_number();
        case '"':
            return JsonRe(parse_string());
        case '[':
            return parse_array();
        case '{':
            return parse_object();
        default:
            break;
    }
    throw new std::logic_error("unexpected character in parse json");
}

void ParserRe::skip_white_space()
{
    while (m_str[m_idx] == ' ' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t' || m_str[m_idx] == '\r')
    {
        m_idx ++;
    }
}

char ParserRe::get_next_token()
{
    skip_white_space();
    return m_str[m_idx ++];
}

JsonRe ParserRe::parse_null()
{
    if (m_str.compare(m_idx, 4, "null") == 0)
    {
        m_idx += 4;
        return JsonRe();
    }
    throw new std::logic_error("parse null error");
}

JsonRe ParserRe::parse_bool()
{
    if (m_str.compare(m_idx, 4, "true") == 0)
    {
        m_idx += 4;
        return JsonRe(true);
    }
    else if (m_str.compare(m_idx, 5, "false") == 0)
    {
        m_idx += 5;
        return JsonRe(false);
    }
    throw new std::logic_error("parse bool error");
}

JsonRe ParserRe::parse_number()
{
    int pos = m_idx;
    if (m_str[m_idx] == '-')
    {
        m_idx ++;
    }
    if (m_str[m_idx] < '0' || m_str[m_idx] > '9')
    {
        throw new std::logic_error("parse number error");
    }
    while (m_str[m_idx] >= '0' && m_str[m_idx] <= '9')
    {
        m_idx ++;
    }
    if (m_str[m_idx] != '.')
    {
        int i = std::atoi(m_str.c_str() + pos);
        return JsonRe(i);
    }
    m_idx ++;
    if (m_str[m_idx] < '0' || m_str[m_idx] > '9')
    {
        throw new std::logic_error("parse number error");
    }
    while (m_str[m_idx] >= '0' && m_str[m_idx] <= '9')
    {
        m_idx ++;
    }
    double f = std::atof(m_str.c_str() + pos);
    return JsonRe(f);
}

std::string ParserRe::parse_string()
{
    std::string out;
    while (true)
    {
        char ch = m_str[m_idx ++];
        if (ch == '"')
        {
            break;
        }
        if (ch == '\\')
        {
            ch = m_str[m_idx ++];
            switch (ch)
            {
                case '\n':
                    out += '\n'; //这里在字符串连接时，把转义字符整体连接到后面
                    break;
                case '\r':
                    out += '\r';
                    break;
                case '\t':
                    out += '\t';
                    break;
                case '\b':
                    out += '\b';
                    break;
                case '\f':
                    out += '\f';
                    break;
                case '"':
                    out += "\\\"";
                    break;
                case '\\':
                    out += "\\\\";
                    break;
                case 'u':
                    out += "\\u";
                    for (int i = 0; i < 4; i ++)
                    {
                        out += m_str[m_idx ++];
                    }
                    break;
                default:
                    break;
            }
        }
        else
        {
            out += ch;
        }
    }
    return out;
}

JsonRe ParserRe::parse_array()
{
    JsonRe arr(JsonRe::json_array);
    char ch = get_next_token();
    if (ch == ']')
    {
        return arr;
    }
    m_idx --;
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
            throw new std::logic_error("parse array error");
        }
        m_idx ++;
    }
    return arr;
}

JsonRe ParserRe::parse_object()
{
    JsonRe obj(JsonRe::json_object);
    char ch = get_next_token();
    if (ch == '}')
    {
        return obj;
    }
    m_idx --;
    while (true)
    {
        ch = get_next_token();
        if (ch != '"')
        {
            throw new std::logic_error("parse object error");
        }
        std::string key = parse_string();
        ch = get_next_token();
        if (ch != ':')
        {
            throw new std::logic_error("parse object error");
        }
        obj[key] = parse();
        ch = get_next_token();
        if (ch == '}')
        {
            break;
        }
        if (ch != ',')
        {
            throw new std::logic_error("parse object error");
        }
    }
    return obj;
}