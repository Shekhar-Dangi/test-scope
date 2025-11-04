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
    file << "    * { margin: 0; padding: 0; box-sizing: border-box; }\n";
    file << "    body { \n";
    file << "      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', 'Roboto', sans-serif;\n";
    file << "      background: #0a0a0a;\n";
    file << "      color: #e4e4e7;\n";
    file << "      line-height: 1.6;\n";
    file << "      padding: 2rem;\n";
    file << "    }\n";
    file << "    .container { max-width: 1200px; margin: 0 auto; }\n";
    file << "    header { margin-bottom: 3rem; }\n";
    file << "    h1 { \n";
    file << "      font-size: 2rem;\n";
    file << "      font-weight: 600;\n";
    file << "      color: #fafafa;\n";
    file << "      margin-bottom: 0.5rem;\n";
    file << "    }\n";
    file << "    .subtitle { color: #a1a1aa; font-size: 0.875rem; }\n";
    file << "    .stats { \n";
    file << "      display: grid;\n";
    file << "      grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));\n";
    file << "      gap: 1rem;\n";
    file << "      margin-bottom: 2rem;\n";
    file << "    }\n";
    file << "    .stat-card { \n";
    file << "      background: #18181b;\n";
    file << "      border: 1px solid #27272a;\n";
    file << "      border-radius: 8px;\n";
    file << "      padding: 1.25rem;\n";
    file << "    }\n";
    file << "    .stat-value { \n";
    file << "      font-size: 2rem;\n";
    file << "      font-weight: 700;\n";
    file << "      margin-bottom: 0.25rem;\n";
    file << "    }\n";
    file << "    .stat-label { \n";
    file << "      font-size: 0.875rem;\n";
    file << "      color: #a1a1aa;\n";
    file << "      text-transform: uppercase;\n";
    file << "      letter-spacing: 0.05em;\n";
    file << "    }\n";
    file << "    .stat-card.total .stat-value { color: #60a5fa; }\n";
    file << "    .stat-card.passed .stat-value { color: #22c55e; }\n";
    file << "    .stat-card.failed .stat-value { color: #ef4444; }\n";
    file << "    .stat-card.time .stat-value { color: #a78bfa; }\n";
    file << "    .tests { \n";
    file << "      background: #18181b;\n";
    file << "      border: 1px solid #27272a;\n";
    file << "      border-radius: 8px;\n";
    file << "      overflow: hidden;\n";
    file << "    }\n";
    file << "    .test-item { \n";
    file << "      padding: 1rem 1.5rem;\n";
    file << "      border-bottom: 1px solid #27272a;\n";
    file << "      display: flex;\n";
    file << "      justify-content: space-between;\n";
    file << "      align-items: center;\n";
    file << "      transition: background 0.2s;\n";
    file << "    }\n";
    file << "    .test-item:last-child { border-bottom: none; }\n";
    file << "    .test-item:hover { background: #27272a; }\n";
    file << "    .test-info { flex: 1; }\n";
    file << "    .test-name { \n";
    file << "      font-weight: 500;\n";
    file << "      margin-bottom: 0.25rem;\n";
    file << "    }\n";
    file << "    .test-error { \n";
    file << "      font-size: 0.875rem;\n";
    file << "      color: #fca5a5;\n";
    file << "      margin-top: 0.5rem;\n";
    file << "      font-family: 'Courier New', monospace;\n";
    file << "    }\n";
    file << "    .test-meta { \n";
    file << "      display: flex;\n";
    file << "      gap: 1rem;\n";
    file << "      align-items: center;\n";
    file << "    }\n";
    file << "    .test-time { \n";
    file << "      font-size: 0.875rem;\n";
    file << "      color: #a1a1aa;\n";
    file << "      font-variant-numeric: tabular-nums;\n";
    file << "    }\n";
    file << "    .status { \n";
    file << "      padding: 0.25rem 0.75rem;\n";
    file << "      border-radius: 4px;\n";
    file << "      font-size: 0.75rem;\n";
    file << "      font-weight: 600;\n";
    file << "      text-transform: uppercase;\n";
    file << "      letter-spacing: 0.05em;\n";
    file << "    }\n";
    file << "    .status.pass { background: #14532d; color: #86efac; }\n";
    file << "    .status.fail { background: #450a0a; color: #fca5a5; }\n";
    file << "    .status.skip { background: #422006; color: #fcd34d; }\n";
    file << "  </style>\n";
    file << "</head>\n";
    file << "<body>\n";
    file << "  <div class=\"container\">\n";
    file << "    <header>\n";
    file << "      <h1>" << suite_name << "</h1>\n";
    file << "      <p class=\"subtitle\">Test Report</p>\n";
    file << "    </header>\n";
    
    file << "    <div class=\"stats\">\n";
    file << "      <div class=\"stat-card total\">\n";
    file << "        <div class=\"stat-value\">" << results.total << "</div>\n";
    file << "        <div class=\"stat-label\">Total Tests</div>\n";
    file << "      </div>\n";
    file << "      <div class=\"stat-card passed\">\n";
    file << "        <div class=\"stat-value\">" << results.passed << "</div>\n";
    file << "        <div class=\"stat-label\">Passed</div>\n";
    file << "      </div>\n";
    file << "      <div class=\"stat-card failed\">\n";
    file << "        <div class=\"stat-value\">" << results.failed << "</div>\n";
    file << "        <div class=\"stat-label\">Failed</div>\n";
    file << "      </div>\n";
    file << "      <div class=\"stat-card time\">\n";
    file << "        <div class=\"stat-value\">" << results.total_time_ms << "</div>\n";
    file << "        <div class=\"stat-label\">Time (ms)</div>\n";
    file << "      </div>\n";
    file << "    </div>\n";
    
    file << "    <div class=\"tests\">\n";
    
    for (const auto& test : results.test_cases) {
        std::string status_class;
        std::string status_text;
        
        switch (test.getStatus()) {
            case TestStatus::PASSED:
                status_class = "pass";
                status_text = "PASS";
                break;
            case TestStatus::FAILED:
                status_class = "fail";
                status_text = "FAIL";
                break;
            case TestStatus::SKIPPED:
                status_class = "skip";
                status_text = "SKIP";
                break;
        }
        
        file << "      <div class=\"test-item\">\n";
        file << "        <div class=\"test-info\">\n";
        file << "          <div class=\"test-name\">" << test.getName() << "</div>\n";
        
        if (!test.getErrorMessage().empty()) {
            file << "          <div class=\"test-error\">" << test.getErrorMessage() << "</div>\n";
        }
        
        file << "        </div>\n";
        file << "        <div class=\"test-meta\">\n";
        file << "          <span class=\"test-time\">" << test.getExecutionTime() << "ms</span>\n";
        file << "          <span class=\"status " << status_class << "\">" << status_text << "</span>\n";
        file << "        </div>\n";
        file << "      </div>\n";
    }
    
    file << "    </div>\n";
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
