# Construct Standard Library

The Construct standard library provides essential I/O and utility functions for compiled programs.

## Organization

```
src/srdlib/          # Standard library directory
├── stdlib.h         # Public C declarations (for compiler use)
├── Stream.cpp       # I/O functions implementation
└── [future modules]
```

## Output Functions (I/O)

### dump(value) → void

Dumps a value to stdout without newline. Works with all types:

```construct
dump(42)           # prints: 42
dump(3.14)         # prints: 3.14
dump(true)         # prints: true
dump(false)        # prints: false
dump("hello")      # prints: hello
```

**Implementation:**
- `dump()` is a special builtin that dispatches to type-specific runtime functions
- Integer: calls `construct_dump_int(i32)`
- Float: calls `construct_dump_float(double)`
- Boolean: calls `construct_dump_bool(i8)` → prints "true" or "false"
- String: calls `construct_dump_string(char*)`

### Usage Examples

```construct
# Simple output
dump(42)

# With newlines (using string literals)
dump(100)
dump("\n")

# Chaining multiple values
dump("Result: ")
dump(42)
dump("\n")

# In conditionals
if (x > 0)
  then dump("positive")
  else dump("non-positive")
```

## Compiler Integration

The compiler automatically:
1. Recognizes `dump()` as a builtin function
2. Determines the argument type from IR
3. Calls the appropriate runtime function
4. Links with `srdlib/Stream.cpp`

### Generated IR

For `dump(42)`, the compiler generates:

```llvm
declare void @construct_dump_int(i32)

define i32 @main() {
entry:
  call void @construct_dump_int(i32 42)
  ret i32 0
}
```

## Future Standard Library

### Input Functions (Planned)

```construct
read_int()        # → Int - read integer from stdin
read_float()      # → Float - read float from stdin
read_string()     # → String - read line from stdin
```

### Type Conversion (Planned)

```construct
to_string(42)     # → String - convert int to string
to_int("42")      # → Int - convert string to int
to_float(42)      # → Float - int to float conversion
```

### Collection Functions (Planned)

```construct
length([1,2,3])   # → 3 - get list length
first([1,2,3])    # → 1 - get first element
rest([1,2,3])     # → [2,3] - get tail
```

### Math Functions (Planned)

```construct
abs(-5)           # → 5
max(a, b)         # → maximum value
min(a, b)         # → minimum value
sqrt(16.0)        # → 4.0
```

## Building with Stdlib

The build system automatically includes stdlib files:

**meson.build:**
```meson
sources = [
    'src/main.cpp',
    'src/lexer.cpp',
    'src/parser.cpp',
    'src/compiler.cpp',
    'src/llvm_codegen.cpp',
    'src/srdlib/Stream.cpp',
]
```

The compiled `construct` binary links with all stdlib implementations.

## Runtime Support

Stdlib functions are normal C++ functions with `extern "C"` linkage:

```cpp
extern "C" void construct_dump_int(int32_t value) {
    printf("%d", value);
}
```

This allows:
- Compilation to LLVM IR that calls these symbols
- Linking with the compiled stdlib object files
- Direct usage from command-line tools

## Example Program

**program.ct:**
```construct
dump("Enter a number: ")
dump(42)
dump("\n")
dump("Done!")
dump("\n")
```

**Compiled to:**
```llvm
declare void @construct_dump_string(ptr)
declare void @construct_dump_int(i32)

define i32 @main() {
entry:
  call void @construct_dump_string(ptr @.str1)
  call void @construct_dump_int(i32 42)
  call void @construct_dump_string(ptr @.str2)
  call void @construct_dump_string(ptr @.str3)
  call void @construct_dump_string(ptr @.str4)
  ret i32 0
}
```

**Execution:**
```bash
$ ./construct program.ct > program.ll
$ llc program.ll
$ gcc -o program program.s src/srdlib/Stream.o
$ ./program
Enter a number: 42
Done!
```

## Design Principles

1. **Minimal Core**: Only essential I/O functions in stdlib
2. **Extensible**: Easy to add more functions
3. **Type-Safe**: Each type has its own function
4. **Efficient**: Direct C implementation, no overhead
5. **C Compatible**: Can link with existing C libraries

## Testing

All stdlib functions are tested with:

```bash
./build/construct examples/dump_test.ct
```

This generates `dump_test.ll` showing the IR calls to stdlib functions.
