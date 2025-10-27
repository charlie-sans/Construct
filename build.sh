#!/bin/bash
# Build and compile the Construct compiler

set -e

echo "=== Building Construct Compiler ==="
echo

# Setup build directory if not exists
if [ ! -d "build" ]; then
    echo "Setting up build directory..."
    meson setup build
    echo
fi

# Build the project
echo "Building with Meson..."
meson compile -C build
echo

echo "Build complete! Executable at: build/construct"
echo

# Test compilation
if [ -f "build/construct" ]; then
    echo "=== Testing Compiler ==="
    echo
    
    if [ -f "examples/hello.ct" ]; then
        echo "Compiling examples/hello.ct..."
        ./build/construct examples/hello.ct -o /tmp/hello.ll -v
        echo
        echo "Output LLVM IR saved to /tmp/hello.ll"
        echo
        echo "LLVM IR:"
        head -20 /tmp/hello.ll
        echo "... (see /tmp/hello.ll for full output)"
    fi
fi
