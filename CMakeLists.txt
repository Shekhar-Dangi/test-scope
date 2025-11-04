cmake_minimum_required(VERSION 3.15)
project(TestScope VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

include(FetchContent)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

FetchContent_Declare(
    httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
    GIT_TAG v0.14.3
)
FetchContent_MakeAvailable(httplib)

include_directories(${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    src/test_case.cpp
    src/test_runner.cpp
    src/assertions.cpp
    src/http_client.cpp
    src/json_parser.cpp
    src/reporter.cpp
)

add_executable(testscope main.cpp ${SOURCES})

target_link_libraries(testscope PRIVATE nlohmann_json::nlohmann_json httplib::httplib)

enable_testing()
