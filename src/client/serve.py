#!/usr/bin/env python3
"""
Simple HTTP server for serving Qt WebAssembly applications.
This server sets the necessary headers for SharedArrayBuffer support.

Usage:
    python3 serve.py [port]

Default port is 8080.
"""

import http.server
import socketserver
import sys
import os

class QtWasmHandler(http.server.SimpleHTTPRequestHandler):
    def end_headers(self):
        # Enable SharedArrayBuffer for Qt multithreaded WebAssembly
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        
        # Set proper MIME types
        if self.path.endswith('.wasm'):
            self.send_header('Content-Type', 'application/wasm')
        elif self.path.endswith('.js'):
            self.send_header('Content-Type', 'application/javascript')
        elif self.path.endswith('.html'):
            self.send_header('Content-Type', 'text/html')
        
        # Enable compression
        if self.path.endswith(('.js', '.wasm', '.html')):
            self.send_header('Content-Encoding', 'gzip')
        
        super().end_headers()

    def log_message(self, format, *args):
        # Custom logging
        print(f"[{self.log_date_time_string()}] {format % args}")

def main():
    port = 8080
    
    if len(sys.argv) > 1:
        try:
            port = int(sys.argv[1])
        except ValueError:
            print(f"Invalid port: {sys.argv[1]}")
            sys.exit(1)
    
    os.chdir(os.path.dirname(os.path.abspath(__file__)))
    
    with socketserver.TCPServer(("", port), QtWasmHandler) as httpd:
        print(f"Serving Qt WebAssembly application at http://localhost:{port}/")
        print(f"Open http://localhost:{port}/appSchreckNET_QML_PoC.html in your browser")
        print("Press Ctrl+C to stop the server")
        
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\nServer stopped.")

if __name__ == "__main__":
    main()