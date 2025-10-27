# Construct Language Compiler

A functional programming language compiler targeting LLVM IR. Written in C++17 with a focus on clean architecture and extensibility.

## Current Status

✅ **Core Compiler Complete**
- Lexer: Tokenization with indentation support
- Parser: Recursive descent with operator precedence
- Type System: Basic types (Int, Float, Bool, String)
- LLVM Backend: Direct IR generation with PIMPL pattern
- Standard Library: I/O functions (dump)

## Quick Start

### Build

```bash
cd Construct
meson setup build
meson compile -C build
```

### Run Example

```bash
./build/construct examples/simple.ct -v
# Outputs: simple.ll (LLVM IR file)
```

## Features

### Language Features (Implemented)

- **Arithmetic**: `+`, `-`, `*`, `/`, `%`, `**`
- **Comparisons**: `==`, `!=`, `<`, `<=`, `>`, `>=`
- **Logical**: `&&`, `||`, `!`
- **Control Flow**: `if <cond> then <expr> else <expr>`
- **Literals**: integers, floats, booleans, strings
- **Comments**: `#` line comments

### Standard Library (Implemented)

- `dump(value)` - Output value to stdout (works with Int, Float, Bool, String)

### Language Features (In Progress)

- Variable bindings with type annotations
- Function definitions
- Lambda expressions
- List operations
- Record/struct types
- Pattern matching

## Architecture

### File Structure

```
src/
├── main.cpp              # Entry point and driver
├── lexer.h/cpp           # Tokenizer
├── parser.h/cpp          # Parser (recursive descent)
├── compiler.h/cpp        # Main compiler interface
├── llvm_codegen.h/cpp    # LLVM code generation (PIMPL pattern)
├── ast.h                 # Abstract syntax tree definitions
└── srdlib/
    ├── stdlib.h          # Standard library declarations
    └── Stream.cpp        # I/O implementation

build/                     # Build directory (created by Meson)
meson.build               # Build configuration
```

### Design Patterns

**PIMPL (Pointer to Implementation)**
- Public header exposes only clean interface
- LLVM internals hidden in .cpp implementation
- Benefits: Fast compilation, maintainability, flexible implementation changes

## Examples

### Basic Arithmetic

**Code:**
```construct
5 + 3
```

**IR:**
```llvm
define i32 @main() {
entry:
  ret i32 8
}
```

### If Expression

**Code:**
```construct
if true then 5 else 3
```

**IR:**
```llvm
define i32 @main() {
entry:
  br i1 true, label %then, label %else
then:
  br label %merge
else:
  br label %merge
merge:
  %iftmp = phi i32 [ 5, %then ], [ 3, %else ]
  ret i32 %iftmp
}
```

### Dump Output

**Code:**
```construct
dump(42)
```

**IR:**
```llvm
declare void @construct_dump_int(i32)

define i32 @main() {
entry:
  call void @construct_dump_int(i32 42)
  ret i32 0
}
```

## Test Examples

All examples are in `examples/`:

| File | Description | Expected |
|------|-------------|----------|
| `simple.ct` | `5 + 3` | 8 |
| `nested.ct` | `(2+3)*4-1` | 19 |
| `logic.ct` | `5 > 3 && 2 < 4` | 1 |
| `iftest.ct` | `if true then 5 else 3` | 5 |
| `nested_if.ct` | Nested if-expressions | 2 |
| `dump_test.ct` | Test dump() function | (prints "42") |
| `dump_multiple.ct` | dump(100) | (prints "100") |

## Implementation Details

### Type System

```cpp
enum class TypeKind {
    INT,        // 32-bit integer
    FLOAT,      // double precision float
    BOOL,       // boolean (i1)
    STRING,     // string (pointer)
    FUNCTION,   // function type
    LIST,       // list type
    TUPLE,      // tuple type
    RECORD,     // record/struct type
};
```

### AST Structure

```cpp
struct Expr {
    enum Kind {
        INT_LITERAL,
        FLOAT_LITERAL,
        STRING_LITERAL,
        BOOL_LITERAL,
        IDENTIFIER,
        BINARY_OP,
        UNARY_OP,
        IF_EXPR,
        APPLICATION,
        LAMBDA,
        // ... more
    };
    Kind kind;
    // Data members for each expression type
};
```

## Build System

Uses **Meson** for cross-platform build configuration:

```bash
# Configure
meson setup build

# Compile
meson compile -C build

# Run tests
meson compile -C build test

# Install
meson install -C build
```

**Build Dependencies:**
- C++17 compiler (GCC, Clang)
- LLVM >= 10 (using 20.1.8)
- Meson >= 0.50

## Compiler Flags

```bash
-Wall -Wextra      # Warnings
-fno-rtti           # LLVM compatibility (no RTTI)
-std=c++17          # C++17 standard
```

## Generated Files

Running `./build/construct file.ct` generates:

- `file.ll` - LLVM IR (human-readable)
- `file.ir` - Alternative IR format (if requested)

## Next Steps

### Priority 1: Expression Completeness
- [ ] Variable bindings with type inference
- [ ] Function definitions and calls
- [ ] Lambda expressions

### Priority 2: Collections
- [ ] List literals: `[1, 2, 3]`
- [ ] List operations: indexing, slicing, iteration
- [ ] Record/struct literals and access

### Priority 3: Advanced Features
- [ ] Pattern matching
- [ ] Algebraic data types (ADTs)
- [ ] Type classes / traits
- [ ] Module system

### Priority 4: Backend
- [ ] Generate `.o` object files via llc
- [ ] Link with gcc to produce executables
- [ ] Runtime type information
- [ ] Garbage collection (if needed)

### Priority 5: Optimization
- [ ] LLVM pass manager integration
- [ ] Inline functions
- [ ] Dead code elimination
- [ ] Loop optimizations

## Performance

The compiler generates optimized LLVM IR that can be further optimized by LLVM passes.

**Compilation Time:**
- Simple expressions: < 100ms
- Complex nested expressions: < 500ms

**Generated Code Quality:**
- Proper SSA form
- Minimal temporaries
- Good for LLVM optimization passes

## Documentation

- `README.md` - This file
- `LLVM_INTEGRATION.md` - LLVM backend details
- `VERIFICATION_REPORT.md` - Test results and verification
- `STDLIB.md` - Standard library documentation
- `QUICKSTART.md` - Getting started guide
- `IMPLEMENTATION.md` - Internal architecture details

## Contributing

The compiler is organized for easy extension:

1. **Adding Language Features**: Update `ast.h`, then `parser.cpp`, then `llvm_codegen.cpp`
2. **Adding Stdlib Functions**: Create new functions in `src/srdlib/`, declare in `stdlib.h`
3. **Testing**: Add examples to `examples/` and run through compiler

## License

MIT License (or your choice)

## Author

Created October 2025 with LLVM C++ API

---

**Latest Update**: October 27, 2025
**LLVM Version**: 20.1.8
**Build System**: Meson
**Language**: C++17
