#include "testscope/assertions.hpp"
#include <algorithm>

using namespace testscope;

bool Assert::equals(const std::string& expected, const std::string& actual) {
    return expected == actual;
}

bool Assert::equals(int expected, int actual) {
    return expected == actual;
}

bool Assert::contains(const std::string& haystack, const std::string& needle) {
    return haystack.find(needle) != std::string::npos;
}

bool Assert::statusCode(int expected, int actual) {
    return expected == actual;
}

bool Assert::inRange(int value, int min, int max) {
    return value >= min && value <= max;
}
