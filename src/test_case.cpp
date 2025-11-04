#include "testscope/test_case.hpp"
#include "testscope/http_client.hpp"
#include "testscope/assertions.hpp"
#include <chrono>
#include <iostream>

using namespace testscope;

TestCase::TestCase(const TestConfig& config) 
    : config_(config), 
      status_(TestStatus::SKIPPED), 
      error_message_(""),
      execution_time_ms_(0) {
}

bool TestCase::execute() {
    auto start = std::chrono::high_resolution_clock::now();
    
    bool result = false;
    
    try {
        if (config_.type == TestType::HTTP) {
            HttpClient client;
            client.setTimeout(config_.expect.timeout_ms / 1000);
            
            HttpResponse response;
            if (config_.method == "GET") {
                response = client.get(config_.url, config_.headers);
            } else if (config_.method == "POST") {
                response = client.post(config_.url, "", config_.headers);
            }
            
            if (!response.success) {
                status_ = TestStatus::FAILED;
                error_message_ = "HTTP request failed: " + response.error_message;
                result = false;
            } else {
                bool status_ok = true;
                bool content_ok = true;
                
                if (!Assert::statusCode(config_.expect.status_code, response.status_code)) {
                    status_ok = false;
                    error_message_ = "Expected status " + 
                                   std::to_string(config_.expect.status_code) + 
                                   " but got " + std::to_string(response.status_code);
                }
                
                if (!config_.expect.contains.empty()) {
                    if (!Assert::contains(response.body, config_.expect.contains)) {
                        content_ok = false;
                        error_message_ += (error_message_.empty() ? "" : "; ");
                        error_message_ += "Response doesn't contain: " + config_.expect.contains;
                    }
                }
                
                if (!config_.expect.equals.empty()) {
                    if (!Assert::equals(response.body, config_.expect.equals)) {
                        content_ok = false;
                        error_message_ += (error_message_.empty() ? "" : "; ");
                        error_message_ += "Response doesn't match expected value";
                    }
                }
                
                if (status_ok && content_ok) {
                    status_ = TestStatus::PASSED;
                    result = true;
                } else {
                    status_ = TestStatus::FAILED;
                    result = false;
                }
            }
            
        } else if (config_.type == TestType::CLI) {
            status_ = TestStatus::SKIPPED;
            error_message_ = "CLI tests not yet implemented";
            result = false;
        } else {
            status_ = TestStatus::FAILED;
            error_message_ = "Unknown test type";
            result = false;
        }
        
    } catch (const std::exception& e) {
        status_ = TestStatus::FAILED;
        error_message_ = std::string("Exception: ") + e.what();
        result = false;
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    execution_time_ms_ = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    
    return result;
}
