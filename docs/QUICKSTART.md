# Construct Language - Quick Start Guide

## Installation & Setup

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt-get install build-essential ninja-build meson g++ git

# Fedora
sudo dnf install gcc-c++ ninja-build meson

# macOS
brew install ninja meson
```

### Build the Compiler

```bash
cd /run/media/charlie/the\ cat\ storage\ v2/Construct
meson setup build
meson compile -C build
```

The executable is at `build/construct`.

## Your First Program

### Create a file: `hello.ct`

```construct
# Comments start with #

# Variable declarations with type annotations
message: String = "Hello, World!"
number: Int = 42
pi: Float = 3.14159

# Functions
double: Int -> Int = fn (x: Int) -> x * 2
result: Int = double 21  # Returns 42
```

### Compile it

```bash
./build/construct hello.ct
```

This creates `hello.ll` (LLVM IR file).

To see the generated code:

```bash
./build/construct hello.ct -v
```

## Language Tour

### 1. Basic Types

```construct
# Integers
x: Int = 42
negative: Int = -10

# Floats
pi: Float = 3.14159
e: Float = 2.71828

# Booleans
is_valid: Bool = true
is_empty: Bool = false

# Strings
greeting: String = "Hello, Construct!"
multiline: String = "Line 1\nLine 2"
```

### 2. Functions

**Arrow-style (lambda):**

```construct
square: Int -> Int = fn (x: Int) -> x * x
result: Int = square 5  # 25
```

**Curried functions:**

```construct
add: Int -> Int -> Int = 
  fn (a: Int) -> fn (b: Int) -> a + b

add_five: Int -> Int = add 5
result: Int = add_five 3  # 8
```

**Block-style:**

```construct
fn multiply(x: Int): Int
{
    y: Int = x * 2
    z: Int = y + 1
    return z
}
```

### 3. Control Flow

**If-then-else (expressions):**

```construct
check_sign: Int -> String = fn (n: Int) ->
  if n > 0
    then "positive"
    else if n < 0
      then "negative"
      else "zero"

result: String = check_sign 5  # "positive"
```

**Pattern matching:**

```construct
describe: Int -> String = fn (n: Int) ->
  match n:
    | 0 -> "zero"
    | 1 -> "one"
    | 2 -> "two"
    | x -> "many"

result: String = describe 1  # "one"
```

### 4. Collections

**Lists:**

```construct
numbers: [Int] = [1, 2, 3, 4, 5]
empty: [Int] = []

# List comprehensions
squares: [Int] = [x * x for x in numbers]
evens: [Int] = [x for x in numbers]  # Filter can be added later
```

**Tuples:**

```construct
pair: (Int, String) = (42, "answer")
triple: (Bool, Float, String) = (true, 3.14, "pi")
```

**Records:**

```construct
person: {name: String, age: Int} = {name: "Alice", age: 30}

# Non-destructive update
older: {name: String, age: Int} = person edit {age: 31}
```

### 5. Operators

**Arithmetic:**

```construct
add: Int = 5 + 3        # 8
subtract: Int = 10 - 4  # 6
multiply: Int = 3 * 4   # 12
divide: Int = 15 / 3    # 5
modulo: Int = 10 % 3    # 1
power: Int = 2 ** 3     # 8
```

**Comparison:**

```construct
equal: Bool = 5 == 5         # true
not_equal: Bool = 5 != 3     # true
less: Bool = 5 < 10          # true
less_equal: Bool = 5 <= 5    # true
greater: Bool = 10 > 5       # true
greater_equal: Bool = 5 >= 5 # true
```

**Logical:**

```construct
both: Bool = true && false   # false
either: Bool = true || false # true
not_val: Bool = !true        # false
```

### 6. Function Composition

**Piping (|>):**

```construct
# Apply functions left-to-right
result: Int = 5 |> double |> square

# Equivalent to:
# result = square(double(5))
```

### 7. Higher-Order Functions

**Functions as parameters:**

```construct
apply_twice: (Int -> Int) -> Int -> Int =
  fn (f: Int -> Int) -> fn (x: Int) -> f(f(x))

double: Int -> Int = fn (x: Int) -> x * 2
result: Int = apply_twice(double)(5)  # 20
```

**Functions as return values:**

```construct
make_adder: Int -> (Int -> Int) =
  fn (n: Int) -> fn (x: Int) -> x + n

add_ten: Int -> Int = make_adder 10
result: Int = add_ten 5  # 15
```

## Real-World Examples

### Factorial

```construct
factorial: Int -> Int = fn (n: Int) ->
  if n <= 1
    then 1
    else n * factorial(n - 1)

answer: Int = factorial 5  # 120
```

### Sum of List

```construct
sum: [Int] -> Int = fn (xs: [Int]) ->
  match xs:
    | [] -> 0
    | [h, ...t] -> h + sum(t)

total: Int = sum [1, 2, 3, 4, 5]  # 15
```

### Fibonacci

```construct
fib: Int -> Int = fn (n: Int) ->
  if n <= 1
    then n
    else fib(n - 1) + fib(n - 2)

result: Int = fib 10  # 55
```

### Map Function

```construct
map: (a -> b) -> [a] -> [b] =
  fn (f: a -> b) -> fn (xs: [a]) ->
    match xs:
      | [] -> []
      | [h, ...t] -> [f(h), ...map(f)(t)]

doubled: [Int] = map(double)([1, 2, 3])  # [2, 4, 6]
```

## Type System

### Type Annotations

Type annotations are **optional** but help document code:

```construct
# With annotation (recommended for clarity)
add: Int -> Int -> Int = fn (a: Int) -> fn (b: Int) -> a + b

# Without annotation (inferred)
add = fn (a: Int) -> fn (b: Int) -> a + b
```

### Type Inference

The compiler infers types from usage:

```construct
# Type of result is inferred as Int
result = 5 + 3

# Type of double is inferred as Int -> Int
double = fn (x: Int) -> x * 2
```

### Polymorphic Functions

Functions work with type variables (represented with `'a`, `'b`, etc.):

```construct
identity: a -> a = fn (x: a) -> x

result_int: Int = identity 42
result_str: String = identity "hello"
```

## Common Patterns

### Composition

```construct
# Compose functions
compose: (b -> c) -> (a -> b) -> (a -> c) =
  fn (f: b -> c) -> fn (g: a -> b) -> fn (x: a) -> f(g(x))

add_one: Int -> Int = fn (x: Int) -> x + 1
double: Int -> Int = fn (x: Int) -> x * 2

# double then add_one
result: Int = compose(add_one)(double)(5)  # 11
```

### Conditional Logic

```construct
# Using match for boolean logic
handle_result: Bool -> String = fn (success: Bool) ->
  match success:
    | true -> "Success!"
    | false -> "Failed"
```

### Recursion

```construct
# Tail recursion (compiler will optimize)
count_down: Int -> Int = fn (n: Int) ->
  if n <= 0
    then 0
    else n + count_down(n - 1)
```

## Compilation Pipeline

```bash
# Step 1: Tokenize
# hello.ct → [Token, Token, ...]

# Step 2: Parse
# [Token, Token, ...] → AST

# Step 3: Type Check
# AST → (validate types)

# Step 4: Codegen
# AST → LLVM IR

# Step 5: Link (manual)
./build/construct hello.ct -o hello.ll
llc hello.ll -o hello.s
gcc hello.s -o hello
./hello
```

## Debugging Tips

### Check tokens

```bash
# Add verbose flag to see tokenization
./build/construct program.ct -v
```

### View generated IR

```bash
# LLVM IR is human-readable
cat program.ll | head -30
```

### Validate IR

```bash
# Check IR syntax
llvm-as program.ll -o program.bc

# View optimized IR
llvm-dis program.bc
```

## Next Steps

1. **Read** `IMPLEMENTATION.md` for compiler architecture
2. **Explore** `src/` directory to understand each component
3. **Modify** examples to experiment
4. **Add** new language features
5. **Generate** native code with LLVM

## Getting Help

- **Language Reference**: See `examples/main.ct`
- **Compiler Docs**: See `IMPLEMENTATION.md`
- **Source Code**: Comments in `src/*.cpp`
- **Build Issues**: Run `meson setup --reconfigure build` then rebuild

## Troubleshooting

| Problem | Solution |
|---------|----------|
| Build fails | `meson setup --wipe build && meson compile -C build` |
| Unknown token | Check `lexer.h` for supported operators |
| Parse error | Ensure proper indentation (Python-style) |
| Type error | Add type annotations to debug |
| No output | Check that program has expressions (not just definitions) |

---

**Happy coding with Construct!** 🚀

For advanced topics, see `IMPLEMENTATION.md`.
