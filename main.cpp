#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include <json/json.h>
using namespace yazi::json;

/* Tips��
    1) �Ȱ�Ctrl + k���ٰ�Ctrl + c������ע��ѡ�еĶ��д���
    2) �Ȱ�Ctrl + k���ٰ�Ctrl + u������ȡ��ע��ѡ�еĶ��д���(Ctrl + k��Ϊ�ȵ�����)
    3) ����Tab���������Զ�����ѡ�еĶ��д��룻����Shift + Tab�������Է�������ѡ�еĶ��д���
    4) �淶�Ĵ���д�����ؼ���for��if��while�����������ǰ�����ո���һ��ĺ����������ֿ�
    5) ����Ŀ���벻֧��Windows���������У���Ҫ��Linux������ʹ��makefile���������
*/

int main()
{
    // ifstream fin("./main.json");
    // stringstream ss;
    // ss << fin.rdbuf();
    // const string & data = ss.str();
    // Json v;
    // v.parse(data);
    // std::cout << v.str() << std::endl;
    // return 0;

    // Json v1 = true;
    // Json v2 = 123;
    // Json v3 = 1.23;
    // Json v4 = "hello world";
    // const string & s = v4;
    // std::cout << s << std::endl;
    // std::cout << v1.str() << std::endl;
    // std::cout << v2.str() << std::endl;
    // std::cout << v3.str() << std::endl;
    // std::cout << v4.str() << std::endl;
    // return 0;

    //Json����ת��Ϊ������������
    // bool b = v1;
    // int i = v2;
    // double f = v3;
    // const string & s = v4;

    // Json null;
    // std::cout << null.str() << std::endl;
    // return 0;

    // array value;
    // Json arr;
    // arr.append(123);
    // arr.append(1.23);
    // arr.append("hello world");
    // std::cout << arr.str() << std::endl;
    // std::cout << arr[0] << "," << arr[1] << "," << arr[2] << std::endl;
    // return 0;

    // object value
    // Json obj;
    // obj["name"] = "jun";
    // obj["age"] = 18;
    // obj["arr"] = arr; //Json�����Ƕ�ף�ע�ⲻҪ�ظ��ͷ��ڴ棬ֻ���ͷ�һ�Σ�obj.clear();
    // std::cout << obj.str() << std::endl;
    // obj.clear();
    // return 0;
}
