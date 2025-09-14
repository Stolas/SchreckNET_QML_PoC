cmake_minimum_required(VERSION 3.16)

function(deploy_qt_dependencies target_name)
    if(NOT WIN32)
        return()
    endif()

    # Validate that the target exists
    if(NOT TARGET ${target_name})
        message(FATAL_ERROR "Target '${target_name}' does not exist")
        return()
    endif()

    # Find windeployqt executable
    get_target_property(_qt_core_location Qt6::Core LOCATION)
    if(NOT _qt_core_location)
        message(WARNING "Qt6::Core location not found, trying IMPORTED_LOCATION")
        get_target_property(_qt_core_location Qt6::Core IMPORTED_LOCATION)
    endif()

    if(NOT _qt_core_location)
        message(WARNING "Could not determine Qt6::Core location, attempting to find windeployqt in PATH")
        find_program(WINDEPLOYQT_EXECUTABLE windeployqt)
    else()
        get_filename_component(_qt_bin_dir "${_qt_core_location}" DIRECTORY)
        find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
    endif()

    if(NOT WINDEPLOYQT_EXECUTABLE)
        message(FATAL_ERROR "windeployqt not found!")
        return()
    endif()

    # Add custom command to deploy Qt dependencies
    add_custom_command(TARGET ${target_name} POST_BUILD
        COMMAND "${WINDEPLOYQT_EXECUTABLE}" "$<TARGET_FILE:${target_name}>"
        COMMENT "Deploying Qt dependencies for ${target_name}"
        VERBATIM
    )

    message(STATUS "Qt deployment configured for target: ${target_name}")
endfunction()

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
    find_package(Qt6 REQUIRED COMPONENTS Core Quick Widgets)
endif()

qt_standard_project_setup()

# Set Qt policies to suppress warnings
qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)

qt_add_executable(appSchreckNET_QML_PoC
    # Main entry point
    main.cc
    # Controllers
    controllers/login_controller.h
    controllers/login_controller.cc
    controllers/game_lobby_controller.h
    controllers/game_lobby_controller.cc
    controllers/game_controller.h
    controllers/game_controller.cc
    # Models
    models/card.h
    models/card.cc
    models/deck_model.h
    models/deck_model.cc
    models/game_players_model.h
    models/game_players_model.cc
    # Game entities
    game/game_player.h
)

# Add include directories for the new structure
target_include_directories(appSchreckNET_QML_PoC PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/controllers
    ${CMAKE_CURRENT_SOURCE_DIR}/models
    ${CMAKE_CURRENT_SOURCE_DIR}/game
)

qt_add_qml_module(appSchreckNET_QML_PoC
    URI SchreckNET_QML_PoC
    VERSION 1.0
    QML_FILES
        qml/Main.qml
        qml/views/LoginScreen.qml
        qml/views/GameLobby.qml
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
    
    target_link_libraries(appSchreckNET_QML_PoC PRIVATE Qt6::Core Qt6::Quick Qt6::Widgets)
    deploy_qt_dependencies(appSchreckNET_QML_PoC)
endif()

include(GNUInstallDirs)
install(TARGETS appSchreckNET_QML_PoC
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
