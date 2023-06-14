#ifndef __PARSER_REWRITE_H__
#define __PARSER_REWRITE_H__

#include "json_rewrite.h"
#include <string>

namespace yazi_rewrite {
namespace json_rewrite {

class ParserRe
{
public:
    ParserRe();
    ~ParserRe();

    void load(const std::string & str);
    JsonRe parse();

private:
    void skip_white_space();
    char get_next_token();

    JsonRe parse_null();
    JsonRe parse_bool();
    JsonRe parse_number();
    std::string parse_string();
    JsonRe parse_array();
    JsonRe parse_object();

private:
    std::string m_str;
    int m_idx;
};

}}

#endif