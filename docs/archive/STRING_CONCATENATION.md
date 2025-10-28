# String Concatenation with `+` Operator

## Overview

The Construct language now supports intuitive string concatenation using the `+` operator, making it easy to combine strings directly in function arguments and expressions.

## Syntax

```construct
"string1" + "string2"
string_var + "suffix"
"prefix" + variable
string_var + toStr(number)
"a" + "b" + "c"  (* multiple concatenations *)
```

## Basic Examples

### Simple Concatenation
```construct
let greeting: String = "Hello" + " " + "World"
dump(greeting)  (* Output: Hello World *)
nl()
```

### In Function Calls
```construct
dump("The answer is " + "42")  (* Output: The answer is 42 *)
nl()
```

### With Type Conversion
```construct
let x: Int = 100
dump("Number: " + toStr(x))  (* Output: Number: 100 *)
nl()
```

### Multiple Concatenations
```construct
dump("A" + "B" + "C" + "D")  (* Output: ABCD *)
nl()
```

### Mixed Variables and Literals
```construct
let name: String = "Construct"
dump("Hello " + name + " language!")  (* Output: Hello Construct language! *)
nl()
```

### Complex Expressions
```construct
let count: Int = 42
let total: Int = 100
dump("Progress: " + toStr(count) + " of " + toStr(total) + " items")
nl()
```

## How It Works

### Type Handling
The `+` operator automatically handles type conversion:
- **String + String** → Direct concatenation
- **String + Int** → Converts int to string and concatenates
- **String + Float** → Converts float to string and concatenates
- **String + Bool** → Converts bool to string and concatenates
- **Any type + String** → Same conversions apply

### Example with Type Coercion
```construct
let x: Int = 42
let y: Float = 3.14
dump("Values: " + toStr(x) + " and " + toStr(y))
nl()
```

## Implementation Details

### Code Generation
When the compiler encounters `"str1" + "str2"`, it:
1. Checks if either operand is a string (pointer type)
2. If not, converts both to strings using `toStr()`, `toInt()`, etc.
3. Generates a call to `construct_string_concat(left, right)`

### Memory Management
- Each concatenation dynamically allocates memory using `malloc()`
- The returned pointer points to a valid heap-allocated string
- For long chains of concatenations, multiple allocations occur

### Performance Notes
- Each `+` operation allocates a new string on the heap
- Deeply nested concatenations may create multiple temporary strings
- For maximum efficiency, pre-allocate if building very long strings

## Comparison with Alternative Approaches

### Before (without + operator)
```construct
let result: String = concat(concat("Hello", " "), "World")
dump(result)
nl()
```

### After (with + operator)
```construct
dump("Hello" + " " + "World")
nl()
```

## Common Use Cases

### Building Formatted Output
```construct
let id: Int = 123
let status: String = "active"
dump("User ID: " + toStr(id) + " - Status: " + status)
nl()
```

### Creating Labels
```construct
let item: String = "book"
let count: Int = 5
dump(toStr(count) + " " + item + "s")  (* Output: 5 books *)
nl()
```

### Combining String Operations
```construct
let text: String = "hello"
dump("Uppercase: " + upper(text))  (* Output: Uppercase: HELLO *)
nl()
```

## Limitations

1. **Operator Precedence**: The `+` operator has the same precedence as arithmetic addition
   - Use parentheses for clarity: `"a" + (b + c)`

2. **Memory Allocation**: Each concatenation creates a new string
   - Very long chains create intermediate strings that can be freed

3. **No In-Place Modification**: Strings are immutable
   - `s = s + "more"` creates a new string

## Examples from Test Suite

### test_concat.ct
```construct
let result1: String = "Hello" + " " + "World"
dump(result1)
nl()

let result2: String = "Value: " + "42"
dump(result2)
nl()

dump("Done!")
nl()
```

Output:
```
Hello World
Value: 42
Done!
```

### test_concat_advanced.ct
```construct
let x: Int = 42
dump("The answer is " + toStr(x))
nl()

let name: String = "Construct"
dump("Hello " + name + " language!")
nl()

dump("Combining: " + "numbers " + toStr(100) + " and " + toStr(200))
nl()
```

Output:
```
The answer is 42
Hello Construct language!
Combining: numbers 100 and 200
```

## Technical Implementation

### Code Generator (llvm_codegen.cpp)
The `+` operator in `codegenBinOpExpr()` checks operand types:

```cpp
if (expr->op == "+") {
    // String concatenation: if either operand is a string
    if (left->getType()->isPointerTy() || right->getType()->isPointerTy()) {
        // Convert both to strings and call construct_string_concat
        // ...
    } else if (left->getType()->isIntegerTy()) {
        // Integer addition
    } else {
        // Float addition
    }
}
```

### Runtime (Stream.cpp)
The `construct_string_concat()` function allocates memory for the result:

```cpp
extern "C" const char* construct_string_concat(const char* a, const char* b) {
    if (!a) a = "";
    if (!b) b = "";
    
    size_t len_a = strlen(a);
    size_t len_b = strlen(b);
    size_t total = len_a + len_b + 1;
    
    char* result = (char*)malloc(total);
    if (!result) return "";
    
    strcpy(result, a);
    strcat(result, b);
    return result;
}
```

## Related Features

- `concat(s1, s2)` - Explicit string concatenation function
- `toStr(x)` - Convert any type to string
- `upper(s)` - Convert string to uppercase
- `lower(s)` - Convert string to lowercase
- `trim(s)` - Remove leading/trailing whitespace

---

**Feature Status:** ✓ IMPLEMENTED AND TESTED  
**Date:** October 28, 2025
