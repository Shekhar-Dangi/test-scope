#pragma once

#include <string>
#include <vector>
#include "test_case.hpp"

namespace testscope {

class JsonParser {
public:
    static std::vector<TestConfig> parseTestFile(const std::string& filepath, 
                                                  std::string& suite_name);
    
private:
    static TestConfig parseTestObject(const nlohmann::json& test_json);
    static TestExpectation parseExpectation(const nlohmann::json& expect_json);
    static TestType stringToTestType(const std::string& type_str);
};

}
