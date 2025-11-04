#include "testscope/reporter.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace testscope;

const std::string Reporter::RESET = "\033[0m";
const std::string Reporter::RED = "\033[31m";
const std::string Reporter::GREEN = "\033[32m";
const std::string Reporter::YELLOW = "\033[33m";
const std::string Reporter::BLUE = "\033[34m";
const std::string Reporter::BOLD = "\033[1m";

void Reporter::printToConsole(const TestResults& results, const std::string& suite_name) {
    std::cout << "\n" << BOLD << "=" << std::string(60, '=') << RESET << std::endl;
    std::cout << BOLD << "  Test Results: " << suite_name << RESET << std::endl;
    std::cout << BOLD << "=" << std::string(60, '=') << RESET << "\n" << std::endl;
    
    for (const auto& test : results.test_cases) {
        std::string symbol = getStatusSymbol(test.getStatus());
        std::string color = getStatusColor(test.getStatus());
        
        std::cout << color << symbol << " " << test.getName() << RESET;
        std::cout << " (" << test.getExecutionTime() << "ms)" << std::endl;
        
        if (test.getStatus() == TestStatus::FAILED && !test.getErrorMessage().empty()) {
            std::cout << "  " << RED << "└─ " << test.getErrorMessage() << RESET << std::endl;
        }
    }
    
    std::cout << "\n" << BOLD << "-" << std::string(60, '-') << RESET << std::endl;
    std::cout << BOLD << "Summary:" << RESET << std::endl;
    std::cout << "  Total:   " << results.total << std::endl;
    std::cout << GREEN << "  Passed:  " << results.passed << RESET << std::endl;
    std::cout << RED << "  Failed:  " << results.failed << RESET << std::endl;
    std::cout << YELLOW << "  Skipped: " << results.skipped << RESET << std::endl;
    std::cout << "  Time:    " << results.total_time_ms << "ms" << std::endl;
    
    if (results.failed == 0) {
        std::cout << "\n" << GREEN << BOLD << "✓ All tests passed!" << RESET << std::endl;
    } else {
        std::cout << "\n" << RED << BOLD << "✗ Some tests failed!" << RESET << std::endl;
    }
    
    std::cout << BOLD << "=" << std::string(60, '=') << RESET << "\n" << std::endl;
}

bool Reporter::generateHTML(const TestResults& results, 
                            const std::string& suite_name,
                            const std::string& output_path) {
    std::ofstream file(output_path);
    if (!file.is_open()) {
        std::cerr << "Could not create HTML report at: " << output_path << std::endl;
        return false;
    }
    
    file << "<!DOCTYPE html>\n";
    file << "<html lang=\"en\">\n";
    file << "<head>\n";
    file << "  <meta charset=\"UTF-8\">\n";
    file << "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    file << "  <title>Test Report - " << suite_name << "</title>\n";
    file << "  <style>\n";
    file << "    body { font-family: Arial, sans-serif; margin: 20px; background: #f5f5f5; }\n";
    file << "    .container { max-width: 1000px; margin: 0 auto; background: white; padding: 30px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }\n";
    file << "    h1 { color: #333; border-bottom: 3px solid #4CAF50; padding-bottom: 10px; }\n";
    file << "    .summary { display: flex; gap: 20px; margin: 20px 0; }\n";
    file << "    .stat { flex: 1; padding: 15px; border-radius: 4px; text-align: center; }\n";
    file << "    .stat h3 { margin: 0; font-size: 2em; }\n";
    file << "    .stat p { margin: 5px 0 0 0; color: #666; }\n";
    file << "    .passed { background: #e8f5e9; color: #2e7d32; }\n";
    file << "    .failed { background: #ffebee; color: #c62828; }\n";
    file << "    .skipped { background: #fff3e0; color: #ef6c00; }\n";
    file << "    .total { background: #e3f2fd; color: #1565c0; }\n";
    file << "    table { width: 100%; border-collapse: collapse; margin-top: 20px; }\n";
    file << "    th, td { padding: 12px; text-align: left; border-bottom: 1px solid #ddd; }\n";
    file << "    th { background: #f5f5f5; font-weight: bold; }\n";
    file << "    .status { font-weight: bold; }\n";
    file << "    .status.pass { color: #2e7d32; }\n";
    file << "    .status.fail { color: #c62828; }\n";
    file << "    .status.skip { color: #ef6c00; }\n";
    file << "    .error { color: #c62828; font-size: 0.9em; margin-top: 5px; }\n";
    file << "  </style>\n";
    file << "</head>\n";
    file << "<body>\n";
    file << "  <div class=\"container\">\n";
    file << "    <h1>🧪 Test Report: " << suite_name << "</h1>\n";
    file << "    <div class=\"summary\">\n";
    file << "      <div class=\"stat total\"><h3>" << results.total << "</h3><p>Total Tests</p></div>\n";
    file << "      <div class=\"stat passed\"><h3>" << results.passed << "</h3><p>Passed</p></div>\n";
    file << "      <div class=\"stat failed\"><h3>" << results.failed << "</h3><p>Failed</p></div>\n";
    file << "      <div class=\"stat skipped\"><h3>" << results.skipped << "</h3><p>Skipped</p></div>\n";
    file << "    </div>\n";
    file << "    <p><strong>Total Time:</strong> " << results.total_time_ms << "ms</p>\n";
    file << "    <table>\n";
    file << "      <thead>\n";
    file << "        <tr><th>Test Name</th><th>Status</th><th>Time (ms)</th><th>Details</th></tr>\n";
    file << "      </thead>\n";
    file << "      <tbody>\n";
    
    for (const auto& test : results.test_cases) {
        std::string status_class;
        std::string status_text;
        
        switch (test.getStatus()) {
            case TestStatus::PASSED:
                status_class = "pass";
                status_text = "✓ PASSED";
                break;
            case TestStatus::FAILED:
                status_class = "fail";
                status_text = "✗ FAILED";
                break;
            case TestStatus::SKIPPED:
                status_class = "skip";
                status_text = "⊘ SKIPPED";
                break;
        }
        
        file << "        <tr>\n";
        file << "          <td>" << test.getName() << "</td>\n";
        file << "          <td class=\"status " << status_class << "\">" << status_text << "</td>\n";
        file << "          <td>" << test.getExecutionTime() << "</td>\n";
        file << "          <td>";
        
        if (!test.getErrorMessage().empty()) {
            file << "<div class=\"error\">" << test.getErrorMessage() << "</div>";
        }
        
        file << "</td>\n";
        file << "        </tr>\n";
    }
    
    file << "      </tbody>\n";
    file << "    </table>\n";
    file << "  </div>\n";
    file << "</body>\n";
    file << "</html>\n";
    
    file.close();
    std::cout << "\n📄 HTML report generated: " << output_path << std::endl;
    return true;
}

std::string Reporter::getStatusColor(TestStatus status) {
    switch (status) {
        case TestStatus::PASSED: return GREEN;
        case TestStatus::FAILED: return RED;
        case TestStatus::SKIPPED: return YELLOW;
        default: return RESET;
    }
}

std::string Reporter::getStatusSymbol(TestStatus status) {
    switch (status) {
        case TestStatus::PASSED: return "✓";
        case TestStatus::FAILED: return "✗";
        case TestStatus::SKIPPED: return "⊘";
        default: return "?";
    }
}
