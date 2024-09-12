#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/async.h>
#include <iostream>

//mode: true - 发布模式  false - 调试模式
namespace lcz_zpy {
    #define LOG_NAME "logger"
    #define LOG_FILE "log/runing.log"
    std::shared_ptr<spdlog::logger> global_logger;

    class StartupInitlialize {
        public:
            StartupInitlialize() {
                // 初始化异步线程数量和日志缓冲区节点数量
                spdlog::init_thread_pool(32768, 1);
                // 构造日志器
                spdlog::flush_every(std::chrono::seconds(1));
                spdlog::flush_on(spdlog::level::debug);
                global_logger = spdlog::basic_logger_mt(LOG_NAME, LOG_FILE);
                // 设置日志输出等级
                global_logger->set_level(spdlog::level::debug);
                // 设置日志输出格式
                global_logger->set_pattern("%H:%M:%S [%l] %v");
            }
    };

    // 全局静态实例，保证日志在启动时初始化
    static StartupInitlialize start_initialize;

    // 修改宏定义，直接使用 spdlog 的格式化功能
    #define DEBUG(format, ...) lcz_zpy::global_logger->debug("{}:{} " format, __FILE__, __LINE__, ##__VA_ARGS__);
    #define INFO(format, ...) lcz_zpy::global_logger->info("{}:{} " format, __FILE__, __LINE__, ##__VA_ARGS__);
    #define WARN(format, ...) lcz_zpy::global_logger->warn("{}:{} " format, __FILE__, __LINE__, ##__VA_ARGS__);
    #define ERROR(format, ...) lcz_zpy::global_logger->error("{}:{} " format, __FILE__, __LINE__, ##__VA_ARGS__);
    #define CRITICAL(format, ...) lcz_zpy::global_logger->critical("{}:{} " format, __FILE__, __LINE__, ##__VA_ARGS__);
}
