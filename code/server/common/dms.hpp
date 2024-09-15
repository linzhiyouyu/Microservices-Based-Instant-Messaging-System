#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#include <alibabacloud/core/AlibabaCloud.h>
#include <alibabacloud/core/CommonRequest.h>
#include <alibabacloud/core/CommonClient.h>
#include <alibabacloud/core/CommonResponse.h>
#include "logger.hpp"

namespace chat_im
{
    class DMSClient
    {
    public:
        using ptr = std::shared_ptr<DMSClient>;
        DMSClient(const std::string &access_key_id,
                  const std::string &access_key_secret)
        {
            AlibabaCloud::InitializeSdk();
            AlibabaCloud::ClientConfiguration configuration("cn-chengdu");
            configuration.setConnectTimeout(1500);
            configuration.setReadTimeout(4000);
            AlibabaCloud::Credentials credential(access_key_id, access_key_secret);
            _client = std::make_unique<AlibabaCloud::CommonClient>(credential, configuration);
        }
        ~DMSClient() { AlibabaCloud::ShutdownSdk(); }
        bool send(const std::string &phone, const std::string &code)
        {
            AlibabaCloud::CommonRequest request(AlibabaCloud::CommonRequest::RequestPattern::RpcPattern);
            request.setHttpMethod(AlibabaCloud::HttpRequest::Method::Post);
            request.setDomain("dysmsapi.aliyuncs.com");
            request.setVersion("2017-05-25");
            request.setQueryParameter("Action", "SendSms");
            request.setQueryParameter("SignName", "chatim聊天室");
            request.setQueryParameter("TemplateCode", "SMS_473345171");
            request.setQueryParameter("PhoneNumbers", phone);
            std::string param_code = "{\"code\":\"" + code + "\"}";
            request.setQueryParameter("TemplateParam", param_code);
            auto response = _client->commonResponse(request);
            if (!response.isSuccess())
            {
                ERROR("短信验证码请求失败：{}", response.error().errorMessage());
                return false;
            }
            return true;
        }

    private:
        std::unique_ptr<AlibabaCloud::CommonClient> _client;
    };
}