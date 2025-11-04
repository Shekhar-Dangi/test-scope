# TestScope - Automated Test Framework

A lightweight, modern C++ testing framework for validating APIs and CLI tools.

## Features

- 🚀 JSON-based test definitions
- 🌐 HTTP API testing
- 📊 Colorful CLI output
- 📝 HTML report generation
- ⚡ Fast and lightweight

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

🏗️ Currently in development - Phase 1 (MVP)
