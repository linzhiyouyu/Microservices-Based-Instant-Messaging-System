#include "../../code/server/common/data_es.hpp"

#include <gflags/gflags.h>

DEFINE_bool(run_mode, false, "程序的运行模式，false-调试； true-发布；");
DEFINE_string(log_file, "", "发布模式下，用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下，用于指定日志输出等级");

DEFINE_string(es_host, "http://127.0.0.1:9200/", "es服务器URL");
DEFINE_string(es_username, "elastic", "Elasticsearch 用户名");
DEFINE_string(es_password, "", "Elasticsearch 密码");

int main(int argc, char *argv[])
{
    google::ParseCommandLineFlags(&argc, &argv, true);
    chat_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);

    auto es_client = chat_im::ESClientFactory::create({FLAGS_es_host});

    auto es_user = std::make_shared<chat_im::ESUser>(es_client);
    es_user->createIndex();
    es_user->appendData("用户ID1", "手机号1", "小猪佩奇", "这是一只小猪", "小猪头像1");
    es_user->appendData("用户ID2", "手机号2", "小猪乔治", "这是一只小小猪", "小猪头像2");
    auto res = es_user->search("小猪", {"用户ID1"});
    for (auto &u : res)
    {
        std::cout << "-----------------" << std::endl;
        std::cout << u.user_id() << std::endl;
        std::cout << u.phone() << std::endl;
        std::cout << u.nickname() << std::endl;
        std::cout << u.description() << std::endl;
        std::cout << u.avatar_id() << std::endl;
    }
    return 0;
}