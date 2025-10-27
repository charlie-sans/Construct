#!/bin/bash
# compile_to_native.sh
# Compiles a Construct program to native binary

set -e

if [ $# -lt 1 ]; then
    echo "Usage: $0 <program.ct> [output_name]"
    echo ""
    echo "Compiles a Construct program to native binary"
    echo ""
    echo "Example:"
    echo "  $0 examples/hello.ct"
    echo "  $0 examples/hello.ct my_program"
    exit 1
fi

SOURCE_FILE="$1"
OUTPUT_NAME="${2:-$(basename "$SOURCE_FILE" .ct)}"
TEMP_LL="/tmp/${OUTPUT_NAME}.ll"
TEMP_S="/tmp/${OUTPUT_NAME}.s"

echo "=== Construct Compiler - Full Pipeline ==="
echo ""
echo "Source:     $SOURCE_FILE"
echo "Output:     $OUTPUT_NAME"
echo "Temp IR:    $TEMP_LL"
echo "Temp ASM:   $TEMP_S"
echo ""

# Step 1: Compile to LLVM IR
echo "[1/4] Compiling to LLVM IR..."
./build/construct "$SOURCE_FILE" -o "$TEMP_LL"
echo "      ✓ Generated $TEMP_LL"
echo ""

# Step 2: Verify LLVM IR (optional)
if command -v llvm-as &> /dev/null; then
    echo "[2/4] Verifying LLVM IR..."
    llvm-as "$TEMP_LL" -o /tmp/temp.bc
    echo "      ✓ IR is valid"
    echo ""
else
    echo "[2/4] Skipping verification (llvm-as not found)"
    echo ""
fi

# Step 3: Generate assembly
echo "[3/4] Generating assembly..."
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

# Step 4: Assemble and link
echo "[4/4] Assembling and linking..."
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
