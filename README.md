# Construct - A Functional Programming Language

**Construct** is a functional programming language compiler that targets LLVM IR. It's designed to be more accessible than Haskell or F# while maintaining a strong functional programming paradigm.

## Design Philosophy

- **Functional First**: Everything is an expression; immutability by default
- **Accessible Syntax**: Python-like indentation, familiar operators
- **Type Inference**: Optional type annotations; compiler infers where needed
- **LLVM Backend**: Modern compilation to native code via LLVM

## Language Features

### 1. Functions

**Lambda (arrow) syntax:**
```construct
factorial: Int -> Int = fn (n: Int) ->
  if n <= 1
    then 1
    else n * factorial(n - 1)
```

**Block function syntax:**
```construct
fn example(x: Int): Int
{
    y: Int = x + 1
    z: Int = y * 2
    return z
}
```

### 2. Pattern Matching

```construct
match value:
  | 0 -> "zero"
  | 1 -> "one"
  | n -> "many"
```

### 3. Piping (Function Composition)

```construct
process: Int = data |> map(double) |> filter(isEven) |> sum
```

### 4. Immutable Data Structures

**Records:**
```construct
point: {x: Int, y: Int} = {x: 10, y: 20}
moved: {x: Int, y: Int} = point edit {x: 15}
```

**Lists:**
```construct
squares: [Int] = [1, 4, 9, 16, 25]
```

**Tuples:**
```construct
pair: (Int, String) = (42, "answer")
```

### 5. List Comprehensions

```construct
doubled: [Int] = [x * 2 for x in range(1, 11)]
```

### 6. Higher-Order Functions

```construct
apply_twice: (Int -> Int) -> Int -> Int = 
  fn (f: Int -> Int) -> fn (x: Int) -> f(f(x))

result: Int = apply_twice(double)(5)  # 20
```

### 7. Type Annotations (Optional)

The compiler performs type inference, but annotations help document code:

```construct
add: Int -> Int -> Int = fn (a: Int) -> fn (b: Int) -> a + b
add 2 3  # Type is inferred even without explicit annotation
```

## Project Structure

```
Construct/
├── src/
│   ├── ast.h           # Abstract Syntax Tree definitions
│   ├── lexer.h/cpp     # Tokenizer
│   ├── parser.h/cpp    # Parser (recursive descent)
│   ├── compiler.h/cpp  # AST → LLVM IR compiler
│   └── main.cpp        # Driver program
├── examples/
│   └── main.ct         # Example programs
├── meson.build         # Build configuration
└── README.md           # This file
```

## Architecture

```
Source Code (.ct)
    ↓
Lexer (Tokenization)
    ↓
Parser (AST Generation)
    ↓
Type Checker & Inference
    ↓
Compiler (LLVM IR Generation)
    ↓
LLVM Backend (Optimization & Codegen)
    ↓
Native Binary
```

## Building

### Prerequisites
- C++17 compiler (GCC, Clang)
- Meson build system
- LLVM development libraries (optional, for code generation)

### Compilation

```bash
# Setup build directory
meson setup build

# Build
meson compile -C build

# Run tests (if available)
meson test -C build
```

## Usage

```bash
./construct program.ct              # Generates program.ll
./construct program.ct -o out.ll    # Custom output filename
./construct program.ct -v           # Verbose (prints LLVM IR)
```

## Compiler Components

### Lexer (`lexer.h` / `lexer.cpp`)

Tokenizes source into:
- **Keywords**: `if`, `then`, `else`, `match`, `fn`, `let`, `type`, etc.
- **Operators**: `+`, `-`, `*`, `/`, `|>`, `->`, `==`, etc.
- **Literals**: integers, floats, strings, booleans
- **Identifiers**: variable and function names
- **Delimiters**: `()`, `[]`, `{}`

Handles Python-style indentation (INDENT/DEDENT tokens).

### Parser (`parser.h` / `parser.cpp`)

Recursive descent parser that builds an AST with:
- **Expressions**: literals, identifiers, operations, function calls
- **Patterns**: for pattern matching and destructuring
- **Types**: function types, records, tuples, lists
- **Statements**: let bindings, function definitions, imports

### Compiler (`compiler.h` / `compiler.cpp`)

Converts AST to LLVM IR:
- **Symbol Table**: tracks variable and function types
- **Type Inference**: Hindley-Milner style (simplified)
- **Code Generation**: emits LLVM IR instructions
- **Optimization**: delegates to LLVM backend

### AST (`ast.h`)

Defines node types for:
- **Types** (`Type`): Int, Float, Bool, String, Function, List, Record, Tuple
- **Expressions** (`Expr`): all expression forms
- **Patterns** (`Pattern`): destructuring patterns
- **Statements** (`Statement`): top-level definitions
- **Programs** (`Program`): collection of statements

## Current Limitations

- Type inference is basic (no full Hindley-Milner)
- No module system yet
- Limited built-in library
- No tail call optimization (LLVM can do it)
- Pattern matching is simplified
- Record types are basic

## Future Enhancements

1. **Full Type Inference** - Implement proper Hindley-Milner algorithm
2. **Standard Library** - List operations, I/O, etc.
3. **Module System** - Import and code organization
4. **Algebraic Data Types** - Union types, constructors
5. **Guards** - Pattern matching guards
6. **Lazy Evaluation** - Optional lazy function application
7. **Optimization Passes** - Custom LLVM optimization passes
8. **Better Error Messages** - Including source location info

## Example Programs

### Factorial

```construct
factorial: Int -> Int = fn (n: Int) ->
  if n <= 1
    then 1
    else n * factorial(n - 1)

result: Int = factorial 5  # 120
```

### Map Function

```construct
map: (a -> b) -> [a] -> [b] = 
  fn (f: a -> b) -> fn (xs: [a]) ->
    match xs:
      | [] -> []
      | [h, ...t] -> [f(h), ...map(f)(t)]

doubled: [Int] = map(fn (x: Int) -> x * 2)([1, 2, 3])
```

### Fibonacci with Memoization

```construct
fib: Int -> Int = fn (n: Int) ->
  if n <= 1
    then n
    else fib(n - 1) + fib(n - 2)

result: Int = fib 10
```

## Contributing

This is an educational project demonstrating:
- Language design principles
- Compiler construction
- AST manipulation
- LLVM code generation

Contributions welcome! Areas for help:
- Extended built-in functions
- Better type inference
- Optimization passes
- Error messages
- Documentation

## License

Educational purposes. Use freely in projects.

## References

- [LLVM Language Reference Manual](https://llvm.org/docs/LangRef/)
- [Crafting Interpreters](https://craftinginterpreters.com/)
- [Write an Interpreter in Go](https://interpreterbook.com/)
- [Haskell Report](https://www.haskell.org/onlinereport/)
