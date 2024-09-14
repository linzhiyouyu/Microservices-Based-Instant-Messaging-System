#include <odb/database.hxx>
#include <odb/mysql/database.hxx>
#include "student.hxx"
#include "student-odb.hxx"
#include <gflags/gflags.h>
#include <memory>
#include "student-odb.cxx"
#include <iostream>
DEFINE_string(host, "127.0.0.1", "这是MySQL服务器地址");
DEFINE_int32(port, 3306, "这是MySQL服务端口");
DEFINE_string(db, "test_db", "数据库默认库名称");
DEFINE_string(user, "root", "用户名");
DEFINE_string(pswd, "123456", "密码");
DEFINE_string(cset, "utf8", "字符集");
DEFINE_int32(max_pool, 3, "最大连接池数量");

void insert(odb::mysql::database& db) {
    try {
        //3. 获取事务操作对象，开始事务
        odb::mysql::transaction trans(db.begin());
        Classes c1("一年级一班");
        Classes c2("一年级二班");
        db.persist(c1);
        db.persist(c2);
        //5. 提交事务
        trans.commit();
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
Classes select(odb::mysql::database& db) {
    Classes res;
    try {
        odb::transaction trans(db.begin());
        using query = odb::query<Classes>;
        using result = odb::result<Classes>;
        result r(db.query<Classes>(query::name == "一年级一班"));
        if(r.size() != 1) {
            std::cout << "数据量不正确" << std::endl;
            return Classes();
        }
        res = *r.begin();
        trans.commit();
    }catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
    return res;

}
void update(odb::mysql::database& db, Classes& c) {
    try {
        odb::transaction trans(db.begin());
        db.update(c);
        trans.commit();
    } catch(std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

int main(int argc, char* argv[]) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    //1. 构造连接池工厂配置对象
    std::unique_ptr<odb::mysql::connection_pool_factory> cpf(new odb::mysql::connection_pool_factory(FLAGS_max_pool, 0));
    //2. 构造数据库操作对象
    odb::mysql::database db(FLAGS_user, FLAGS_pswd, FLAGS_db, FLAGS_host, FLAGS_port, "", FLAGS_cset, 0, std::move(cpf));
    //4. 数据操作
    //insert(db);
    auto res = select(db);
    std::cout << res.name() << std::endl;
    res.name("一年级三班");
    update(db, res);
    return 0;
}