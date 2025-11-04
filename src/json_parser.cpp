#include "testscope/json_parser.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;
using namespace testscope;

std::vector<TestConfig> JsonParser::parseTestFile(const std::string& filepath, 
                                                    std::string& suite_name) {
    std::vector<TestConfig> configs;
    
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }
    
    json j;
    file >> j;
    
    if (!j.contains("test_suite")) {
        throw std::runtime_error("JSON must contain 'test_suite' object");
    }
    
    json test_suite = j["test_suite"];
    
    if (test_suite.contains("name")) {
        suite_name = test_suite["name"].get<std::string>();
    } else {
        suite_name = "Unnamed Suite";
    }
    
    if (!test_suite.contains("tests")) {
        throw std::runtime_error("test_suite must contain 'tests' array");
    }
    
    json tests = test_suite["tests"];
    if (!tests.is_array()) {
        throw std::runtime_error("'tests' must be an array");
    }
    
    for (const auto& test_json : tests) {
        configs.push_back(parseTestObject(test_json));
    }
    
    return configs;
}

TestConfig JsonParser::parseTestObject(const json& test_json) {
    TestConfig config;
    
    if (!test_json.contains("name")) {
        throw std::runtime_error("Test must have 'name' field");
    }
    config.name = test_json["name"].get<std::string>();
    
    if (!test_json.contains("type")) {
        throw std::runtime_error("Test must have 'type' field");
    }
    config.type = stringToTestType(test_json["type"].get<std::string>());
    
    if (config.type == TestType::HTTP) {
        if (test_json.contains("method")) {
            config.method = test_json["method"].get<std::string>();
        } else {
            config.method = "GET";
        }
        
        if (!test_json.contains("url")) {
            throw std::runtime_error("HTTP test must have 'url' field");
        }
        config.url = test_json["url"].get<std::string>();
        
        if (test_json.contains("headers")) {
            for (auto& [key, value] : test_json["headers"].items()) {
                config.headers[key] = value.get<std::string>();
            }
        }
    }
    
    if (config.type == TestType::CLI) {
        if (test_json.contains("command")) {
            config.command = test_json["command"].get<std::string>();
        }
    }
    
    if (test_json.contains("expect")) {
        config.expect = parseExpectation(test_json["expect"]);
    }
    
    return config;
}

TestExpectation JsonParser::parseExpectation(const json& expect_json) {
    TestExpectation expect;
    
    if (expect_json.contains("status")) {
        expect.status_code = expect_json["status"].get<int>();
    }
    
    if (expect_json.contains("contains")) {
        expect.contains = expect_json["contains"].get<std::string>();
    }
    
    if (expect_json.contains("equals")) {
        expect.equals = expect_json["equals"].get<std::string>();
    }
    
    if (expect_json.contains("timeout_ms")) {
        expect.timeout_ms = expect_json["timeout_ms"].get<int>();
    }
    
    return expect;
}

TestType JsonParser::stringToTestType(const std::string& type_str) {
    if (type_str == "http" || type_str == "HTTP") {
        return TestType::HTTP;
    } else if (type_str == "cli" || type_str == "CLI") {
        return TestType::CLI;
    } else {
        return TestType::UNKNOWN;
    }
}
