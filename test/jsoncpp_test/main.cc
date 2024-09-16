#include <json/json.h>
#include <memory>
#include <sstream>
#include <string>
#include <iostream>

int main() {
    char name[] = "张三";
    int age = 18;
    float score[3] = {88, 90.5, 86.5};
    Json::Value stu;
    stu["姓名"] = name;
    stu["年龄"] = age;
    stu["成绩"].append(score[0]);
    stu["成绩"].append(score[1]);
    stu["成绩"].append(score[2]);
    std::string dst;
    auto serialize = [&]() {
        Json::StreamWriterBuilder swb;
        std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());
        std::stringstream ss;
        int ret = sw->write(stu, &ss);
        if(ret != 0) {
            std::cout << "序列化失败" << std::endl;
            return false;
        }
        dst = ss.str();
        return true;
    };
    serialize();
    std::cout << dst << std::endl;
    Json::Value ans;
    std::string err;
    auto unserialize = [&]() {
        Json::CharReaderBuilder crb;
        std::unique_ptr<Json::CharReader> cr(crb.newCharReader());
        bool ret = cr->parse(dst.c_str(), dst.c_str() + dst.size(), &ans, &err);
        if(ret == false) {
            std::cout << "反序列化失败: " << err << std::endl; 
            return false;
        }
        return true;
    };
    unserialize();
    std::cout << ans["姓名"].asString() << std::endl;
    std::cout << ans["年龄"].asInt() << std::endl;
    int sz = ans["成绩"].size();
    for(int i = 0; i < sz; ++i)
        std::cout << ans["成绩"][i].asFloat() << std::endl;
    return 0;
}