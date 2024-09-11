#include "../common/logger.hpp"

#include <fmt/core.h>
#include <string>

bool registry(const std::string& key, const std::string& value) {
    // 正确使用 fmt::format 进行格式化
    std::string formatted = fmt::format("Key: {}, Value: {}", key, value);
    fmt::print("你好啊: {}", formatted);
    return true;
}

int main() {
    registry("123", "2354");
    return 0;
}