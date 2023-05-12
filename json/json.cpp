#include <sstream>
using std::stringstream;

#include <stdexcept>

#include <json/json.h>
#include <json/parser.h>
using namespace yazi::json; //引入自定义的命名空间

/* Tips：
    1) 
    2) 
    3) 
*/

Json::Json() : m_type(json_null)
{
}

//涉及到构造函数的重载，根据传入的参数不同选择调用不同的构造函数
Json::Json(Type type) : m_type(type)
{
    //使switch语句的case标签可以使用枚举类型，但别忘记每种case都要有break语句
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
            m_value.m_string = new string("");
            break;
        case json_array:
            m_value.m_array = new std::vector<Json>();
            break;
        case json_object:
            m_value.m_object = new std::map<string, Json>();
        default:
            break;
    }
}

//使用成员初始化列表的方式初始化枚举类型变量
//使用成员运算符的方式初始化联合体类型变量
Json::Json(bool value) : m_type(json_bool)
{
    m_value.m_bool = value;
}

Json::Json(int value) : m_type(json_int)
{
    m_value.m_int = value;
}

Json::Json(double value) : m_type(json_double)
{
    m_value.m_double = value;
}

Json::Json(const char * value) : m_type(json_string)
{
    m_value.m_string = new string(value);
}

Json::Json(const string & value) : m_type(json_string)
{
    m_value.m_string = new string(value);
    //与上一个Json构造函数在开辟内存空间时调用了不同的string构造函数
}

Json::Json(const Json & other) : m_type(json_null)
{
    copy(other);
}

Json::Json(Json && other)
{
    swap(other);
}

Json::~Json()
{
    clear();
}

Json::Type Json::type() const
{
    return m_type;
}

bool Json::is_null() const
{
    return m_type == json_null;
}

bool Json::is_bool() const
{
    return m_type == json_bool;
}

bool Json::is_int() const
{
    return m_type == json_int;
}

bool Json::is_double() const
{
    return m_type == json_double;
}

bool Json::is_string() const
{
    return m_type == json_string;
}

bool Json::is_array() const
{
    return m_type == json_array;
}

bool Json::is_object() const
{
    return m_type == json_object;
}

bool Json::as_bool() const
{
    if (m_type == json_bool)
    {
        return m_value.m_bool;
    }
    throw std::logic_error("function Json::asBool value type error");
}

int Json::as_int() const
{
    if (m_type == json_int)
    {
        return m_value.m_int;
    }
    throw std::logic_error("function Json::asInt value type error");
}

double Json::as_double() const
{
    if (m_type == json_double)
    {
        return m_value.m_double;
    }
    throw std::logic_error("function Json::asDouble value type error");
}

string Json::as_string() const
{
    if (m_type == json_string)
    {
        return *(m_value.m_string);
    }
    throw std::logic_error("function Json::asString value type error");
}

void Json::copy(const Json & other)
{
    clear();
    m_type = other.m_type;
    switch (m_type)
    {
        case json_null:
        case json_bool:
        case json_int:
        case json_double:
            m_value = other.m_value;
            break;
        case json_string:
            {
                if (other.m_value.m_string != nullptr)
                {
                    m_value.m_string = new string(*(other.m_value.m_string));
                }
            }
            break;
        case json_array:
            {
                if (other.m_value.m_array != nullptr)
                {
                    m_value.m_array = new std::vector<Json>();
                    for (auto it = (other.m_value.m_array)->begin(); it != (other.m_value.m_array)->end(); it++)
                    {
                        m_value.m_array->push_back(*it);
                    }
                }
            }
            break;
        case json_object:
            {
                if (other.m_value.m_object != nullptr)
                {
                    m_value.m_object = new std::map<string, Json>();
                    for (auto it = (other.m_value.m_object)->begin(); it != (other.m_value.m_object)->end(); it++)
                    {
                        (*(m_value.m_object))[it->first] = it->second;
                    }
                }
            }
            break;
        default:
            break;
    }
}

void Json::swap(Json & other)
{
    Type type = m_type;
    Value value = m_value;
    m_type = other.m_type;
    m_value = other.m_value;
    other.m_type = type;
    other.m_value = value;
}

int Json::size() const
{
    switch (m_type)
    {
        case json_array:
            return (m_value.m_array)->size();
        case json_object:
            return (m_value.m_object)->size();
        default:
            break;
    }
    throw std::logic_error("function Json::size value type error");
}

bool Json::empty() const
{
    switch (m_type)
    {
        case json_null:
            return true;
        case json_array:
            return (m_value.m_array)->empty();
        case json_object:
            return (m_value.m_object)->empty();
        default:
            break;
    }
    return false;
}

void Json::clear()
{
    switch (m_type)
    {
        case json_null:
        case json_bool:
        case json_int:
        case json_double:
            break;
        case json_string:
            {
                if (m_value.m_string != nullptr)
                {
                    delete m_value.m_string;
                    m_value.m_string = nullptr;
                    //delete释放指针之后还要把指针置为空，这是一套连招
                }
            }
            break;
        case json_array:
            {
                if (m_value.m_array != nullptr)
                {
                    for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++)
                    {
                        it->clear(); //递归调用，因为vector中存放的是Json类型的对象
                    }
                    delete m_value.m_array;
                    m_value.m_array = nullptr;
                }
            }
            break;
        case json_object:
            {
                if (m_value.m_object != nullptr)
                {
                    for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++)
                    {
                        it->second.clear(); //递归调用，因为pair中第二个类型是Json类型
                    }
                    delete m_value.m_object;
                    m_value.m_object = nullptr;
                }
            }
            break;
        default:
            break;
    }
    m_type = json_null;
}

bool Json::has(int index) const
{
    if (m_type != json_array)
    {
        return false;
    }
    int size = (m_value.m_array)->size();
    return (index >= 0) && (index < size);
}

bool Json::has(const char * key) const
{
    string name(key);
    return has(name);
}

bool Json::has(const string & key) const
{
    if (m_type != json_object)
    {
        return false;
    }
    return (m_value.m_object)->find(key) != (m_value.m_object)->end();
    //当find()函数找不到key时，返回的是end()迭代器
}

Json Json::get(int index) const
{
    if (!has(index))
    {
        return Json();
    }
    return (*(m_value.m_array))[index];
}

Json Json::get(const char * key) const
{
    if (!has(key))
    {
        return Json();
    }
    return (*(m_value.m_object))[key];
}

Json Json::get(const string & key) const
{
    if (!has(key))
    {
        return Json();
    }
    return (*(m_value.m_object))[key];
}

void Json::remove(int index)
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

void Json::remove(const char * key)
{
    string name = key;
    remove(name);
}

void Json::remove(const string & key)
{
    if (m_type != json_object)
    {
        return;
    }
    auto it = (m_value.m_object)->find(key);
    //当find()函数找不到key时，返回的是end()迭代器
    if (it != (m_value.m_object)->end())
    {
        it->second.clear();
        (m_value.m_object)->erase(key);
    }
}

void Json::append(const Json & value)
{
    if (m_type != json_array)
    {
        clear();
        m_type = json_array;
        m_value.m_array = new std::vector<Json>();
    }
    (m_value.m_array)->push_back(value);
}

void Json::append(Json && value)
{
    if (m_type != json_array)
    {
        clear();
        m_type = json_array;
        m_value.m_array = new std::vector<Json>();
    }
    (m_value.m_array)->push_back(std::move(value));
}

Json & Json::operator = (const Json & other)
{
    clear();
    copy(other);
    return *this;
}

Json & Json::operator = (Json && other)
{
    swap(other);
    return *this;
}

bool Json::operator == (const Json & other)
{
    if (m_type != other.type())
    {
        return false;
    }
    switch (m_type)
    {
        case json_null:
            return true;
        case json_bool:
            return (m_value.m_bool == other.m_value.m_bool);
        case json_int:
            return (m_value.m_int == other.m_value.m_int);
        case json_double:
            return (m_value.m_double == other.m_value.m_double);
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

bool Json::operator != (const Json & other)
{
    return !(*this == other);
}

Json & Json::operator [] (int index)
{
    if (m_type != json_array)
    {
        throw std::logic_error("function [] not an array");
    }
    if (index < 0)
    {
        throw std::logic_error("function [] index less than 0");
    }
    int size = (m_value.m_array)->size(); //获取vector的大小
    if (index >= size)
    {
        throw std::logic_error("function [] out of range");
    }
    return (*(m_value.m_array))[index]; //vector容器本身有[]运算符重载
}

Json & Json::operator [] (const char * key)
{
    //使用字符串字面值构造string对象，进而将两个函数的思路统一
    string name = key;
    return (*this)[name];
}

Json & Json::operator [] (const string & key)
{
    if (m_type != json_object)
    {
        clear();
        m_type = json_object;
        m_value.m_object = new std::map<string, Json>();
    }
    return (*(m_value.m_object))[key];
}

Json::operator bool()
{
    if (m_type != json_bool)
    {
        throw std::logic_error("function Json::operator (bool) requires bool value");
    }
    return m_value.m_bool;
}

Json::operator int()
{
    if (m_type != json_int)
    {
        throw std::logic_error("function Json::operator (int) requires int value");
    }
    return m_value.m_int;
}

Json::operator double()
{
    if (m_type != json_double)
    {
        throw std::logic_error("function Json::operator (double) requires double value");
    }
    return m_value.m_double;
}

Json::operator string()
{
    if (m_type != json_string)
    {
        throw std::logic_error("function Json::operator (string) requires string value");
    }
    return *(m_value.m_string); //这里与上面稍有区别，因为存放的是指针，所以要解引用
}

Json::operator string() const
{
    if (m_type != json_string)
    {
        throw std::logic_error("function Json::operator (string) requires string value");
    }
    return *(m_value.m_string);
}

void Json::parse(const string & str)
{
    Parser parser;
    parser.load(str);
    *this = parser.parse();
}

string Json::str() const
{
    stringstream ss; //既可从string读数据，也可向string写数据
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
            ss << "\"" << *(m_value.m_string) << "\""; //注意输出格式，双引号都需要转义
            break;
        case json_array:
            {
                ss << "[";
                for (auto it = (m_value.m_array)->begin(); it != (m_value.m_array)->end(); it++)
                {
                    if (it != (m_value.m_array)->begin())
                    {   //数组各元素之间用逗号隔开
                        ss << ",";
                    }
                    ss << (*it).str();
                    //ss << it->str();
                }
                ss << "]";
            }
            break;
        case json_object:
            {
                ss << "{";
                for (auto it = (m_value.m_object)->begin(); it != (m_value.m_object)->end(); it++)
                {
                    if (it != (m_value.m_object)->begin())
                    {
                        ss << ",";
                    }
                    ss << "\"" << it->first << "\":" << it->second.str(); //迭代器指向的是pair类型
                }
                ss << "}";
            }
            break;
        default:
            break;
    }
    return ss.str();
}