# SchreckNet Server - Coding Style Guide

This document describes the coding style used in the SchreckNet Server project.

## Project Structure

The SchreckNet Server project follows a modular directory structure:

```
SchreckNet-Server/
├── .github/                    # GitHub workflows and actions
├── data/                       # Application data files
├── docs/                       # Documentation files
├── meta/                       # Build system metadata and helper scripts
├── pocs/                       # Proof of concepts and prototypes
├── resources/                  # Static resources (images, configs)
├── src/                        # Main source code
│   ├── client/                 # Client application
│   ├── pb/                     # Protocol Buffers definitions
│   ├── server/                 # Server application
│   ├── updater/                # Application updater utility
│   └── utility/                # Shared utility classes
└── tests/                      # Test suite
    ├── e2e/                    # End-to-end tests
    ├── integration/            # Integration tests
    └── unit/                   # Unit tests
```

### Directory Guidelines

- **Source Organization**: Each major component has its own directory under `src/`
- **Test Organization**: Tests mirror the source structure under `tests/`
- **Shared Code**: Common utilities go in `src/utility/`
- **Protocol Definitions**: All `.proto` files are in `src/pb/`
- **Documentation**: Project docs in `docs/`, design docs in `src/pb/design/`

### File Naming Conventions

- **Directories**: Use lowercase with underscores (`networking`, `card_database`)
- **Source Files**: Use snake_case for both headers (`.h`) and implementation (`.cc`)
- **CMake Files**: Always named `CMakeLists.txt`
- **Protocol Buffers**: Use snake_case with `.proto` extension

## C++ Style Guidelines

### General Principles
- Follow modern C++17 standards
- Use Qt conventions for Qt-related code
- Prioritize readability and maintainability
- Use meaningful variable and function names

### Formatting
- **Indentation**: 4 spaces (no tabs)
- **Line Length**: Maximum 120 characters
- **Pointer Alignment**: Left
- **Braces**: Microsoft/K&R style (opening brace on same line)
  ```cpp
  if (condition) {
      // code here
  }
  ```
  Exception: Class and function definitions have opening brace on new line:
  ```cpp
  class MyClass
  {
  public:
      void myFunction()
      {
          // implementation
      }
  };
  ```

### Naming Conventions
- **Classes**: PascalCase (`SessionManager`, `GameServer`)
- **Functions/Methods**: camelCase (`createSession`, `validateToken`)
- **Variables**: snake_case (`user_name`, `session_token`)
- **Constants**: UPPER_SNAKE_CASE (`MAX_SESSIONS`, `DEFAULT_TIMEOUT`)
- **Private members**: snake_case (`session_map`, `is_valid`)
- **Function parameters that might shadow members**: snake_case with trailing underscore (`name_`)
- **Member variables**: snake_case without prefix/suffix
- **Getters**: When a getter method would conflict with the variable name, use `getXXX()` format (`getName()` for `name` variable)

### Const Correctness
- Use `const` whenever possible for variables, parameters, and methods
- Mark member functions as `const` when they don't modify object state
- Use `const` references for function parameters when the parameter won't be modified
- Prefer `const` variables over mutable ones when the value doesn't change

### File Organization
- Header files use `.h` extension
- Implementation files use `.cc` extension
- One class per file when possible
- Don't use include guards, always use `#pragma once`

### Qt Specific
- Use Qt containers (`QString`, `QList`, `QMap`) for Qt-interfacing code
- Use Qt's signal/slot mechanism for event handling
- Follow Qt's object tree model for memory management
- Use `Q_OBJECT` macro for classes with signals/slots

### Comments
- Use `//` for single-line comments
- Use `/* */` for multi-line comments
- Include copyright header in all files
- To Do points must use the `// Todo; xxx` or `/* Todo; */` format.

### Example Code Structure:
```cpp
/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <QString>
#include <QObject>

class SessionManager : public QObject
{
    Q_OBJECT

public:
    explicit SessionManager(QObject* parent = nullptr);

    QString createSession(const QString& username);
    bool validateSession(const QString& token, QString& username);

private:
    QMap<QString, QString> session_map;

    QString generateToken() const;
};
