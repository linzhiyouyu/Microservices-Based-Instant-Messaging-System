#include "../../code/server/common/dms.hpp"
#include "../../code/server/common/logger.hpp"
#include "gflags/gflags.h"
#include <json/json.h>
#include <fstream>
#include <iostream>

DEFINE_bool(run_mode, false, "程序的运行模式，false-调试； true-发布；");
DEFINE_string(log_file, "", "发布模式下，用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下，用于指定日志输出等级");

bool load_config(const std::string &file_path, std::string &app_id, std::string &api_key)
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

    // 获取 app_id 和 api_key
    if (config.isMember("ali_api_id") && config["ali_api_id"].isString())
    {
        app_id = config["ali_api_id"].asString();
    }
    else
    {
        std::cerr << "配置文件中缺少 ali_api_id 或 ali_api_id 类型错误" << std::endl;
        return false;
    }

    if (config.isMember("ali_api_key") && config["ali_api_key"].isString())
    {
        api_key = config["ali_api_key"].asString();
    }
    else
    {
        std::cerr << "配置文件中缺少 ali_api_key 或 ali_api_key 类型错误" << std::endl;
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

    // 加载配置文件
    if (!load_config("../config.json", app_id, api_key))
    {
        return 1; // 失败时退出
    }

    chat_im::DMSClient client(app_id, api_key);
    client.send("18106008871", "666666");
    return 0;
}