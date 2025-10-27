/// Construct Standard Library
/// This header declares all runtime support functions available to Construct programs
///
/// These functions are linked into compiled Construct binaries and provide
/// I/O, type conversion, string handling, math, and other core functionality.

#ifndef CONSTRUCT_STDLIB_H
#define CONSTRUCT_STDLIB_H

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Output Functions - Basic (dump)
// ============================================================================

/// Dump an integer value to stdout without newline
void construct_dump_int(int32_t value);

/// Dump a floating-point value to stdout without newline
void construct_dump_float(double value);

/// Dump a boolean value to stdout (prints "true" or "false")
void construct_dump_bool(int8_t value);

/// Dump a string to stdout without newline
void construct_dump_string(const char* str);

// ============================================================================
// Output Functions - Enhanced (dump_line)
// ============================================================================

/// Print a newline character
void construct_newline(void);

/// Alias for newline
void construct_nl(void);

/// Dump integer with newline
void construct_dump_int_line(int32_t value);

/// Dump float with newline
void construct_dump_float_line(double value);

/// Dump bool with newline
void construct_dump_bool_line(int8_t value);

/// Dump string with newline
void construct_dump_string_line(const char* str);

/// Print a formatted string
int construct_print(const char* fmt, ...);

// ============================================================================
// Input Functions - Reading from stdin
// ============================================================================

/// Read an integer value from stdin
int32_t construct_read_int(void);

/// Read a floating-point value from stdin
double construct_read_float(void);

/// Read a boolean value from stdin (0 = false, 1 = true)
int8_t construct_read_bool(void);

/// Read a string value from stdin (non-whitespace)
const char* construct_read_string(void);

/// Read a line of text from stdin (including whitespace)
const char* construct_read_line(void);

/// Read a line with a prompt displayed first
const char* construct_read_line_prompt(const char* prompt);

/// input(prompt) - Read a line with a prompt (alias for read_line_prompt)
const char* construct_input(const char* prompt);

// ============================================================================
// Type Conversion Functions - to String
// ============================================================================

/// Convert int to string (returns pointer to static buffer)
const char* construct_int_to_string(int32_t value);

/// Convert float to string (returns pointer to static buffer)
const char* construct_float_to_string(double value);

/// Convert bool to string (returns "true" or "false")
const char* construct_bool_to_string(int8_t value);

// ============================================================================
// String Functions
// ============================================================================

/// Get length of a string
int32_t construct_string_length(const char* str);

/// Concatenate two strings (returns pointer to static buffer)
const char* construct_string_concat(const char* a, const char* b);

/// Compare two strings for equality (returns 1 if equal, 0 if different)
int8_t construct_string_equals(const char* a, const char* b);

/// Convert string to uppercase (returns pointer to static buffer)
const char* construct_string_upper(const char* str);

/// Convert string to lowercase (returns pointer to static buffer)
const char* construct_string_lower(const char* str);

/// Trim whitespace from both ends (returns pointer to static buffer)
const char* construct_string_trim(const char* str);

/// Check if string starts with prefix (returns 1 if true, 0 if false)
int8_t construct_string_starts_with(const char* str, const char* prefix);

/// Check if string ends with suffix (returns 1 if true, 0 if false)
int8_t construct_string_ends_with(const char* str, const char* suffix);

// ============================================================================
// Type Conversion Functions - from String
// ============================================================================

/// Convert string to int (returns parsed value, 0 on error)
int32_t construct_string_to_int(const char* str);

/// Convert string to float (returns parsed value, 0.0 on error)
double construct_string_to_float(const char* str);

/// Convert string to bool (returns 1 for "true", 0 otherwise)
int8_t construct_string_to_bool(const char* str);

// ============================================================================
// Math Functions
// ============================================================================

/// Get absolute value of an integer
int32_t construct_abs_int(int32_t x);

/// Get absolute value of a float
double construct_abs_float(double x);

/// Get maximum of two integers
int32_t construct_max_int(int32_t a, int32_t b);

/// Get maximum of two floats
double construct_max_float(double a, double b);

/// Get minimum of two integers
int32_t construct_min_int(int32_t a, int32_t b);

/// Get minimum of two floats
double construct_min_float(double a, double b);

/// Clamp integer value between min and max
int32_t construct_clamp_int(int32_t val, int32_t min_val, int32_t max_val);

/// Clamp float value between min and max
double construct_clamp_float(double val, double min_val, double max_val);

/// Round a float to nearest integer
double construct_round(double x);

/// Floor function (rounds down)
double construct_floor(double x);

/// Ceiling function (rounds up)
double construct_ceil(double x);

// ============================================================================
// Utility Functions
// ============================================================================

/// Sleep for milliseconds
void construct_sleep_ms(int32_t milliseconds);

/// Sleep for seconds
void construct_sleep(int32_t seconds);

/// Clear the screen
void construct_clear_screen(void);

/// Exit program with status code
void construct_exit(int32_t code);

/// Print an error message to stderr
void construct_print_error(const char* msg);

/// Debug output (prints to stderr)
void construct_debug(const char* msg);

// ============================================================================
// New I/O Functions - show() / input() style (variadic support)
// ============================================================================

/// Show integer value (without newline)
void construct_show_int(int32_t value);

/// Show float value (without newline)
void construct_show_float(double value);

/// Show bool value (without newline)
void construct_show_bool(int8_t value);

/// Show string value (without newline)
void construct_show_string(const char* str);

/// Show with newline - integer
void construct_showln_int(int32_t value);

/// Show with newline - float
void construct_showln_float(double value);

/// Show with newline - bool
void construct_showln_bool(int8_t value);

/// Show with newline - string
void construct_showln_string(const char* str);

/// Show newline only
void construct_showln(void);

#ifdef __cplusplus
}
#endif

#endif // CONSTRUCT_STDLIB_H

