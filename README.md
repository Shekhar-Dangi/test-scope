# TestScope - Automated Test Framework

![CI Build](https://github.com/Shekhar-Dangi/test-scope/workflows/CI%20Build%20and%20Test/badge.svg)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/license-MIT-green.svg)](LICENSE)

A lightweight, modern C++ testing framework for validating APIs and CLI tools.

## Features

- JSON-based test definitions
- HTTP/HTTPS API testing with SSL support
- Colorful CLI output with execution times
- Modern HTML report generation
- Automatic redirect following
- GitHub Actions CI/CD integration
- Fast and lightweight

## Building

```bash
mkdir build && cd build
cmake ..
make
```

## Running Tests

```bash
./testscope examples/api_tests.json
```

## Example Test Definition

```json
{
  "test_suite": {
    "name": "API Health Check",
    "tests": [
      {
        "name": "GET user endpoint",
        "type": "http",
        "method": "GET",
        "url": "https://jsonplaceholder.typicode.com/users/1",
        "expect": {
          "status": 200,
          "contains": "Leanne Graham"
        }
      }
    ]
  }
}
```

## Project Status

Phase 1 Complete: Core HTTP testing, JSON parsing, CLI & HTML reporting

## Roadmap

- [x] JSON-based test definitions
- [x] HTTP/HTTPS API testing (GET, POST)
- [x] SSL/TLS support with OpenSSL
- [x] Automatic redirect handling
- [x] Colorful CLI output with execution times
- [x] Modern HTML report generation
- [x] GitHub Actions CI/CD
- [ ] CLI command testing
- [ ] Performance/load testing mode
- [ ] Regression testing
- [ ] JUnit XML output

## Architecture

```
TestScope/
├── Core Components
│   ├── TestRunner    - Orchestrates test execution
│   ├── TestCase      - Individual test representation
│   └── JsonParser    - Parses test definitions
├── Executors
│   ├── HttpClient    - HTTP/HTTPS request handler with SSL
│   └── Assertions    - Validation logic
└── Reporters
    ├── CLI Reporter  - Colored terminal output
    └── HTML Reporter - Minimal modern design
```
