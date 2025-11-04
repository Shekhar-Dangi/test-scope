#include "testscope/test_runner.hpp"
#include "testscope/json_parser.hpp"
#include <iostream>

using namespace testscope;

bool TestRunner::loadFromJson(const std::string& filepath) {
    try {
        std::vector<TestConfig> configs = JsonParser::parseTestFile(filepath, suite_name_);
        
        for (const auto& config : configs) {
            test_cases_.emplace_back(config);
        }
        
        std::cout << "Loaded " << test_cases_.size() << " test(s) from " << filepath << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error loading tests: " << e.what() << std::endl;
        return false;
    }
}

TestResults TestRunner::run() {
    TestResults results;
    
    std::cout << "\n🚀 Running test suite: " << suite_name_ << "\n" << std::endl;
    
    for (auto& test : test_cases_) {
        std::cout << "Running: " << test.getName() << "... " << std::flush;
        
        bool passed = executeTest(test);
        
        if (passed) {
            std::cout << "✓ PASSED" << std::endl;
        } else {
            std::cout << "✗ FAILED" << std::endl;
            if (!test.getErrorMessage().empty()) {
                std::cout << "  Error: " << test.getErrorMessage() << std::endl;
            }
        }
    }
    
    results.test_cases = std::move(test_cases_);
    results.calculate();
    
    return results;
}

void TestRunner::addTest(const TestCase& test) {
    test_cases_.push_back(test);
}

bool TestRunner::executeTest(TestCase& test) {
    return test.execute();
}
