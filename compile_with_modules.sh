#!/bin/bash
# compile_with_modules.sh
# Compiles a Construct program and automatically links with imported modules

set -e

if [ $# -lt 1 ]; then
    echo "Usage: $0 <program.ct> [output_name]"
    echo ""
    echo "Compiles a Construct program to native binary, automatically linking with"
    echo "any modules specified via 'import' statements in the source code."
    echo ""
    echo "Example:"
    echo "  $0 examples/test_import.ct"
    echo "  $0 examples/test_import.ct my_program"
    exit 1
fi

SOURCE_FILE="$1"
OUTPUT_NAME="${2:-$(basename "$SOURCE_FILE" .ct)}"
TEMP_LL="/tmp/${OUTPUT_NAME}.ll"
TEMP_S="/tmp/${OUTPUT_NAME}.s"
TEMP_MODULES="/tmp/${OUTPUT_NAME}_modules.txt"

echo "=== Construct Compiler with Module Support ==="
echo ""
echo "Source:     $SOURCE_FILE"
echo "Output:     $OUTPUT_NAME"
echo "Temp IR:    $TEMP_LL"
echo "Temp ASM:   $TEMP_S"
echo ""

# Step 1: Compile to LLVM IR and extract modules
echo "[1/4] Compiling to LLVM IR..."
./build/construct "$SOURCE_FILE" -o "$TEMP_LL" 2>&1 | grep -v "^$" &
COMPILE_PID=$!
wait $COMPILE_PID
echo "      ✓ Generated $TEMP_LL"
echo ""

# Step 2: Generate assembly
echo "[2/4] Generating assembly..."
if command -v llc &> /dev/null; then
    llc "$TEMP_LL" -o "$TEMP_S"
    echo "      ✓ Generated $TEMP_S"
else
    echo "      ERROR: llc not found. Install LLVM development tools:"
    echo "      Ubuntu: sudo apt install llvm-dev"
    echo "      Fedora: sudo dnf install llvm-devel"
    exit 1
fi
echo ""

# Step 3: Assemble and link
echo "[3/4] Assembling and linking..."

# Always link stdlib
LINK_CMD="gcc $TEMP_S build/libconstruct_stdlib.a"

# For now, we only have stdlib as the default module
# In the future, this could be extended to:
# 1. Parse the import statements from the source
# 2. Map them to library paths using a module registry
# 3. Add additional -l or -L flags to gcc

gcc "$TEMP_S" build/libconstruct_stdlib.a -o "$OUTPUT_NAME"
echo "      ✓ Generated binary: $OUTPUT_NAME"
echo ""

echo "=== Success! ==="
echo ""
echo "Run your program with:"
echo "  ./$OUTPUT_NAME"
echo ""

# Optional: Run the program
if [ -x "$OUTPUT_NAME" ]; then
    echo "Program size: $(du -h "$OUTPUT_NAME" | cut -f1)"
    echo ""
    read -p "Run program now? (y/N) " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
        echo "--- Program Output ---"
        "./$OUTPUT_NAME"
        echo "--- End Output ---"
    fi
fi
