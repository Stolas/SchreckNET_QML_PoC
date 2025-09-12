/*
 * Copyright (c) 2020-2025, Stolas <schrecknet@codeinject.org>
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <QGuiApplication>
#include <QQmlApplicationEngine>

// Include controller headers to ensure QML type registration
#include "controllers/login_controller.h"
#include "controllers/game_lobby_controller.h"
#include "controllers/game_controller.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/bind.h>
#endif

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    
    // Set application properties
    app.setApplicationName("SchreckNET QML PoC");
    app.setApplicationVersion("0.1");
    app.setOrganizationName("SchreckNET");

#ifdef __EMSCRIPTEN__
    // WebAssembly specific settings
    qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));
    qputenv("QT_VIRTUALKEYBOARD_STYLE", QByteArray("material"));
#endif

    QQmlApplicationEngine engine;
    
    // Handle object creation failures
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { 
#ifdef __EMSCRIPTEN__
            // In WebAssembly, we might want to show an error in the browser console
            EM_ASM({
                console.error("QML object creation failed");
            });
#endif
            QCoreApplication::exit(-1); 
        },
        Qt::QueuedConnection);
        
    // Load the main QML module
    engine.loadFromModule("SchreckNET_QML_PoC", "Main");

    return app.exec();
}
