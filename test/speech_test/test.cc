#include "../../code/server/common/asr.hpp"
#include <json/json.h>
#include <fstream>
#include <iostream>
#include <map>

// 从 JSON 文件加载配置
bool load_config(const std::string &file_path, std::string &app_id, std::string &api_key, std::string &secret_key)
{
    std::ifstream file(file_path, std::ifstream::binary);
    if (!file.is_open())
    {
        std::cerr << "无法打开配置文件: " << file_path << std::endl;
        return false;
    }

    Json::Value config;
    Json::CharReaderBuilder reader;
    std::string errs;

    // 解析 JSON 文件
    if (!Json::parseFromStream(reader, file, &config, &errs))
    {
        std::cerr << "解析配置文件时出错: " << errs << std::endl;
        return false;
    }

    // 获取 app_id, api_key 和 secret_key
    if (config.isMember("baidu_app_id") && config["baidu_app_id"].isString())
    {
        app_id = config["baidu_app_id"].asString();
    }
    else
    {
        std::cerr << "配置文件中缺少 baidu_app_id 或 baidu_app_id 类型错误" << std::endl;
        return false;
    }

    if (config.isMember("baidu_api_key") && config["baidu_api_key"].isString())
    {
        api_key = config["baidu_api_key"].asString();
    }
    else
    {
        std::cerr << "配置文件中缺少 baidu_api_key 或 baidu_api_key 类型错误" << std::endl;
        return false;
    }

    if (config.isMember("baidu_secret_key") && config["baidu_secret_key"].isString())
    {
        secret_key = config["baidu_secret_key"].asString();
    }
    else
    {
        std::cerr << "配置文件中缺少 baidu_secret_key 或 baidu_secret_key 类型错误" << std::endl;
        return false;
    }

    return true;
}

void asr(aip::Speech &client)
{
    // 如果需要覆盖或者加入参数
    std::map<std::string, std::string> options;
    options["dev_pid"] = "1537";
    std::string file_content;
    aip::get_file_content("./16k.pcm", &file_content);
    Json::Value result = client.recognize(file_content, "pcm", 16000, options);

    std::cout << "语音识别本地文件结果:" << std::endl
              << result.toStyledString() << std::endl;

    if (result["err_no"].asInt() != 0)
    {
        std::cout << result["err_msg"].asString() << std::endl;
        return;
    }
    else
    {
        std::cout << result["result"][0].asString() << std::endl;
    }
}

int main()
{
    std::string app_id, api_key, secret_key;

    // 从 config.json 加载配置文件
    if (!load_config("../config.json", app_id, api_key, secret_key))
    {
        return 1; // 加载配置失败，退出
    }

    // 初始化语音识别客户端
    aip::Speech client(app_id, api_key, secret_key);

    // 调用语音识别函数
    asr(client);

    return 0;
}
