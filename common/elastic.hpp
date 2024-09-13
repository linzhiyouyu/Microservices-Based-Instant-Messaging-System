#include <elasticlient/client.h>
#include <cpr/cpr.h>
#include "logger.hpp"
#include <memory>
#include <iostream>
#include <json/json.h>

class ESIndex {
public:
    ESIndex(std::shared_ptr<elasticlient::Client> client, 
            const std::string& name, 
            const std::string& type):
            _name(name), _type(type), _client(client) {
        Json::Value analysis, analyzer, ik, tokenizer;
        tokenizer["tokenizer"] = "ik_max_word";
        ik["ik"] = tokenizer;
        analyzer["analyzer"]  = ik;
        analysis["anslysis"] = analyzer;
        _index["settings"] = analysis;
    }
    ESIndex append(const std::string& key, 
                const std::string& type = "text", 
                const std::string& analyzer = "ik_max_word", 
                bool enabled = true) {
        Json::Value field;
        field["type"] = type;
        field["analyzer"] = analyzer;
        if(enabled == false)
            field["enabled"] = enabled; 
        _properties["key"] = field;
        return *this;
    }
    bool create() {
        Json::Value mappings;
        mappings["dynamic"] = true;
        mappings["properties"] = _properties;
        _index["mapping"] = mappings;

        std::string body;
        auto serialize = [&]() {
            Json::StreamWriterBuilder swb;
            std::unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());
            std::stringstream ss;
            int ret = sw->write(_index, &ss);
            if(ret != 0) {
                ERROR("json序列化失败");
                return false;
            }
            body = ss.str();
            return true;
        };
        if(serialize() == false) {
            ERROR("索引序列化失败");
            return false;
        }
        INFO("请求正文:[{}]", body);
        try {
            auto rsp = _client->index(_name, _type, "", body);
            if(rsp.status_code < 200 || rsp.status_code >= 300) {
                ERROR("创建es索引{}失败，响应状态码异常：{}", _name, rsp.status_code);
                return false;
            }
        } catch(std::exception& e) {
            ERROR("创建es索引{}失败:{}", _name, e.what());
            return false;
        }
        return true;
    }
private:
    std::string _name;
    std::string _type;
    Json::Value _index;
    Json::Value _properties;
    std::shared_ptr<elasticlient::Client> _client;
};