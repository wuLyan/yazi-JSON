#include "parser_rewrite.h"

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
    throw std::logic_error("unexpected character in parse json");
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
    return JsonRe();
}

JsonRe ParserRe::parse_bool()
{
    return JsonRe();
}

JsonRe ParserRe::parse_number()
{
    return JsonRe();
}

std::string ParserRe::parse_string()
{
    return std::string();
}

JsonRe ParserRe::parse_array()
{
    return JsonRe();
}

JsonRe ParserRe::parse_object()
{
    return JsonRe();
}