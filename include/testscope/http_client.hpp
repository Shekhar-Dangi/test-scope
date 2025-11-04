#pragma once

#include <string>
#include <map>

namespace testscope {

struct HttpResponse {
    int status_code = 0;
    std::string body;
    std::map<std::string, std::string> headers;
    bool success = false;
    std::string error_message;
};

class HttpClient {
public:
    HttpClient() = default;
    HttpResponse get(const std::string& url, 
                     const std::map<std::string, std::string>& headers = {});
    HttpResponse post(const std::string& url, 
                      const std::string& body,
                      const std::map<std::string, std::string>& headers = {});
    void setTimeout(int seconds) { timeout_seconds_ = seconds; }

private:
    int timeout_seconds_ = 5;
};

}
