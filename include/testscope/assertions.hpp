#pragma once

#include <string>

namespace testscope {
namespace Assert {

bool equals(const std::string& expected, const std::string& actual);
bool equals(int expected, int actual);
bool contains(const std::string& haystack, const std::string& needle);
bool statusCode(int expected, int actual);
bool inRange(int value, int min, int max);

}
}
