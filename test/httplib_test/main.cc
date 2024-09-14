#include "../common/httplib.h"

int main() {
    httplib::Server server;
    server.Get("/hi", [&](const httplib::Request& req, httplib::Response& rsp) { 
        std::cout << req.method << std::endl;
        std::cout << req.path << std::endl;
        for(auto&& [k, v] : req.headers) 
            std::cout << k << ": " << v << endl;
        std::string body = "<html><body><h1>Hello Bit</h1></body></html>";
        rsp.set_content(body, "text/html");
        rsp.status = 200;
    });
    return 0;
}