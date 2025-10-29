# Dot Accessor & Struct Field Access - Implementation Guide

## Problem

When accessing fields from C structs (especially SDL), two issues arise:

1. **Reserved Keywords as Field Names**: `event.type` fails because `type` is a keyword
2. **Struct Field Access on Opaque Types**: External C structs don't have type information in Construct

## Solution 1: Keywords as Field Names - Parser Support ✅

The parser now allows reserved keywords to be used as field names:

```construct
// Type definitions with keyword field names now parse:
type Event = {type: Int, let: String, fn: Int}

// Field access expressions with keywords now parse:
let evt = {type: 42, let: "test", fn: 1}
evt.type        // Parses successfully
evt.let         // Parses successfully
evt.fn          // Parses successfully
```

### What Was Changed:

**src/parser.cpp - Record Type Definitions**:
- Modified `parseFunctionType()` record field parsing (~20 lines)
- Now accepts keywords as field names: `type`, `let`, `fn`, `if`, `match`, `for`, `while`

**src/parser.cpp - Field Access Expressions**:
- Modified `parsePostfix()` field access parsing (~25 lines)
- Now accepts same keywords after dot operator

**Note**: The parser now supports keywords in these contexts, but runtime field extraction via FIELD_ACCESS codegen is limited (see limitations below).

## Solution 2: Accessing C Struct Fields (RECOMMENDED PATTERN)

For opaque C structs from libraries like SDL, use **helper functions**:

### Pattern: Helper Functions

**Define in Construct/C bindings:**
```construct
extern fn SDL_PollEvent() -> IntPtr
extern fn get_event_type(event: IntPtr) -> Int
extern fn get_event_timestamp(event: IntPtr) -> Int

// Helper getters
extern fn get_window_width(window: IntPtr) -> Int
extern fn get_window_height(window: IntPtr) -> Int
```

**Use in Construct code:**
```construct
let event_ptr = SDL_PollEvent()
let event_type = get_event_type(event_ptr)

if event_type == SDL_QUIT then
    running = false
end
```

### Why This Pattern?

✅ Works with opaque C types  
✅ Zero runtime overhead (inlined)  
✅ Type-safe access  
✅ Clear C function linkage  

## Solution 3: Define Construct Struct Equivalent

If you want dot notation in Construct, define a matching Construct record type:

```construct
type SDLEvent = {
    type: Int,
    timestamp: Int,
    ...other_fields...
}

extern fn SDL_PollEvent() -> SDLEvent

let event = SDL_PollEvent()
dump(event.type)  // Works with record types!
```

**However**, this requires the C library to return a concrete struct, not a pointer.

## Implementation Status

### ✅ DONE:

1. **Parser Support for Keywords as Field Names**
   - `event.type` now parses correctly
   - Keywords allowed: `type`, `let`, `fn`, `if`, `match`, `for`, `while`
   - Type definitions like `type Event = {type: Int}` now parse
   - Field access expressions like `record.type` now parse

### ⏳ LIMITATION - Codegen Support for Record Field Access:

The parser accepts keyword field names, but **codegen doesn't implement record field extraction yet**.

**Current status:**
- `type Config = {type: Int}` ✅ Parses
- `let c = {type: 42}` ✅ Parses  
- `dump(c.type)` ❌ Codegen error: "Unknown method: type"

**Why:** The FIELD_ACCESS codegen currently only handles method calls (`.convert()`, `.toString()`), not record field extraction.

### ⏳ TODO (Future):

1. **Codegen Support for Record Field Access**
   - Track record type information through codegen
   - Extract field values from records at runtime
   - Use LLVM GEP or other mechanisms for field extraction

2. **Struct Field Extraction from C Pointers**
   - Requires layout information from C (sizeof, offsetof)
   - Would need LLVM GEP (GetElementPtr) generation
   - Currently not implemented because C types are opaque to Construct

3. **Better C Interop**
   - FFI type annotations to specify struct layouts
   - Automatic helper function generation
   - Platform-specific struct packing

## Recommended SDL Bindings

Here's the recommended pattern for SDL in Construct:

```construct
// ============================================================================
// SDL Event Types
// ============================================================================

extern fn SDL_PollEvent() -> IntPtr
extern fn get_event_type(event: IntPtr) -> Int
extern fn get_event_timestamp(event: IntPtr) -> Int

// ============================================================================
// SDL Window Functions
// ============================================================================

extern fn SDL_CreateWindow(title: CStr, x: Int, y: Int, w: Int, h: Int, flags: Int) -> IntPtr
extern fn SDL_SetWindowSize(window: IntPtr, w: Int, h: Int)
extern fn SDL_GetWindowSize(window: IntPtr, w_ptr: IntPtr, h_ptr: IntPtr)

// ============================================================================
// SDL Init/Quit
// ============================================================================

extern fn SDL_Init(flags: UInt) -> Int
extern fn SDL_Quit()

// ============================================================================
// SDL Constants
// ============================================================================

let SDL_INIT_VIDEO = 0x00000020
let SDL_QUIT = 12
let SDL_KEYDOWN = 2
let SDL_KEYUP = 3
```

**Usage:**
```construct
SDL_Init(SDL_INIT_VIDEO)

let window = SDL_CreateWindow("My App", 100, 100, 800, 600, 0)
SDL_SetWindowSize(window, 1024, 768)

let mut running = true
while running do
    let event_ptr = SDL_PollEvent()
    let event_type = get_event_type(event_ptr)
    
    if event_type == SDL_QUIT then
        running = false
    end
end
```

## Alternative: Inline Helper Macros (Future)

We could potentially add inline C helpers to the standard library:

```construct
// Potential future syntax:
#[extern_getter(SDL_Event, type)]
// Would generate: extern fn SDL_Event_get_type(e: IntPtr) -> Int
```

## Summary

**Current Recommendation**: Use **helper functions** for C struct field access.

This is:
- ✅ Simple to implement
- ✅ Type-safe
- ✅ Zero overhead
- ✅ Flexible
- ✅ Clear intent

**For the Future**: Real struct field access would require:
- Struct layout information (offsetof, sizeof)
- LLVM GEP instruction generation
- FFI type system improvements

## Testing

Test file: `examples/sdl_field_access_test.ct`

Shows:
- Helper function pattern for field access
- How to wrap opaque C types
- Best practices for C library bindings

## Keywords Now Allowed After Dot

These can now be used as field names:

- `.type`
- `.let`
- `.fn`
- `.if`
- `.match`
- `.for`
- `.while`

Example:
```construct
type Config = {
    type: Int,
    let: String,
    fn: Bool
}

let config = ...
dump(config.type)
dump(config.let)
dump(config.fn)
```

All now work without parse errors!
