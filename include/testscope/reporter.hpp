#pragma once

#include <string>
#include "test_runner.hpp"

namespace testscope {

class Reporter {
public:
    static void printToConsole(const TestResults& results, const std::string& suite_name);
    static bool generateHTML(const TestResults& results, 
                            const std::string& suite_name,
                            const std::string& output_path);

private:
    static const std::string RESET;
    static const std::string RED;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string BLUE;
    static const std::string BOLD;
    
    static std::string getStatusColor(TestStatus status);
    static std::string getStatusSymbol(TestStatus status);
};

}
