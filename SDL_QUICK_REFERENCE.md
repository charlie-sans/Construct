# SDL Window Demo - Quick Reference

## Complete Minimal Example

```construct
// Constants
let SDL_INIT_VIDEO = 0x00000020

// External functions
extern fn SDL_Init(flags: UInt) -> Int
extern fn SDL_CreateWindow(title: CStr, x: Int, y: Int, w: Int, h: Int, flags: Int) -> IntPtr
extern fn SDL_Quit()

// Program
SDL_Init(SDL_INIT_VIDEO)
let window = SDL_CreateWindow("My Window", 100, 100, 800, 600, 0)
SDL_Quit()
dump("Done!")
```

## With Error Checking

```construct
let SDL_INIT_VIDEO = 0x00000020

extern fn SDL_Init(flags: UInt) -> Int
extern fn SDL_CreateWindow(title: CStr, x: Int, y: Int, w: Int, h: Int, flags: Int) -> IntPtr
extern fn SDL_Quit()

SDL_Init(SDL_INIT_VIDEO)
let window = SDL_CreateWindow("My Window", 100, 100, 800, 600, 0)

if window != 0 then
    dump("Window created!")
else
    dump("Failed!")
end

SDL_Quit()
```

## With Simple Event Loop

```construct
let SDL_INIT_VIDEO = 0x00000020
let SDL_QUIT = 12

extern fn SDL_Init(flags: UInt) -> Int
extern fn SDL_CreateWindow(title: CStr, x: Int, y: Int, w: Int, h: Int, flags: Int) -> IntPtr
extern fn SDL_DestroyWindow(window: IntPtr)
extern fn SDL_PollEvent(event: IntPtr) -> Int
extern fn SDL_Delay(ms: UInt)
extern fn SDL_Quit()
extern fn get_event_type(event: IntPtr) -> Int

SDL_Init(SDL_INIT_VIDEO)
let window = SDL_CreateWindow("Event Loop", 100, 100, 800, 600, 0)

let mut running = true
while running do
    let has_event = SDL_PollEvent(0)
    
    if has_event != 0 then
        let event_type = get_event_type(0)
        if event_type == SDL_QUIT then
            running = false
        end
    end
    
    SDL_Delay(10)
end

SDL_DestroyWindow(window)
SDL_Quit()
```

## Copy-Paste Functions

Just copy these declarations into your file:

```construct
// Initialize SDL with video support
extern fn SDL_Init(flags: UInt) -> Int

// Create window
// Returns: pointer to window (0 if failed)
extern fn SDL_CreateWindow(
    title: CStr,
    x: Int,
    y: Int,
    w: Int,
    h: Int,
    flags: Int
) -> IntPtr

// Destroy window
extern fn SDL_DestroyWindow(window: IntPtr)

// Poll for events
// Returns: 1 if event available, 0 otherwise
extern fn SDL_PollEvent(event: IntPtr) -> Int

// Sleep milliseconds
extern fn SDL_Delay(ms: UInt)

// Quit SDL
extern fn SDL_Quit()

// Helper: get event type (returns event.type field)
extern fn get_event_type(event: IntPtr) -> Int
```

## Common SDL Constants

```construct
// Subsystems
let SDL_INIT_VIDEO = 0x00000020      // Video support
let SDL_INIT_AUDIO = 0x00000010      // Audio support
let SDL_INIT_TIMER = 0x00000001      // Timer support

// Event types
let SDL_QUIT = 12                    // Window closed
let SDL_KEYDOWN = 2                  // Key pressed
let SDL_KEYUP = 3                    // Key released
let SDL_MOUSEMOTION = 4              // Mouse moved
let SDL_MOUSEBUTTONDOWN = 5          // Mouse clicked
let SDL_MOUSEBUTTONUP = 6            // Mouse released
```

## Key Types

| Type | Use |
|------|-----|
| `Int` | Positions, sizes, event codes |
| `UInt` | Flags, timing |
| `IntPtr` | Pointers (window, surface) |
| `CStr` | Text (window title) |

## Pattern: Mutable State

```construct
let mut counter = 0
let mut running = true

while running do
    counter = counter + 1
    
    if counter > 100 then
        running = false
    end
end

dump(counter)
```

## Pattern: Error Handling

```construct
let window = SDL_CreateWindow(...)

if window != 0 then
    // Success - do something
else
    // Failed - handle error
    dump("Error: Could not create window")
end
```

## Pattern: Event Loop Structure

```construct
let mut running = true

while running do
    // 1. Poll events
    let has_event = SDL_PollEvent(event_ptr)
    
    if has_event != 0 then
        // 2. Handle event
        let event_type = get_event_type(event_ptr)
        
        if event_type == SDL_QUIT then
            running = false
        end
    end
    
    // 3. Update (game logic)
    // ...
    
    // 4. Render (draw)
    // ...
    
    // 5. Frame rate control
    SDL_Delay(16)  // ~60 FPS
end
```

## Files Provided

1. **sdl_simple_window.ct** - Basic window, 20 lines
2. **sdl_window_demo.ct** - Full event loop, 80 lines
3. **SDL_WINDOW_GUIDE.md** - This guide

## Troubleshooting

| Problem | Solution |
|---------|----------|
| "Unknown type: CStr" | Use CStr for C strings |
| "Unknown method: type" | Use helper function `get_event_type()` |
| "Cannot assign to running" | Use `let mut running = true` |
| "Window didn't show" | Check return value with `if window != 0` |

## Next Steps

1. Copy one of the examples
2. Modify window title and size
3. Add your own logic
4. Compile: `./builddir/construct your_file.ct`
5. Extend with more SDL functions

## Pro Tips

- Use `SDL_Delay(10)` to prevent busy-waiting in loops
- Always check `window != 0` to verify creation
- Use `let mut` for any variable you'll modify
- Comment your constants (SDL values are not obvious!)
- Start simple, add features incrementally

---

Ready to build your SDL app! 🎮
