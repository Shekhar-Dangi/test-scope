#pragma once

#include <vector>
#include <string>
#include <memory>
#include "test_case.hpp"

namespace testscope {

struct TestResults {
    std::vector<TestCase> test_cases;
    int total = 0;
    int passed = 0;
    int failed = 0;
    int skipped = 0;
    long total_time_ms = 0;
    
    void calculate() {
        total = test_cases.size();
        passed = 0;
        failed = 0;
        skipped = 0;
        
        for (const auto& test : test_cases) {
            switch (test.getStatus()) {
                case TestStatus::PASSED: passed++; break;
                case TestStatus::FAILED: failed++; break;
                case TestStatus::SKIPPED: skipped++; break;
            }
            total_time_ms += test.getExecutionTime();
        }
    }
};

class TestRunner {
private:
    std::vector<TestCase> test_cases_;
    std::string suite_name_;

public:
    TestRunner() = default;
    bool loadFromJson(const std::string& filepath);
    TestResults run();
    void addTest(const TestCase& test);
    const std::string& getSuiteName() const { return suite_name_; }
    
private:
    bool executeTest(TestCase& test);
};

}
