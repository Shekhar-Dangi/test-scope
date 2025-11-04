#include "testscope/http_client.hpp"
#include <httplib.h>
#include <iostream>

using namespace testscope;

#include "testscope/http_client.hpp"
#include <httplib.h>
#include <iostream>

using namespace testscope;

HttpResponse HttpClient::get(const std::string& url, 
                              const std::map<std::string, std::string>& headers) {
    HttpResponse response;
    
    try {
        std::string host, path;
        
        if (url.find("https://") == 0) {
            host = url.substr(8);
        } else if (url.find("http://") == 0) {
            host = url.substr(7);
        } else {
            response.error_message = "Invalid URL scheme";
            return response;
        }
        
        size_t path_pos = host.find('/');
        if (path_pos != std::string::npos) {
            path = host.substr(path_pos);
            host = host.substr(0, path_pos);
        } else {
            path = "/";
        }
        
        httplib::Client client(host);
        client.set_read_timeout(timeout_seconds_, 0);
        client.set_write_timeout(timeout_seconds_, 0);
        
        httplib::Headers req_headers;
        for (const auto& [key, value] : headers) {
            req_headers.insert({key, value});
        }
        
        auto res = client.Get(path.c_str(), req_headers);
        
        if (res) {
            response.status_code = res->status;
            response.body = res->body;
            response.success = true;
            
            for (const auto& [key, value] : res->headers) {
                response.headers[key] = value;
            }
        } else {
            response.success = false;
            response.error_message = "Request failed - " + httplib::to_string(res.error());
        }
        
    } catch (const std::exception& e) {
        response.success = false;
        response.error_message = std::string("Exception: ") + e.what();
    }
    
    return response;
}

HttpResponse HttpClient::post(const std::string& url, 
                               const std::string& body,
                               const std::map<std::string, std::string>& headers) {
    HttpResponse response;
    
    try {
        std::string host, path;
        
        if (url.find("https://") == 0) {
            host = url.substr(8);
        } else if (url.find("http://") == 0) {
            host = url.substr(7);
        } else {
            response.error_message = "Invalid URL scheme";
            return response;
        }
        
        size_t path_pos = host.find('/');
        if (path_pos != std::string::npos) {
            path = host.substr(path_pos);
            host = host.substr(0, path_pos);
        } else {
            path = "/";
        }
        
        httplib::Client client(host);
        client.set_read_timeout(timeout_seconds_, 0);
        client.set_write_timeout(timeout_seconds_, 0);
        
        httplib::Headers req_headers;
        for (const auto& [key, value] : headers) {
            req_headers.insert({key, value});
        }
        
        auto res = client.Post(path.c_str(), req_headers, body, "application/json");
        
        if (res) {
            response.status_code = res->status;
            response.body = res->body;
            response.success = true;
            
            for (const auto& [key, value] : res->headers) {
                response.headers[key] = value;
            }
        } else {
            response.success = false;
            response.error_message = "Request failed - " + httplib::to_string(res.error());
        }
        
    } catch (const std::exception& e) {
        response.success = false;
        response.error_message = std::string("Exception: ") + e.what();
    }
    
    return response;
}

// Perform HTTP POST request
HttpResponse HttpClient::post(const std::string& url, 
                               const std::string& body,
                               const std::map<std::string, std::string>& headers) {
    HttpResponse response;
    
    try {
        // Similar parsing as GET
        std::string host, path;
        bool is_https = false;
        
        if (url.find("https://") == 0) {
            is_https = true;
            host = url.substr(8);
        } else if (url.find("http://") == 0) {
            host = url.substr(7);
        } else {
            response.error_message = "Invalid URL scheme";
            return response;
        }
        
        size_t path_pos = host.find('/');
        if (path_pos != std::string::npos) {
            path = host.substr(path_pos);
            host = host.substr(0, path_pos);
        } else {
            path = "/";
        }
        
        auto client = std::make_unique<httplib::Client>(host);
        
        client->set_read_timeout(timeout_seconds_, 0);
        client->set_write_timeout(timeout_seconds_, 0);
        
        httplib::Headers req_headers;
        for (const auto& [key, value] : headers) {
            req_headers.insert({key, value});
        }
        
        auto res = client->Post(path.c_str(), req_headers, body, "application/json");
        
        if (res) {
            response.status_code = res->status;
            response.body = res->body;
            response.success = true;
            
            for (const auto& [key, value] : res->headers) {
                response.headers[key] = value;
            }
        } else {
            response.success = false;
            response.error_message = "Request failed - " + httplib::to_string(res.error());
        }
        
    } catch (const std::exception& e) {
        response.success = false;
        response.error_message = std::string("Exception: ") + e.what();
    }
    
    return response;
}
