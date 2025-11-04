#pragma once

#include <string>
#include <map>

namespace testscope {

enum class TestType {
    HTTP,
    CLI,
    UNKNOWN
};

enum class TestStatus {
    PASSED,
    FAILED,
    SKIPPED
};

struct TestExpectation {
    int status_code = 200;
    std::string contains = "";
    std::string equals = "";
    int timeout_ms = 5000;
};

struct TestConfig {
    std::string name;
    TestType type;
    std::string method;
    std::string url;
    std::string command;
    std::map<std::string, std::string> headers;
    TestExpectation expect;
};

class TestCase {
private:
    TestConfig config_;
    TestStatus status_;
    std::string error_message_;
    long execution_time_ms_;

public:
    explicit TestCase(const TestConfig& config);
    bool execute();
    
    const std::string& getName() const { return config_.name; }
    TestStatus getStatus() const { return status_; }
    const std::string& getErrorMessage() const { return error_message_; }
    long getExecutionTime() const { return execution_time_ms_; }
    TestType getType() const { return config_.type; }
    
    void setStatus(TestStatus status) { status_ = status; }
    void setErrorMessage(const std::string& msg) { error_message_ = msg; }
    void setExecutionTime(long ms) { execution_time_ms_ = ms; }
    
    const TestConfig& getConfig() const { return config_; }
};

}
