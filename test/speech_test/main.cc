#include "../../code/server/common/asr.hpp"
#include "../../code/server/common/logger.hpp"
#include "gflags/gflags.h"
#include <json/json.h>
#include <fstream>
#include <iostream>

DEFINE_bool(run_mode, false, "程序的运行模式，false-调试； true-发布；");
DEFINE_string(log_file, "", "发布模式下，用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下，用于指定日志输出等级");

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

int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    chat_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    std::string app_id;
    std::string api_key;
    std::string secret_key;

    // 加载配置文件
    if (!load_config("../config.json", app_id, api_key, secret_key))
    {
        return 1; // 失败时退出
    }

    chat_im::ASRClient client(app_id, api_key, secret_key);

    std::string file_content, err;
    aip::get_file_content("16k.pcm", &file_content);
    std::string res = client.recognize(file_content, err);
    if (res.empty())
    {
        chat_im::ERROR("识别失败：{}", err);
    }
    else
    {
        chat_im::INFO("识别结果：{}", res);
    }

    return 0;
}
