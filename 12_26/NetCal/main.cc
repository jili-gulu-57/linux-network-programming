#include <iostream>
#include <jsoncpp/json/json.h>
#include <string>

int main()
{
    // 序列化
    Json::Value root;
    root["name"] = "wq";
    root["sex"] = "女";
    root["height"] = 1.6f;
    std::string s = root.toStyledString();
    std::cout << s;

    // 反序列化
    Json::Value droot;
    Json::Reader reader;
    reader.parse(s, droot);

    std::string name = droot["name"].asString();
    std::cout << name << std::endl;
    return 0;
}