// Stream I/O functions for Construct language
// Provides dump() and print operations
//
// Usage in Construct:
//   dump(42)           # prints: 42
//   dump(3.14)         # prints: 3.14
//   dump(true)         # prints: true
//   dump("hello")      # prints: hello
//   dump(x) dump("\n") # dump with newline

#include <cstdio>
#include <cstdint>

// ============================================================================
// Output Functions
// ============================================================================

/// Dump an integer value to stdout without newline
extern "C" void construct_dump_int(int32_t value) {
    printf("%d", value);
}

/// Dump a floating-point value to stdout without newline
extern "C" void construct_dump_float(double value) {
    printf("%g", value);
}

/// Dump a boolean value to stdout (0 = false, 1 = true)
/// Prints "true" or "false"
extern "C" void construct_dump_bool(int8_t value) {
    printf("%s", value ? "true" : "false");
}

/// Dump a string value to stdout without newline
extern "C" void construct_dump_string(const char* str) {
    if (str) {
        printf("%s", str);
    }
}

// ============================================================================
// Utility Functions
// ============================================================================

/// Print a newline character
extern "C" void construct_newline(void) {
    printf("\n");
}
/// @brief 
/// @brief Print a newline character
/// @param   str The string to print
extern "C" void construct_nl(void) {
    printf("\n");
}
/// @brief Print a string without newline
/// @param str The string to print
extern "C" void fdump(const char* format, const char* str) {
    if (str) {
        printf(format, str);
    }
}
/// @brief Print a string with a newline
/// @param str The string to print
extern "C" void fdumpn(const char* str) {
    if (str) {
        printf("%s\n", str);
    } else {
        printf("\n");
    }
}
/// Print formatted string (similar to printf but simplified)
extern "C" int construct_print(const char* fmt, ...) {
    if (fmt) {
        return printf("%s", fmt);
    }
    return 0;
}


// ============================================================================
// Type Conversion Functions
// ============================================================================

/// Convert int to string (returns pointer to static buffer)
extern "C" const char* construct_int_to_string(int32_t value) {
    static char buf[32];
    snprintf(buf, sizeof(buf), "%d", value);
    return buf;
}

/// Convert float to string (returns pointer to static buffer)
extern "C" const char* construct_float_to_string(double value) {
    static char buf[64];
    snprintf(buf, sizeof(buf), "%g", value);
    return buf;
}

/// Convert bool to string (returns pointer to static buffer)
extern "C" const char* construct_bool_to_string(int8_t value) {
    return value ? "true" : "false";
}

/// Get length of a string
extern "C" int32_t construct_string_length(const char* str) {
    if (!str) return 0;
    int32_t len = 0;
    while (str[len] != '\0') ++len;
    return len;
}

/// Concatenate two strings (returns pointer to static buffer)
extern "C" const char* construct_string_concat(const char* a, const char* b) {
    static char buf[512];
    if (!a) a = "";
    if (!b) b = "";
    snprintf(buf, sizeof(buf), "%s%s", a, b);
    return buf;
}

/// Compare two strings for equality (returns 1 if equal, 0 otherwise)
extern "C" int8_t construct_string_equals(const char* a, const char* b) {
    if (!a || !b) return 0;
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        ++i;
    }
    return a[i] == b[i];
}

/// Convert string to int (returns parsed value, 0 on error)
extern "C" int32_t construct_string_to_int(const char* str) {
    if (!str) return 0;
    int32_t val = 0;
    sscanf(str, "%d", &val);
    return val;
}

/// Convert string to float (returns parsed value, 0.0 on error)
extern "C" double construct_string_to_float(const char* str) {
    if (!str) return 0.0;
    double val = 0.0;
    sscanf(str, "%lf", &val);
    return val;
}

/// Convert string to bool (returns 1 for "true", 0 for "false" or other)
extern "C" int8_t construct_string_to_bool(const char* str) {
    if (!str) return 0;
    return (construct_string_equals(str, "true")) ? 1 : 0;
}

// TODO: Implement input functions
// extern "C" int32_t construct_read_int(void);
// extern "C" double construct_read_float(void);
// extern "C" const char* construct_read_string(void);

// ============================================================================
// Type Conversion Functions (Future)
// ============================================================================

// TODO: Implement conversions
// extern "C" const char* construct_int_to_string(int32_t value);
// extern "C" const char* construct_float_to_string(double value);
// extern "C" int32_t construct_string_to_int(const char* str);
// extern "C" double construct_string_to_float(const char* str);
