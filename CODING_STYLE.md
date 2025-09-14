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
│   │   ├── controllers/        # Qt C++ controllers (QML exposed)
│   │   ├── models/             # Qt C++ models (QML exposed)
│   │   ├── qml/                # QML user interface files
│   │   │   ├── components/     # Reusable QML components
│   │   │   └── views/          # Main application views
│   │   └── game/               # Game logic classes
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
- **QML Structure**: QML files are organized into `components/` (reusable) and `views/` (screens)

### File Naming Conventions

- **Directories**: Use lowercase with underscores (`networking`, `card_database`)
- **Source Files**: Use snake_case for both headers (`.h`) and implementation (`.cc`)
- **QML Files**: Use PascalCase for QML files (`.qml`) (`LoginScreen.qml`, `GameListItem.qml`)
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

```

## QML Style Guidelines

### General Principles
- Follow Qt's QML coding conventions
- Maintain consistency with Qt Quick Controls styling
- Use declarative approach - describe what the UI should look like, not how to build it
- Separate logic from presentation where possible

### File Organization
- **Views**: Complete screens or major UI sections (`LoginScreen.qml`, `GameLobby.qml`)
- **Components**: Reusable UI elements (`GameListItem.qml`, `PlayerListItem.qml`)
- **One component per file**: Each QML file should contain exactly one root component
- **Logical grouping**: Related components can be in subdirectories

### Formatting
- **Indentation**: 4 spaces (no tabs), consistent with C++
- **Line Length**: Maximum 120 characters
- **Property Order**: Follow standard Qt ordering (see below)
- **Spacing**: One blank line between major sections, no blank lines between related properties

### Property Ordering
Follow this order for QML properties and sections:
1. `id`
2. Property declarations (`property`)
3. Signal declarations (`signal`)
4. Size and positioning (`width`, `height`, `anchors`)
5. Visual properties (`color`, `border`, `radius`, etc.)
6. Behavioral properties (`enabled`, `visible`, `focus`)
7. Event handlers (`onClicked`, `onTextChanged`)
8. Child elements
9. Component definitions (`Component`)

Example:
```qml
Rectangle {
    id: root
    
    property string title: ""
    property bool isSelected: false
    signal clicked()
    
    width: 200
    height: 50
    anchors.margins: 10
    
    color: isSelected ? "#3498db" : "#ecf0f1"
    border.color: "#bdc3c7"
    border.width: 1
    radius: 4
    
    enabled: true
    visible: true
    
    onClicked: root.clicked()
    
    Text {
        id: titleText
        anchors.centerIn: parent
        text: root.title
        font.pixelSize: 14
    }
}
```

### Naming Conventions
- **File Names**: PascalCase (`LoginScreen.qml`, `GameListItem.qml`)
- **Element IDs**: camelCase (`titleText`, `submitButton`, `gameListView`)
- **Property Names**: camelCase (`playerName`, `isConnected`, `maxPlayers`)
- **Signal Names**: camelCase with descriptive verbs (`clicked`, `textChanged`, `gameJoined`)
- **Function Names**: camelCase (`openGame`, `validateInput`, `resetForm`)

### Import Organization
Order imports logically:
1. QtQuick modules
2. QtQuick.Controls
3. QtQuick.Layouts
4. Custom C++ modules (e.g., `SchreckNET_QML_PoC`)
5. Relative imports (components, views)

```qml
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import SchreckNET_QML_PoC
import "../components"
```

### Component Structure
- **Root Element**: Choose the most appropriate root (Item, Rectangle, Control, etc.)
- **Public Interface**: Define properties and signals that external components need
- **Internal Logic**: Keep complex logic in separate functions or move to C++ controllers
- **Child Components**: Use clear hierarchy and meaningful IDs

### Layout Guidelines
- **Responsive Design**: Use Layout properties and anchors for flexible sizing
- **Minimum Sizes**: Always specify `Layout.minimumWidth` and `Layout.minimumHeight` for critical components
- **Spacing**: Use consistent spacing values throughout the application
- **Margins**: Apply consistent margins using `anchors.margins` or Layout margins

```qml
ColumnLayout {
    Layout.fillWidth: true
    Layout.fillHeight: true
    Layout.minimumWidth: 300
    spacing: 10
    
    Rectangle {
        Layout.fillWidth: true
        Layout.minimumHeight: 50
        // ...
    }
}
```

### Signal and Slot Usage
- **Custom Signals**: Define clear, descriptive signals for component communication
- **Event Handlers**: Use `onXXX` handlers for responding to events
- **Function Calls**: Prefer direct function calls over complex signal chains when appropriate

```qml
// Signal definition
signal gameJoined(string gameName)

// Signal emission
onClicked: root.gameJoined(gameData.name)

// Signal handling
onGameJoined: function(gameName) {
    // Handle the event
}
```

### State Management
- **Properties**: Use properties for component state that needs to be accessible externally
- **Internal State**: Use internal properties for component-specific state
- **Controllers**: Delegate complex state management to C++ controllers

### Performance Considerations
- **Lazy Loading**: Use `Loader` for components that aren't immediately needed
- **List Optimization**: Use appropriate models and delegates for lists
- **Image Loading**: Use appropriate image formats and loading strategies
- **Animations**: Use efficient animations and avoid overuse

### Comments and Documentation
- **File Headers**: Include brief description of component purpose
- **Section Comments**: Use `//` to separate major sections
- **Complex Logic**: Comment any non-obvious QML logic
- **Property Documentation**: Document custom properties and their expected values

```qml
// Main game lobby view - displays available games and chat
import QtQuick
import QtQuick.Controls

Item {
    id: root
    
    // Public interface
    property string playerName: ""
    signal backToLogin()
    
    // Main content area
    RowLayout {
        // ... implementation
    }
    
    // Game creation dialog
    Dialog {
        // ... implementation
    }
}
```

### Integration with C++
- **Controllers**: Use C++ controllers for business logic, QML for presentation
- **Models**: Implement data models in C++ using `QAbstractItemModel`
- **Properties**: Expose C++ properties using `Q_PROPERTY` macro
- **Type Registration**: Register C++ types using `QML_ELEMENT` or manual registration

### Error Handling
- **Property Validation**: Validate property values and provide sensible defaults
- **Null Checks**: Check for null/undefined values before accessing properties
- **Graceful Degradation**: Ensure UI remains functional even with missing data

### Example Complete Component:
```qml
// GameListItem.qml - Displays a single game in the lobby list
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: root
    height: 85
    
    // Public interface
    property var gameData
    signal joinClicked()
    signal spectateClicked()
    
    // Visual container
    Rectangle {
        anchors.fill: parent
        border.color: "#e0e0e0"
        border.width: 1
        color: mouseArea.containsMouse ? "#f5f5f5" : "white"
        
        MouseArea {
            id: mouseArea
            anchors.fill: parent
            hoverEnabled: true
        }
        
        // Main content layout
        RowLayout {
            anchors.fill: parent
            anchors.margins: 12
            spacing: 15
            
            // Game information section
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 6
                
                Text {
                    text: gameData.name
                    font.bold: true
                    font.pixelSize: 14
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                }
                
                Text {
                    text: "Players: " + gameData.currentPlayers + "/" + gameData.maxPlayers
                    font.pixelSize: 12
                    color: gameData.currentPlayers >= gameData.maxPlayers ? "#f44336" : "#4caf50"
                }
            }
            
            // Action buttons section
            ColumnLayout {
                spacing: 6
                
                Button {
                    text: "Join"
                    enabled: gameData.currentPlayers < gameData.maxPlayers
                    onClicked: root.joinClicked()
                }
                
                Button {
                    text: "Spectate"
                    onClicked: root.spectateClicked()
                }
            }
        }
    }
}
