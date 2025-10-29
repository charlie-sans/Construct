# Extended Types - Quick Reference

## All Available Types in Construct

### Integer Types (Signed)
```construct
Int                    // 32-bit signed integer
Char                   // 8-bit signed character
Short                  // 16-bit signed integer
Long                   // 64-bit signed integer
LongLong               // 64-bit signed integer (synonym for Long)
```

### Integer Types (Unsigned)
```construct
UInt                   // 32-bit unsigned integer
UChar                  // 8-bit unsigned character
UShort                 // 16-bit unsigned integer
ULong                  // 64-bit unsigned integer
ULongLong              // 64-bit unsigned integer
```

### Float Types
```construct
Float                  // 32-bit single precision float
Double                 // 64-bit double precision float
```

### Other Types
```construct
Bool                   // Boolean (true/false)
String                 // Construct string type
CStr                   // C string (const char*)
IntPtr                 // Raw void* pointer
Void                   // Return type only
```

## Common C Library Patterns

### Character I/O
```construct
extern fn putchar(c: Char): Int
extern fn getchar() -> Char
```

### String Functions
```construct
extern fn strlen(s: CStr) -> ULong
extern fn strcmp(s1: CStr, s2: CStr) -> Int
extern fn strcpy(dest: CStr, src: CStr) -> CStr
```

### Memory Functions
```construct
extern fn malloc(size: ULong) -> IntPtr
extern fn free(ptr: IntPtr)
```

### Math Functions (Float)
```construct
extern fn sin(x: Double) -> Double
extern fn cos(x: Double) -> Double
extern fn sinf(x: Float) -> Float
extern fn cosf(x: Float) -> Float
```

### File Operations
```construct
extern fn fopen(name: CStr, mode: CStr) -> IntPtr
extern fn fclose(file: IntPtr) -> Int
extern fn fread(ptr: IntPtr, size: ULong, count: ULong, file: IntPtr) -> ULong
```

## Type Choosing Guide

**When to use each type:**

| Type | Best For | Example |
|------|----------|---------|
| `Char` | ASCII characters | `let c: Char = 65` |
| `UChar` | Byte values 0-255 | `let b: UChar = 255` |
| `Short` | Small integers | `let val: Short = 1000` |
| `UShort` | Small unsigned values | `let count: UShort = 50000` |
| `Int` | General purpose integers | `let x: Int = 42` (default) |
| `UInt` | Large unsigned 32-bit | `let flags: UInt = 0xFFFFFFFF` |
| `Long` | Very large integers | `let big: Long = 9223372036854775807` |
| `ULong` | File sizes, addresses | `let size: ULong = file_size()` |
| `Float` | Memory-efficient floats | `let x: Float = 3.14` |
| `Double` | High precision math | `let pi: Double = 3.141592653589793` |

## Type Conversions

Construct's type system ensures type safety. If you need different types:

```construct
// Declare with explicit type
let i: Int = 42
let c: Char = 65
let d: Double = 3.14
let u: UInt = 100

// Types must match in expressions
dump(i)              // OK - Int
dump(c)              // OK - Char
```

## Default Behavior

- **Integer literals** without type annotation → `Int`
- **Float literals** without type annotation → `Float`
- **Type annotation** → Use exact specified type

```construct
let x = 42             // Type: Int
let y = 42: UInt       // Type: UInt
let f = 3.14           // Type: Float
let d = 3.14: Double   // Type: Double
```

## Complete Type Hierarchy

```
Construct Types
├─ Primitives
│  ├─ Integer
│  │  ├─ Char (i8)
│  │  ├─ UChar (i8)
│  │  ├─ Short (i16)
│  │  ├─ UShort (i16)
│  │  ├─ Int (i32)
│  │  ├─ UInt (i32)
│  │  ├─ Long (i64)
│  │  ├─ ULong (i64)
│  │  ├─ LongLong (i64)
│  │  └─ ULongLong (i64)
│  ├─ Float
│  │  ├─ Float (f32)
│  │  └─ Double (f64)
│  ├─ Bool (i1)
│  ├─ String (ptr)
│  └─ Void
├─ Interop
│  ├─ CStr (i8*)
│  └─ IntPtr (i8*)
└─ Composite
   ├─ Function
   ├─ List
   ├─ Tuple
   └─ Record
```

## Pro Tips

1. **Match C headers** - Use exact C types for FFI compatibility
2. **Size matters** - Choose `Short` for small values to save memory
3. **Precision** - Use `Double` for accurate calculations
4. **Performance** - Use `Float` when Double precision isn't needed
5. **Platform safe** - `Long`/`ULong` adapt to platform (32/64-bit)
