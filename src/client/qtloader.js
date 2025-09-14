// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

/**
 * Loads the instance of a WASM module for Qt applications.
 *
 * @param config Configuration object with Qt-specific settings
 * @return Promise<instance: EmscriptenModule>
 */
async function qtLoad(config) {
    if (typeof config !== 'object') {
        throw new Error('qtLoad: config is required and must be an object');
    }
    
    if (typeof config.qt !== 'object') {
        throw new Error('qtLoad: config.qt is required and must be an object');
    }
    
    if (typeof config.qt.entryFunction !== 'function') {
        throw new Error('qtLoad: config.qt.entryFunction is required and must be a function');
    }

    // Set default values
    config.qt.qtdir ??= 'qt';
    config.qt.preload ??= [];
    
    // Transfer Qt-specific config to main config
    config.qtContainerElements = config.qt.containerElements;
    delete config.qt.containerElements;
    config.qtFontDpi = config.qt.fontDpi;
    delete config.qt.fontDpi;

    // Store original values
    const originalNoInitialRun = config.noInitialRun;
    const originalArguments = config.arguments;
    
    // Don't call main automatically - we control startup
    config.noInitialRun = true;

    // Error handling for failed loads
    let circuitBreakerReject;
    const circuitBreaker = new Promise((_, reject) => { 
        circuitBreakerReject = reject; 
    });

    // Handle WebAssembly module instantiation
    if (config.qt.module) {
        config.instantiateWasm = async (imports, successCallback) => {
            try {
                const module = await config.qt.module;
                successCallback(
                    await WebAssembly.instantiate(module, imports), 
                    module
                );
            } catch (e) {
                circuitBreakerReject(e);
            }
        };
    }

    // Handle preloading of files
    const preloadFiles = async () => {
        if (!config.qt.preload.length) return [];
        
        const fetchFile = async (path) => {
            const response = await fetch(path);
            if (!response.ok) {
                throw new Error(`Could not fetch preload file: ${path}`);
            }
            return response.json();
        };
        
        try {
            const preloadData = await Promise.all(
                config.qt.preload.map(fetchFile)
            );
            return preloadData.flat();
        } catch (error) {
            console.warn('Failed to preload files:', error);
            return [];
        }
    };

    const filesToPreload = await preloadFiles();

    // Pre-run setup
    const qtPreRun = (instance) => {
        // Set up environment variables
        if (config.qt.environment && instance.ENV) {
            for (const [name, value] of Object.entries(config.qt.environment)) {
                instance.ENV[name] = value;
            }
        }

        // Handle file preloading
        if (filesToPreload.length && instance.FS) {
            const makeDirs = (FS, filePath) => {
                const parts = filePath.split("/");
                let path = "/";
                for (let i = 0; i < parts.length - 1; ++i) {
                    const part = parts[i];
                    if (part === "") continue;
                    path += part + "/";
                    try {
                        FS.mkdir(path);
                    } catch (error) {
                        const EEXIST = 20;
                        if (error.errno !== EEXIST) throw error;
                    }
                }
            };

            const extractFilenameAndDir = (path) => {
                const parts = path.split('/');
                const filename = parts.pop();
                const dir = parts.join('/');
                return { filename, dir };
            };

            const preloadFile = (file) => {
                makeDirs(instance.FS, file.destination);
                const source = file.source.replace('$QTDIR', config.qt.qtdir);
                const { filename, dir } = extractFilenameAndDir(file.destination);
                instance.FS.createPreloadedFile(
                    dir, filename, source, true, true
                );
            };

            filesToPreload.forEach(preloadFile);
        }
    };

    // Set up pre-run callbacks
    if (!config.preRun) config.preRun = [];
    config.preRun.push(qtPreRun);

    // Handle runtime initialization
    const originalOnRuntimeInitialized = config.onRuntimeInitialized;
    config.onRuntimeInitialized = () => {
        originalOnRuntimeInitialized?.();
        config.qt.onLoaded?.();
    };

    // Handle file location for Qt libraries
    const originalLocateFile = config.locateFile;
    config.locateFile = filename => {
        const originalPath = originalLocateFile ? 
            originalLocateFile(filename) : filename;
        
        if (originalPath.startsWith('libQt6')) {
            return `${config.qt.qtdir}/lib/${originalPath}`;
        }
        return originalPath;
    };

    // Handle exit events
    let onExitCalled = false;
    
    const originalOnExit = config.onExit;
    config.onExit = code => {
        originalOnExit?.();
        
        if (!onExitCalled) {
            onExitCalled = true;
            config.qt.onExit?.({
                code,
                crashed: false
            });
        }
    };

    const originalOnAbort = config.onAbort;
    config.onAbort = text => {
        originalOnAbort?.();
        
        if (!onExitCalled) {
            onExitCalled = true;
            config.qt.onExit?.({
                text,
                crashed: true
            });
        }
    };

    // Load the application
    let instance;
    try {
        instance = await Promise.race([
            circuitBreaker, 
            config.qt.entryFunction(config)
        ]);

        // Call main if not disabled originally
        if (!originalNoInitialRun) {
            instance.callMain(originalArguments || []);
        }
        
    } catch (e) {
        // Handle "unwind" exception from app.exec()
        if (e === "unwind") {
            return instance;
        }

        if (!onExitCalled) {
            onExitCalled = true;
            config.qt.onExit?.({
                text: e.message || e.toString(),
                crashed: true
            });
        }
        throw e;
    }

    return instance;
}

// Export for different module systems
if (typeof module !== 'undefined' && module.exports) {
    module.exports = { qtLoad };
} else if (typeof window !== 'undefined') {
    window.qtLoad = qtLoad;
}