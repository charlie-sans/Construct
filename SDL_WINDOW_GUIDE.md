# SDL Window Demo - Implementation Guide

## Overview

This guide shows how to create an SDL window in Construct using the `extern fn` feature to call C library functions.

## Quick Start

### Simplest Possible Example

```construct
extern fn SDL_Init(flags: UInt) -> Int
extern fn SDL_CreateWindow(title: CStr, x: Int, y: Int, w: Int, h: Int, flags: Int) -> IntPtr
extern fn SDL_Quit()

let SDL_INIT_VIDEO = 0x00000020

SDL_Init(SDL_INIT_VIDEO)
let window = SDL_CreateWindow("My Window", 100, 100, 800, 600, 0)
SDL_Quit()
dump("Window created!")
```

## Key Patterns

### 1. SDL Constants

Define SDL constants as top-level `let` bindings:

```construct
let SDL_INIT_VIDEO = 0x00000020    // Video subsystem
let SDL_QUIT = 12                  // Quit event code
let SDL_KEYDOWN = 2                // Key press event
```

### 2. External Functions

Declare C library functions with `extern fn`:

```construct
extern fn SDL_Init(flags: UInt) -> Int
extern fn SDL_CreateWindow(
    title: CStr,    // Window title (C string)
    x: Int,         // X position
    y: Int,         // Y position
    w: Int,         // Width
    h: Int,         // Height
    flags: Int      // Flags (usually 0)
) -> IntPtr         // Returns window pointer
```

### 3. Error Checking

Check if window creation succeeded:

```construct
let window = SDL_CreateWindow("Title", 100, 100, 800, 600, 0)

if window != 0 then
    dump("Success!")
else
    dump("Failed!")
end
```

### 4. Event Handling (Current Workaround)

Since direct field access on C structs isn't supported yet, use helper functions:

```construct
// Define a helper function (or declare as extern if in C library)
extern fn get_event_type(event: IntPtr) -> Int

// Use it in your code:
let event_ptr = 0
let event_type = get_event_type(event_ptr)

if event_type == SDL_QUIT then
    dump("User quit!")
end
```

### 5. Mutable State for Game Loops

Use `mut` keyword for game state:

```construct
let mut running = true
let mut frame_count = 0

while running do
    frame_count = frame_count + 1
    
    // Handle events...
    
    if should_quit then
        running = false
    end
end

dump("Frames rendered: ")
dump(frame_count)
```

## Full Event Loop Example

```construct
// SDL Setup
let SDL_INIT_VIDEO = 0x00000020
let SDL_QUIT = 12

extern fn SDL_Init(flags: UInt) -> Int
extern fn SDL_CreateWindow(title: CStr, x: Int, y: Int, w: Int, h: Int, flags: Int) -> IntPtr
extern fn SDL_DestroyWindow(window: IntPtr)
extern fn SDL_PollEvent(event: IntPtr) -> Int
extern fn SDL_Delay(ms: UInt)
extern fn SDL_Quit()
extern fn get_event_type(event: IntPtr) -> Int

// Initialize
SDL_Init(SDL_INIT_VIDEO)
let window = SDL_CreateWindow("Game Loop", 100, 100, 800, 600, 0)

// Event loop
let mut running = true
let mut frame_count = 0

while running do
    // Poll events (simplified - normally you'd allocate SDL_Event)
    let has_event = SDL_PollEvent(0)
    
    if has_event != 0 then
        let event_type = get_event_type(0)
        if event_type == SDL_QUIT then
            running = false
        end
    end
    
    // Update and render would go here
    frame_count = frame_count + 1
    
    // Cap frame rate
    SDL_Delay(16)  // ~60 FPS
end

// Cleanup
SDL_DestroyWindow(window)
SDL_Quit()
dump("Rendered frames: ")
dump(frame_count)
```

## Common Issues & Solutions

### Issue 1: "Unknown identifier: CStr"

**Solution**: `CStr` is the C string type in Construct. Pass string literals:
```construct
extern fn SDL_CreateWindow(title: CStr, ...) -> IntPtr

let window = SDL_CreateWindow("My Title", 100, 100, 800, 600, 0)  // ✅ Works
```

### Issue 2: "Window didn't create"

**Solution**: Check return value:
```construct
let window = SDL_CreateWindow(...)

if window == 0 then
    dump("Window creation failed")
else
    dump("Success!")
end
```

### Issue 3: "Event access fails with Unknown method error"

**Solution**: Use helper functions until field access is implemented:
```construct
// Don't do this (will fail):
// let type = event.type  ❌

// Do this instead:
extern fn get_event_type(event: IntPtr) -> Int
let type = get_event_type(event)  // ✅
```

### Issue 4: Mutable variable assignment error

**Solution**: Declare variables with `mut` keyword:
```construct
let mut counter = 0
counter = counter + 1  // ✅ Works
```

## Files in This Session

1. **sdl_simple_window.ct** - Minimal working example (20 lines)
2. **sdl_window_demo.ct** - Full event loop example (80 lines)
3. **sdl_field_access_test.ct** - Keyword field names test

## Next Steps

1. **Compile**: `./builddir/construct examples/sdl_simple_window.ct`
2. **Extend**: Add event handling, drawing, etc.
3. **Optimize**: Add frame rate control, input handling
4. **Debug**: Use `dump()` for printing values

## Type Reference

| Construct Type | C Type | Use For |
|---|---|---|
| `Int` | `int32_t` | Window size, positions, event types |
| `UInt` | `uint32_t` | SDL flags, timer values |
| `IntPtr` | `void*` | Opaque pointers (window, surface) |
| `CStr` | `const char*` | String literals for function arguments |

## Linking

The compiler automatically links against SDL2 when you use `extern fn` declarations for SDL functions.

To compile:
```bash
./builddir/construct examples/sdl_window_demo.ct -o output.ll
```

## Future Improvements

Once codegen field extraction is implemented:

```construct
// This will work (currently needs helper function):
if event.type == SDL_QUIT then
    ...
end
```

Until then, use the helper function pattern shown above.

---

**Ready to learn more?** Check out the other SDL examples or the main documentation in `docs/` folder.
