# Improved I/O Functions: `show()` and `showln()`

## Overview

The Construct language now offers cleaner, more intuitive I/O functions with support for variadic (multiple) arguments. The `show()` and `showln()` functions are replacements for the older `dump()` naming convention, providing better clarity and ergonomics.

## New I/O Functions

### `show()` - Output Without Newline

Outputs one or more values to stdout without a trailing newline.

**Syntax:**
```construct
show(value1)
show(value1, value2, value3)
```

**Features:**
- Works with any type: Int, Float, Bool, String
- Supports multiple arguments (variadic)
- No trailing newline
- Returns void (but can be chained)

### `showln()` - Output With Newline

Outputs one or more values to stdout with a trailing newline.

**Syntax:**
```construct
showln(value1)
showln(value1, value2, value3)
showln()  (* Just print newline *)
```

**Features:**
- Works with any type: Int, Float, Bool, String
- Supports multiple arguments (variadic)
- Automatically adds newline after all arguments
- No-argument version just prints a newline

## Examples

### Basic Output

```construct
show("Hello ")
show("World")
showln()
(* Output: Hello World *)
```

### Direct Output with Newline

```construct
showln("Hello, Construct!")
(* Output: Hello, Construct! *)
```

### Multiple Arguments

```construct
show("A", "B", "C")
showln()
(* Output: ABC *)

show("Numbers: ", 1, 2, 3)
showln()
(* Output: Numbers: 123 *)
```

### Mixed Types

```construct
let x: Int = 42
let y: Float = 3.14
let name: String = "Construct"

show("Language: ")
show(name)
showln()
(* Output: Language: Construct *)

showln("Values: ", x, " and ", y)
(* Output: Values: 42 and 3.14 *)
```

### With Variables

```construct
let count: Int = 100
show("Count: ")
showln(count)
(* Output: Count: 100 *)

show("Result: ")
show(count + 50)
showln()
(* Output: Result: 150 *)
```

### Real-World Examples

#### Simple Report
```construct
let name: String = "Alice"
let age: Int = 30
let height: Float = 5.6

showln("Name: ", name)
showln("Age: ", age)
showln("Height: ", height)

(* Output:
   Name: Alice
   Age: 30
   Height: 5.6
*)
```

#### Formatted Output
```construct
show("Processing: ")
show("Item 1 ... ")
show("Item 2 ... ")
showln("Done!")
(* Output: Processing: Item 1 ... Item 2 ... Done! *)
```

#### Loop Output
```construct
let i: Int = 0
show("Sequence: ")
show(i)
show(" ")
show(i + 1)
show(" ")
show(i + 2)
showln()
(* Output: Sequence: 0 1 2 *)
```

## Comparison with Older Functions

### Before (dump style)

```construct
dump("Hello ")
dump("World")
nl()
dump_line("Done!")
```

### After (show style)

```construct
show("Hello ")
show("World")
showln()
showln("Done!")
```

## Type Support

Both `show()` and `showln()` automatically handle type conversion:

| Type | Output Format |
|------|---------------|
| Int | Decimal number (e.g., `42`) |
| Float | Decimal with precision (e.g., `3.14159`) |
| Bool | `true` or `false` |
| String | String literal content |

## Function Signatures

### C Runtime Functions (Internal)

```c
void construct_show_int(int32_t value);
void construct_show_float(double value);
void construct_show_bool(int8_t value);
void construct_show_string(const char* str);

void construct_showln_int(int32_t value);
void construct_showln_float(double value);
void construct_showln_bool(int8_t value);
void construct_showln_string(const char* str);

void construct_showln(void);  /* Newline only */
```

### Code Generation

When the Construct compiler sees `show(arg1, arg2, arg3)`, it:
1. Determines the type of each argument
2. Routes to the appropriate `construct_show_*()` function
3. Generates multiple calls for multiple arguments
4. For `showln()`, adds a call to `construct_showln()` at the end

## Implementation Details

### Variadic Support via Code Generation

Since C doesn't support true variadic functions with type-safe handling, Construct implements variadic support at the code generation level:

```
Construct Code:  show("A", "B", "C")
                    ↓
Generated IR:   call @construct_show_string("A")
                call @construct_show_string("B")
                call @construct_show_string("C")
                    ↓
Runtime:        ABC (output concatenated)
```

### Return Value

Both `show()` and `showln()` are effectively void but return an integer constant (0) to maintain type system compatibility.

## Migration Guide

If you're using the older `dump()` functions:

| Old Code | New Code |
|----------|----------|
| `dump(x)` | `show(x)` |
| `dump_line(x)` | `showln(x)` |
| `nl()` | `showln()` |
| `dump(a); dump(b)` | `show(a, b)` |
| `dump(a); dump(b); nl()` | `showln(a, b)` |

## Testing

Comprehensive tests are provided:

- **test_show.ct** - Basic show/showln functionality
- **test_show_variadic.ct** - Multiple argument tests

Both test files compile and execute successfully, demonstrating the full capabilities of the new I/O functions.

## Performance

- **Single argument**: Minimal overhead, single function call
- **Multiple arguments**: One function call per argument (no string concatenation overhead)
- **No hidden allocations**: Uses stack-based printf under the hood

## Backward Compatibility

The old `dump()` and `dump_line()` functions remain available but are considered legacy. New code should prefer `show()` and `showln()` for better clarity.

---

**Feature Status:** ✓ IMPLEMENTED AND TESTED  
**Date:** October 28, 2025  
**Available in:** Construct compiler (all platforms)
