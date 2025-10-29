# Adding C Integer and Float Types to Construct

## Goal
Expand Construct's type system to support more C primitive types for better FFI compatibility.

## Current Types
- INT (int)
- FLOAT (float)
- BOOL (bool)
- STRING (Construct string)
- CSTR (const char*)
- INTPTR (void* / raw pointer)
- VOID (void)

## New Types to Add

### Character Types
- `Char` - signed char (8-bit)
- `UChar` - unsigned char (8-bit)

### Short Integer Types
- `Short` - short (16-bit signed)
- `UShort` - unsigned short (16-bit unsigned)

### Extended Integer Types
- `UInt` - unsigned int (32-bit unsigned)
- `Long` - long (64-bit signed on most platforms)
- `ULong` - unsigned long (64-bit unsigned)
- `LongLong` - long long (64-bit signed)
- `ULongLong` - unsigned long long (64-bit unsigned)

### Float Types
- `Double` - double precision float (64-bit)

## Files to Modify

### 1. src/ast.h
Add new Kind enum values for each type

### 2. src/parser.cpp
Add string matching in parseType() function

### 3. src/llvm_codegen.cpp
Map new types to LLVM IR types (i8, i16, i32, i64, f64, etc.)

## LLVM Type Mappings

| Construct Type | C Type | LLVM Type |
|---|---|---|
| Int | int | i32 |
| Char | char | i8 |
| UChar | unsigned char | i8 |
| Short | short | i16 |
| UShort | unsigned short | i16 |
| UInt | unsigned int | i32 |
| Long | long | i64 |
| ULong | unsigned long | i64 |
| LongLong | long long | i64 |
| ULongLong | unsigned long long | i64 |
| Float | float | f32 |
| Double | double | f64 |
| CStr | const char* | i8* |
| IntPtr | void* | i8* |

## Benefits

✅ Better C library interop
✅ Precise type matching with C headers
✅ No need for type casting workarounds
✅ Clearer intent in code
✅ Future-proof for more complex FFI scenarios

## Implementation Plan

1. Add 11 new enum values to Type::Kind
2. Add make*() static factory methods for each type
3. Update parseType() string matching
4. Update getLLVMType() in codegen to map new types
5. Create test cases in examples/

## Example Usage After Implementation

```construct
extern fn printf(format: CStr, ...): Int
extern fn putchar(c: Char): Int
extern fn malloc(size: ULong): IntPtr

let x: UInt = 42
let c: Char = 65        // 'A'
let uc: UChar = 255
let s: Short = 1000
let d: Double = 3.14159

dump(x)
dump(c)
```

## Backward Compatibility

✅ All existing code continues to work
✅ Int, Float, Bool, String unchanged
✅ No breaking changes to parser
✅ Optional: can use new types gradually
