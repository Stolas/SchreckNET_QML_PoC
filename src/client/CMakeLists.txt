cmake_minimum_required(VERSION 3.16)

project(SchreckNET_QML_PoC VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Detect if we're building for WebAssembly
if(EMSCRIPTEN)
    message(STATUS "Building for WebAssembly with Emscripten")
    
    # Qt6 WebAssembly specific settings
    set(CMAKE_PREFIX_PATH "$ENV{QT_WASM_PATH}")
    
    # Emscripten specific flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s USE_WEBGL2=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s WASM=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ALLOW_MEMORY_GROWTH=1")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s NO_EXIT_RUNTIME=0")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASSERTIONS=1")
    
    # Find Qt6 for WebAssembly
    find_package(Qt6 REQUIRED COMPONENTS Core Quick)
    
else()
    # Windows/Desktop build settings
    set(CMAKE_PREFIX_PATH "C:/Dev/Libs/Qt/6.9.2/msvc2022_arm64")
    find_package(Qt6 REQUIRED COMPONENTS Quick Widgets)
endif()

qt_standard_project_setup()

# Set Qt policies to suppress warnings
qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_executable(appSchreckNET_QML_PoC
    main.cc
    login_controller.h
    login_controller.cc
    game_lobby_controller.h
    game_lobby_controller.cc
    game_controller.h
    game_controller.cc
)

qt_add_qml_module(appSchreckNET_QML_PoC
    URI SchreckNET_QML_PoC
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/views/LoginScreen.qml
        qml/views/GameLobby.qml
        qml/views/GameListItem.qml
        qml/views/GameTabView.qml
        qml/views/GameView.qml
        qml/components/CardTypeSection.qml
        qml/components/GameListItem.qml
        qml/components/PlayerListItem.qml
)

# Platform-specific target properties
if(EMSCRIPTEN)
    set_target_properties(appSchreckNET_QML_PoC PROPERTIES
        SUFFIX ".html"
    )
    
    # Link Qt6 libraries for WebAssembly
    target_link_libraries(appSchreckNET_QML_PoC
        PRIVATE Qt6::Core Qt6::Quick
    )
    
    # Set additional Emscripten linker flags
    set_target_properties(appSchreckNET_QML_PoC PROPERTIES
        LINK_FLAGS "-s WASM=1 -s ALLOW_MEMORY_GROWTH=1 -s USE_WEBGL2=1 --bind"
    )
    
else()
    # Desktop properties
    set_target_properties(appSchreckNET_QML_PoC PROPERTIES
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
        MACOSX_BUNDLE TRUE
        WIN32_EXECUTABLE TRUE
    )
    
    target_link_libraries(appSchreckNET_QML_PoC
        PRIVATE Qt6::Quick Qt6::Widgets
    )
endif()

include(GNUInstallDirs)
install(TARGETS appSchreckNET_QML_PoC
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)