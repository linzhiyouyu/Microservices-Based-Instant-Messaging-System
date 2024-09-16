#include <gtest/gtest.h>
#include <string>
//TEST(测试名称, 测试用例名称) {}
//ASSERT_TRUE/ASSERT_FALSE

int add(int x, int y) { return x + y; }
TEST(test, add_test) {
    ASSERT_EQ(add(10, 20), 30);
}
TEST(test, string_compare_test) {
    std::string str = "hello";
    ASSERT_EQ(str, "hello");
}

int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    //开始单元测试
    return RUN_ALL_TESTS();
}