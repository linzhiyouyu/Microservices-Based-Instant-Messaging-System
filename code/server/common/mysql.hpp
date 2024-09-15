#pragma once
#include <string>
#include <memory>  // std::auto_ptr
#include <cstdlib> // std::exit
#include <iostream>
#include <odb/database.hxx>
#include <odb/mysql/database.hxx>
#include "logger.hpp"

namespace chat_im
{
    class ODBFactory
    {
    public:
        static std::shared_ptr<odb::core::database> create(
            const std::string &user,
            const std::string &pswd,
            const std::string &host,
            const std::string &db,
            const std::string &cset,
            int port,
            int conn_pool_count)
        {
            std::unique_ptr<odb::mysql::connection_pool_factory> cpf(
                new odb::mysql::connection_pool_factory(conn_pool_count, 0));
            auto res = std::make_shared<odb::mysql::database>(user, pswd,
                                                              db, host, port, "", cset, 0, std::move(cpf));
            return res;
        }
    };
}