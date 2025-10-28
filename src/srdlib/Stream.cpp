// Stream I/O functions for Construct language
// Provides dump(), print, input operations, and utilities
//
// Usage in Construct:
//   dump(42)                    # prints: 42
//   dump(3.14)                  # prints: 3.14
//   dump(true)                  # prints: true
//   dump("hello")               # prints: hello
//   dump_line(x)                # prints with newline
//   read_int()                  # read integer from stdin
//   read_string()               # read string from stdin

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
#include <unistd.h>

// ============================================================================
// Output Functions - Basic
// ============================================================================

/// Dump an integer value to stdout without newline
extern "C" void construct_dump_int(int32_t value) {
    printf("%d", value);
}

/// Dump a floating-point value to stdout without newline
extern "C" void construct_dump_float(double value) {
    printf("%g", value);
}

/// Dump a boolean value to stdout (prints "true" or "false")
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
// Output Functions - Enhanced
// ============================================================================

/// Print a newline character
extern "C" void construct_newline(void) {
    printf("\n");
}

/// Alias for newline
extern "C" void construct_nl(void) {
    printf("\n");
}

/// Dump integer with newline
extern "C" void construct_dump_int_line(int32_t value) {
    printf("%d\n", value);
}

/// Dump float with newline
extern "C" void construct_dump_float_line(double value) {
    printf("%g\n", value);
}

/// Dump bool with newline
extern "C" void construct_dump_bool_line(int8_t value) {
    printf("%s\n", value ? "true" : "false");
}

/// Dump string with newline
extern "C" void construct_dump_string_line(const char* str) {
    if (str) {
        printf("%s\n", str);
    } else {
        printf("\n");
    }
}

/// Print a formatted string (printf-style)
extern "C" int construct_print(const char* fmt, ...) {
    if (fmt) {
        return printf("%s", fmt);
    }
    return 0;
}

// ============================================================================
// Input Functions - Reading from stdin
// ============================================================================

/// Read an integer value from stdin
extern "C" int32_t construct_read_int(void) {
    int32_t value = 0;
    scanf("%d", &value);
    // Clear remaining newline from input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    return value;
}

/// Read a floating-point value from stdin
extern "C" double construct_read_float(void) {
    double value = 0.0;
    scanf("%lf", &value);
    // Clear remaining newline from input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    return value;
}

/// Read a boolean value from stdin (0 = false, 1 = true)
extern "C" int8_t construct_read_bool(void) {
    int8_t value = 0;
    scanf("%hhd", &value);
    // Clear remaining newline from input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    return value;
}

/// Read a string value from stdin (non-whitespace)
extern "C" const char* construct_read_string(void) {
    static char buffer[512];
    scanf("%511s", buffer);
    // Clear remaining newline from input buffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    return buffer;
}

/// Read a line of text from stdin (including whitespace)
extern "C" const char* construct_read_line(void) {
    static char buffer[1024];
    if (fgets(buffer, sizeof(buffer), stdin)) {
        // Remove trailing newline if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        
        // Allocate new memory for the string to avoid static buffer issues
        char* result = (char*)malloc(len + 1);
        if (result) {
            strcpy(result, buffer);
            return result;
        }
    }
    return "";
}

/// Read a line with a prompt displayed first
extern "C" const char* construct_read_line_prompt(const char* prompt) {
    if (prompt) {
        printf("%s", prompt);
        fflush(stdout);
    }
    return construct_read_line();
}

/// input(prompt) - Read a line with a prompt (alias for read_line_prompt)
extern "C" const char* construct_input(const char* prompt) {
    return construct_read_line_prompt(prompt);
}

/// Deprecated: older readline function
extern "C" const char* readline(const char* prompt) {
    return construct_read_line_prompt(prompt);
}

/// Helper for formatted dumping
extern "C" void fdump(const char* format, const char* str) {
    if (format && str) {
        printf(format, str);
    }
}

/// Helper for dumping string with newline
extern "C" void fdumpn(const char* str) {
    if (str) {
        printf("%s\n", str);
    } else {
        printf("\n");
    }
}

// ============================================================================
// Type Conversion Functions - to String
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

/// Convert bool to string (returns "true" or "false")
extern "C" const char* construct_bool_to_string(int8_t value) {
    return value ? "true" : "false";
}

// ============================================================================
// String Functions
// ============================================================================

/// Get length of a string
extern "C" int32_t construct_string_length(const char* str) {
    if (!str) return 0;
    int32_t len = 0;
    while (str[len] != '\0') ++len;
    return len;
}

/// Concatenate two strings (returns pointer to static buffer)
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

/// Compare two strings for equality (returns 1 if equal, 0 otherwise)
extern "C" int8_t construct_string_equals(const char* a, const char* b) {
    if (!a || !b) return (a == b) ? 1 : 0;
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        ++i;
    }
    return a[i] == b[i] ? 1 : 0;
}

/// Convert string to uppercase (returns pointer to static buffer)
extern "C" const char* construct_string_upper(const char* str) {
    static char buf[1024];
    if (!str) {
        buf[0] = '\0';
        return buf;
    }
    int i = 0;
    while (str[i] && i < 1023) {
        buf[i] = toupper((unsigned char)str[i]);
        ++i;
    }
    buf[i] = '\0';
    return buf;
}

/// Convert string to lowercase (returns pointer to static buffer)
extern "C" const char* construct_string_lower(const char* str) {
    static char buf[1024];
    if (!str) {
        buf[0] = '\0';
        return buf;
    }
    int i = 0;
    while (str[i] && i < 1023) {
        buf[i] = tolower((unsigned char)str[i]);
        ++i;
    }
    buf[i] = '\0';
    return buf;
}

/// Trim whitespace from both ends of string (returns pointer to static buffer)
extern "C" const char* construct_string_trim(const char* str) {
    if (!str) return "";
    
    // Find first non-whitespace
    int start = 0;
    while (str[start] && isspace((unsigned char)str[start])) ++start;
    
    // Find last non-whitespace
    int end = 0;
    while (str[end] != '\0') ++end;
    --end;
    while (end >= start && isspace((unsigned char)str[end])) --end;
    
    // Copy to buffer
    static char buf[1024];
    int len = end - start + 1;
    if (len <= 0) {
        buf[0] = '\0';
        return buf;
    }
    if (len > 1023) len = 1023;
    
    for (int i = 0; i < len; ++i) {
        buf[i] = str[start + i];
    }
    buf[len] = '\0';
    return buf;
}

/// Check if string starts with prefix (returns 1 if true, 0 if false)
extern "C" int8_t construct_string_starts_with(const char* str, const char* prefix) {
    if (!str || !prefix) return 0;
    while (*prefix) {
        if (*str != *prefix) return 0;
        ++str;
        ++prefix;
    }
    return 1;
}

/// Check if string ends with suffix (returns 1 if true, 0 if false)
extern "C" int8_t construct_string_ends_with(const char* str, const char* suffix) {
    if (!str || !suffix) return 0;
    
    int str_len = 0;
    while (str[str_len] != '\0') ++str_len;
    
    int suffix_len = 0;
    while (suffix[suffix_len] != '\0') ++suffix_len;
    
    if (suffix_len > str_len) return 0;
    
    int start = str_len - suffix_len;
    for (int i = 0; i < suffix_len; ++i) {
        if (str[start + i] != suffix[i]) return 0;
    }
    return 1;
}

// ============================================================================
// Type Conversion Functions - from String
// ============================================================================

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
    return construct_string_equals(str, "true") ? 1 : 0;
}

// ============================================================================
// Math Functions
// ============================================================================

/// Get absolute value of an integer
extern "C" int32_t construct_abs_int(int32_t x) {
    return x < 0 ? -x : x;
}

/// Get absolute value of a float
extern "C" double construct_abs_float(double x) {
    return x < 0.0 ? -x : x;
}

/// Get maximum of two integers
extern "C" int32_t construct_max_int(int32_t a, int32_t b) {
    return a > b ? a : b;
}

/// Get maximum of two floats
extern "C" double construct_max_float(double a, double b) {
    return a > b ? a : b;
}

/// Get minimum of two integers
extern "C" int32_t construct_min_int(int32_t a, int32_t b) {
    return a < b ? a : b;
}

/// Get minimum of two floats
extern "C" double construct_min_float(double a, double b) {
    return a < b ? a : b;
}

/// Clamp integer value between min and max
extern "C" int32_t construct_clamp_int(int32_t val, int32_t min_val, int32_t max_val) {
    if (val < min_val) return min_val;
    if (val > max_val) return max_val;
    return val;
}

/// Clamp float value between min and max
extern "C" double construct_clamp_float(double val, double min_val, double max_val) {
    if (val < min_val) return min_val;
    if (val > max_val) return max_val;
    return val;
}

/// Round a float to nearest integer
extern "C" double construct_round(double x) {
    return floor(x + 0.5);
}

/// Floor function
extern "C" double construct_floor(double x) {
    return floor(x);
}

/// Ceiling function
extern "C" double construct_ceil(double x) {
    return ceil(x);
}

// ============================================================================
// Utility Functions
// ============================================================================

/// Sleep for milliseconds
extern "C" void construct_sleep_ms(int32_t milliseconds) {
    if (milliseconds > 0) {
        usleep(milliseconds * 1000);
    }
}

/// Sleep for seconds
extern "C" void construct_sleep(int32_t seconds) {
    if (seconds > 0) {
        sleep(seconds);
    }
}

/// Clear the screen
extern "C" void construct_clear_screen(void) {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

/// Exit program with status code
extern "C" void construct_exit(int32_t code) {
    exit(code);
}

/// Print an error message to stderr
extern "C" void construct_print_error(const char* msg) {
    if (msg) {
        fprintf(stderr, "%s\n", msg);
    }
}

/// Debug output (prints to stderr)
extern "C" void construct_debug(const char* msg) {
    if (msg) {
        fprintf(stderr, "[DEBUG] %s\n", msg);
    }
}

// ============================================================================
// New I/O Functions - show() / input() style
// ============================================================================

extern "C" const char* input(const char* prompt) {
    return construct_read_line_prompt(prompt);
}

/// Show integer value (without newline)
extern "C" void construct_show_int(int32_t value) {
    printf("%d", value);
}

/// Show float value (without newline)
extern "C" void construct_show_float(double value) {
    printf("%g", value);
}

/// Show bool value (without newline)
extern "C" void construct_show_bool(int8_t value) {
    printf("%s", value ? "true" : "false");
}

/// Show string value (without newline)
extern "C" void construct_show_string(const char* str) {
    if (str) {
        printf("%s", str);
    }
}

/// Show with newline - integer
extern "C" void construct_showln_int(int32_t value) {
    printf("%d\n", value);
}

/// Show with newline - float
extern "C" void construct_showln_float(double value) {
    printf("%g\n", value);
}

/// Show with newline - bool
extern "C" void construct_showln_bool(int8_t value) {
    printf("%s\n", value ? "true" : "false");
}

/// Show with newline - string
extern "C" void construct_showln_string(const char* str) {
    if (str) {
        printf("%s\n", str);
    } else {
        printf("\n");
    }
}

/// Show newline only
extern "C" void construct_showln(void) {
    printf("\n");
}

/// Convert Construct string to C string
/// Since Construct strings are already C strings (char*), this is a no-op
/// but exists for FFI type safety and clarity
extern "C" const char* construct_string_to_cstr(const char* str) {
    return str;
}

