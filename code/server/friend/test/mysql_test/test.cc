#include "mysql_chat_session.hpp"
#include "mysql_relation.hpp"
#include "mysql_apply.hpp"
#include "mysql_chat_session_member.hpp"
#include <gflags/gflags.h>


DEFINE_bool(run_mode, false, "日志的运行模式, false - 调试模式, true - 发布模式");
DEFINE_string(log_file, "", "发布模式下，用于指定日志的输出文件");
DEFINE_int32(log_level, 0, "发布模式下，用于指定日志的输出等级");
void r_insert_test(chat_im::RelationTable& tb) {
    tb.insert("用户ID1", "用户ID2");
    tb.insert("用户ID2", "用户ID3");
}


int main(int argc, char* argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    chat_im::init_logger(FLAGS_run_mode, FLAGS_log_file, FLAGS_log_level);
    auto db = chat_im::ODBFactory::create("lcz", "123456", "yanzzp.asuscomm.com", "chat_im", "utf8", 0, 1);
    chat_im::RelationTable tb1(db);
    chat_im::FriendApplyTable tb2(db);
    chat_im::ChatSessionTable tb3(db);
    r_insert_test(tb1);
    return 0;
}