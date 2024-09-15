#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/async.h>
#include <iostream>

namespace chat_im {
//mode: true - 发布模式  false - 调试模式

    std::shared_ptr<spdlog::logger> global_logger;

    void init_logger(bool mode, const std::string& filename, int level) {
        if(mode == false) {
            global_logger = spdlog::stdout_color_mt("default-logger");
            global_logger->set_level(spdlog::level::level_enum::trace);
            global_logger->flush_on(spdlog::level::level_enum::trace);
        } else {
            global_logger = spdlog::basic_logger_mt("default-logger", filename);
            global_logger->set_level((spdlog::level::level_enum)level);
            global_logger->flush_on((spdlog::level::level_enum)level);
        }
        global_logger->set_pattern("[%n][%H:%M:%S][%t][%-8l]%v");
    }


    // 修改宏定义，直接使用 spdlog 的格式化功能
    #define DEBUG(format, ...) global_logger->debug("{}:{} " format, __FILE__, __LINE__, ##__VA_ARGS__);
    #define INFO(format, ...) global_logger->info("{}:{} " format, __FILE__, __LINE__, ##__VA_ARGS__);
    #define WARN(format, ...) global_logger->warn("{}:{} " format, __FILE__, __LINE__, ##__VA_ARGS__);
    #define ERROR(format, ...) global_logger->error("{}:{} " format, __FILE__, __LINE__, ##__VA_ARGS__);
    #define CRITICAL(format, ...) global_logger->critical("{}:{} " format, __FILE__, __LINE__, ##__VA_ARGS__);
}