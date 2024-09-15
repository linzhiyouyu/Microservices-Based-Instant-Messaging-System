#include <cstdlib>
#include <iostream>
#include <fstream>
#include <json/json.h>
#include <alibabacloud/core/AlibabaCloud.h>
#include <alibabacloud/core/CommonRequest.h>
#include <alibabacloud/core/CommonClient.h>
#include <alibabacloud/core/CommonResponse.h>


using namespace AlibabaCloud;

bool load_config(const std::string &file_path, std::string &app_id, std::string &api_key)
{
    std::ifstream file(file_path, std::ifstream::binary);
    if (!file.is_open())
    {
        std::cerr << "无法打开配置文件: " << file_path << std::endl;
        return false;
    }

    Json::Value config;
    Json::CharReaderBuilder reader;
    std::string errs;

    // 解析 JSON 文件
    if (!Json::parseFromStream(reader, file, &config, &errs))
    {
        std::cerr << "解析配置文件时出错: " << errs << std::endl;
        return false;
    }

    // 获取 app_id 和 api_key
    if (config.isMember("ali_api_id") && config["ali_api_id"].isString())
    {
        app_id = config["ali_api_id"].asString();
    }
    else
    {
        std::cerr << "配置文件中缺少 ali_api_id 或 ali_api_id 类型错误" << std::endl;
        return false;
    }

    if (config.isMember("ali_api_key") && config["ali_api_key"].isString())
    {
        api_key = config["ali_api_key"].asString();
    }
    else
    {
        std::cerr << "配置文件中缺少 ali_api_key 或 ali_api_key 类型错误" << std::endl;
        return false;
    }

    return true;
}

int main(int argc, char **argv)
{
    AlibabaCloud::InitializeSdk();
    AlibabaCloud::ClientConfiguration configuration("cn-shanghai");
    // specify timeout when create client.
    configuration.setConnectTimeout(1500);
    configuration.setReadTimeout(4000);
    std::string app_id;
    std::string api_key;

    if (!load_config("../config.json", app_id, api_key))
    {
        return 1; // 失败时退出
    }

    // Please ensure that the environment variables ALIBABA_CLOUD_ACCESS_KEY_ID and ALIBABA_CLOUD_ACCESS_KEY_SECRET are set.
    AlibabaCloud::Credentials credential(app_id, api_key);
    /* use STS Token
    credential.setSessionToken( getenv("ALIBABA_CLOUD_SECURITY_TOKEN") );
    */
    AlibabaCloud::CommonClient client(credential, configuration);
    AlibabaCloud::CommonRequest request(AlibabaCloud::CommonRequest::RequestPattern::RpcPattern);
    request.setHttpMethod(AlibabaCloud::HttpRequest::Method::Post);
    request.setDomain("dysmsapi.aliyuncs.com");
    request.setVersion("2017-05-25");
    request.setQueryParameter("Action", "SendSms");
    request.setQueryParameter("SignName", "chatim聊天室");
    request.setQueryParameter("TemplateCode", "SMS_473345171");
    request.setQueryParameter("PhoneNumbers", "18106008871");
    request.setQueryParameter("TemplateParam", "{\"code\":\"1234\"}");

    auto response = client.commonResponse(request);
    if (response.isSuccess())
    {
        printf("request success.\n");
        printf("result: %s\n", response.result().payload().c_str());
    }
    else
    {
        printf("error: %s\n", response.error().errorMessage().c_str());
        printf("request id: %s\n", response.error().requestId().c_str());
    }

    AlibabaCloud::ShutdownSdk();
    return 0;
}