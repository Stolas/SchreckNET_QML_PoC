cmake_minimum_required(VERSION 3.16)

project(SchreckNET_QML_PoC VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Qt6 path for ARM64 system
set(CMAKE_PREFIX_PATH "C:/Dev/Libs/Qt/6.9.2/msvc2022_arm64")

find_package(Qt6 REQUIRED COMPONENTS Quick Widgets)

qt_standard_project_setup()

# Set Qt policy to suppress warning about QML resource prefix
qt_policy(SET QTP0001 NEW)

qt_add_executable(appSchreckNET_QML_PoC
    main.cpp
    LoginController.h
    LoginController.cpp
    GameLobbyController.h
    GameLobbyController.cpp
    GameController.h
    GameController.cpp
)

qt_add_qml_module(appSchreckNET_QML_PoC
    URI SchreckNET_QML_PoC
    VERSION 1.0
    QML_FILES
        Main.qml
        LoginScreen.qml
        GameLobby.qml
        GameListItem.qml
        GameTabView.qml
        GameView.qml
        CardTypeSection.qml
        PlayerListItem.qml
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appSchreckNET_QML_PoC PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appSchreckNET_QML_PoC
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appSchreckNET_QML_PoC
    PRIVATE Qt6::Quick Qt6::Widgets
)

include(GNUInstallDirs)
install(TARGETS appSchreckNET_QML_PoC
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
