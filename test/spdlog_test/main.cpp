#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <iostream>
#include <chrono>
// int main() {
//     //设置全局刷新策略
//     spdlog::flush_every(std::chrono::seconds(1));
//     spdlog::flush_on(spdlog::level::level_enum::debug);   //遇到debug以上日志立即刷新
//     //设置全局的日志输出等级（也可以不关心，每个日志器单独设置）
//     spdlog::set_level(spdlog::level::level_enum::debug);
//     //创建同步日志器 -- 工厂接口默认创建同步日志器
//     auto logger = spdlog::stdout_color_mt("default-logger");
//     //auto logger = spdlog::basic_logger_mt
//     logger->set_pattern("[%H:%M:%S][%t][%-7l - %v] ");
//     logger->trace("你好! {}", "小明");
//     logger->debug("你好! {}", "小明");
//     logger->info("你好! {}", "小明");
//     logger->warn("你好! {}", "小明");
//     logger->error("你好! {}", "小明");
//     logger->critical("你好! {}", "小明");
//     std::cout << "日志输出完毕" << std::endl; 
//     return 0;
// }


#include "logger.hpp"
#include <gflags/gflags.h>
DEFINE_bool(run_mode, false, "日志的运行模式, false - 调试模式, true - 发布模式");
DEFINE_string(log_file, "", "发布模式下，用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下，用于指定日志的输出等级");

int main(int argc, char* argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);
    LOG_DEBUG("你好啊: {}", "小明");
    LOG_INFO("你好啊: {}", "小明");
    LOG_ERROR("你好啊: {}", "小明");
    LOG_WARN("你好啊: {}", "小明");
    LOG_FATAL("你好啊: {}", "小明");
}