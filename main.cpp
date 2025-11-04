#include <iostream>
#include <string>
#include "testscope/test_runner.hpp"
#include "testscope/reporter.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <test_file.json> [options]" << std::endl;
        std::cerr << "Example: " << argv[0] << " examples/api_tests.json" << std::endl;
        return 1;
    }
    
    std::string test_file = argv[1];
    std::string html_output = "report.html";
    
    for (int i = 2; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--output" && i + 1 < argc) {
            html_output = argv[++i];
        } else if (arg == "--help" || arg == "-h") {
            std::cout << "TestScope - Automated Test Framework\n" << std::endl;
            std::cout << "Usage: " << argv[0] << " <test_file.json> [options]\n" << std::endl;
            std::cout << "Options:" << std::endl;
            std::cout << "  --output <file>    Specify HTML report output path (default: report.html)" << std::endl;
            std::cout << "  --help, -h         Show this help message" << std::endl;
            return 0;
        }
    }
    
    std::cout << "\n╔════════════════════════════════════════════════════════════╗" << std::endl;
    std::cout << "║           TestScope - Automated Test Framework            ║" << std::endl;
    std::cout << "╚════════════════════════════════════════════════════════════╝\n" << std::endl;
    
    testscope::TestRunner runner;
    
    if (!runner.loadFromJson(test_file)) {
        std::cerr << "Failed to load tests from: " << test_file << std::endl;
        return 1;
    }
    
    testscope::TestResults results = runner.run();
    testscope::Reporter::printToConsole(results, runner.getSuiteName());
    testscope::Reporter::generateHTML(results, runner.getSuiteName(), html_output);
    
    return (results.failed == 0) ? 0 : 1;
}
