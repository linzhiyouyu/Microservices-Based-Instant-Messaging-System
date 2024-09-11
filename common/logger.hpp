#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/async.h>
#include <iostream>

//mode: true - 发布模式  false - 调试模式
namespace perfect {
    #define LOG_NAME "logger"
    #define LOG_FILE "log/runing.log"
    std::shared_ptr<spdlog::logger> global_logger;

    class StartupInitlialize {
        public:
            StartupInitlialize() {
                //初始化异步线程数量和日志缓冲区节点数量
                spdlog::init_thread_pool(32768, 1);
                //构造日志器
                spdlog::flush_every(std::chrono::seconds(1));
                spdlog::flush_on(spdlog::level::debug);
                global_logger = spdlog::basic_logger_mt(LOG_NAME, LOG_FILE);
                //global_logger = spdlog::stdout_color_mt(LOG_NAME);
                //设置日志输出等级
                global_logger->set_level(spdlog::level::debug);
                //设置日志输出格式
                global_logger->set_pattern("%H:%M:%S [%l] %v");
            }
    };
    static StartupInitlialize start_initialize;

    #define DEBUG(format, ...) perfect::global_logger->debug(std::string("{}:{} ") + format, __FILE__, __LINE__, ##__VA_ARGS__);
    #define INFO(format, ...) perfect::global_logger->info(std::string("{}:{} ") + format, __FILE__, __LINE__, ##__VA_ARGS__);
    #define WARN(format, ...) perfect::global_logger->warn(std::string("{}:{} ") + format, __FILE__, __LINE__, ##__VA_ARGS__);
    #define ERROR(format, ...) perfect::global_logger->error(std::string("{}:{} ") + format, __FILE__, __LINE__, ##__VA_ARGS__);
    #define CRITICAL(format, ...) perfect::global_logger->critical(std::string("{}:{} ") + format, __FILE__, __LINE__, ##__VA_ARGS__);
}