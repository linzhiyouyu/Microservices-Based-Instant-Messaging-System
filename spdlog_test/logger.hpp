#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/async.h>
#include <iostream>

//mode: true - 发布模式  false - 调试模式
std::shared_ptr<spdlog::logger> g_default_logger;
void init_logger(bool mode, const std::string& filename, int level) {
    //如果是调试模式，创建标准输出日志，输出等级为最低
    if(mode == false) {
        g_default_logger = spdlog::stdout_color_mt("default-logger");
        g_default_logger->set_level(spdlog::level::level_enum::trace);
        g_default_logger->flush_on(spdlog::level::level_enum::trace);
    } else {    //否则是发布模式，则创建文件输出日志器，输出等级为level
        g_default_logger = spdlog::basic_logger_mt("default-logger", filename);
        g_default_logger->set_level((spdlog::level::level_enum)level);
        g_default_logger->flush_on((spdlog::level::level_enum)level);
    }
    g_default_logger->set_pattern("[%n][%H:%M:%S][%t][%-8l]%v");
}

#define LOG_TRACE(format, ...) g_default_logger->trace(std::string {"[{}:{}] "} + format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) g_default_logger->debug(std::string {"[{}:{}] "} + format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(format, ...) g_default_logger->info(std::string {"[{}:{}] "} + format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_WARN(format, ...) g_default_logger->warn(std::string {"[{}:{}] "} + format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) g_default_logger->error(std::string {"[{}:{}] "} + format, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) g_default_logger->critical(std::string {"[{}:{}] "} + format, __FILE__, __LINE__, ##__VA_ARGS__)




