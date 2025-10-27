# Extern Keyword - Foreign Function Interface

The `extern` keyword in Construct allows you to declare and link to external C/C++ functions from dynamic libraries (.so files on Linux, .dll files on Windows, .dylib on macOS).

## Overview

The `extern` keyword is used to declare function signatures for functions that are defined in external libraries. When you use `extern`, you're telling the Construct compiler:

1. "This function exists in an external library"
2. "Don't generate code for this function - it will be linked at compile/link time"
3. "Use this type signature when calling the function"

## Syntax

```construct
extern fn function_name(param1: Type1, param2: Type2): ReturnType
```

### Components

- `extern` - Keyword indicating this is an external function declaration
- `fn` - Keyword indicating this is a function
- `function_name` - Name of the external function (usually the C symbol name)
- `(param1: Type1, ...)` - Parameter list with type annotations
- `: ReturnType` - Return type of the function

## Basic Examples

### Math Functions

```construct
// Declare math functions from libm
extern fn sin(x: Float): Float
extern fn cos(x: Float): Float
extern fn sqrt(x: Float): Float
extern fn pow(base: Float, exponent: Float): Float

// Use them in your code
result: Float = sin(3.14159)
hypotenuse: Float = sqrt(3.0)
```

### String Functions

```construct
// String functions from libc
extern fn strlen(str: String): Int
extern fn strcmp(str1: String, str2: String): Int
extern fn strcat(dest: String, src: String): String

// Use them
len: Int = strlen("hello")
```

### Custom Library Functions

```construct
// Declare functions from your own custom library
extern fn custom_process(data: String): String
extern fn calculate_value(x: Int, y: Int): Int

// Use them
output: String = custom_process("input data")
value: Int = calculate_value(10, 20)
```

## Type Mapping

When declaring extern functions, type annotations must match the C library's types:

| Construct Type | C Type | Notes |
|---|---|---|
| `Int` | `int32_t` / `int` | 32-bit signed integer |
| `Float` | `double` | 64-bit floating point |
| `Bool` | `int8_t` / `bool` | 8-bit boolean |
| `String` | `const char*` | Null-terminated C string |

## Compilation and Linking

### Single File Example

**myprogram.ct:**
```construct
extern fn printf(fmt: String): Int

result: Int = printf("Hello World\n")
```

**Compile:**
```bash
./construct myprogram.ct -o myprogram
```

This works because `printf` is in the standard C library which is automatically linked.

### With External Libraries

**mathprogram.ct:**
```construct
extern fn sin(x: Float): Float
extern fn cos(x: Float): Float

result1: Float = sin(0.0)
result2: Float = cos(0.0)
```

**Compile and link with math library:**
```bash
./construct mathprogram.ct -o mathprogram -lm
```

The `-lm` flag tells the linker to include the math library.

### With Custom Libraries

**usecustom.ct:**
```construct
// Your custom library provides these functions
extern fn init_system(): Int
extern fn process_value(val: Int): Int
extern fn cleanup(): Int

status: Int = init_system()
value: Int = process_value(42)
```

**Compile:**
```bash
# If custom library is in current directory
./construct usecustom.ct -o usecustom -L. -lcustomlib

# Or if installed system-wide
./construct usecustom.ct -o usecustom -lcustomlib
```

## Important Considerations

### 1. No Function Body

Extern functions cannot have a body - they are declarations only:

```construct
// ✓ Correct
extern fn sqrt(x: Float): Float

// ✗ Wrong - will cause parse error
extern fn sqrt(x: Float): Float {
    return x * x  // Error: extern functions can't have bodies
}
```

### 2. Type Safety

The type annotations you provide are critical for correctness. If types don't match the actual C function, you may get runtime errors:

```construct
// If actual C function is: int add(int a, int b)
extern fn add(x: Int, y: Int): Int  // ✓ Correct

// This would be wrong:
extern fn add(x: Float, y: Float): Float  // ✗ Type mismatch!
```

### 3. Function Naming

By default, the function name in your Construct code must match the C symbol name exactly:

```construct
// Assumes there's a C function named "my_function"
extern fn my_function(x: Int): Int
```

### 4. Linking Order Matters

When linking multiple libraries, you may need to specify them in the correct order:

```bash
# Correct order: mylib depends on customlib
./construct program.ct -o program -lmylib -lcustomlib

# Might fail: reverse order if mylib depends on customlib
./construct program.ct -o program -lcustomlib -lmylib
```

### 5. No Variadic Functions Yet

Variadic C functions (like `printf`) are supported by the Construct stdlib, but custom variadic functions are not yet fully supported.

```construct
// This works (built-in)
result: Int = printf("%d\n", 42)

// But declaring custom variadic functions isn't yet supported
// extern fn custom_printf(fmt: String, ...): Int  // Not supported yet
```

## Common Libraries to Link

| Library | Flag | Functions | Use Case |
|---|---|---|---|
| C Standard Library | `-lc` | `printf`, `strlen`, etc. | Default, usually linked automatically |
| Math Library | `-lm` | `sin`, `cos`, `sqrt`, `pow` | Mathematical operations |
| POSIX Threads | `-lpthread` | Thread functions | Multi-threading |
| OpenSSL | `-lssl -lcrypto` | Cryptographic functions | Encryption/Security |
| Zlib | `-lz` | Compression functions | Data compression |

## Workflow Example

### Step 1: Create Construct Program

**fibonacci.ct:**
```construct
extern fn printf(fmt: String): Int

// Calculate fibonacci number
fibonacci: Int = 8
result: Int = printf("Fibonacci(%d) calculated\n", fibonacci)
```

### Step 2: Compile

```bash
./construct fibonacci.ct -o fibonacci
```

### Step 3: Run

```bash
./fibonacci
```

Output:
```
Fibonacci(8) calculated
```

## Advantages

1. **Reuse Existing Code** - Leverage battle-tested C/C++ libraries
2. **Performance** - No performance overhead for external function calls
3. **Interoperability** - Full C FFI (Foreign Function Interface) support
4. **Flexibility** - Use specialized libraries like OpenSSL, Zlib, etc.
5. **Gradual Migration** - Wrap existing C codebases in Construct

## Limitations & Future Work

- [ ] Variadic function support (like `printf` with multiple arguments)
- [ ] Struct/Record FFI (currently only scalar types)
- [ ] Callback functions (passing Construct functions to C)
- [ ] Automatic wrapper generation from C headers
- [ ] C++ function support

## See Also

- [Function Definitions](FUNCTION_CALLS.md)
- [Type System](LANGUAGE_FEATURES.md)
- [External Linking](EXTERNAL_LINKING_AND_STDLIB.md)
- Examples: `examples/extern_functions.ct`
