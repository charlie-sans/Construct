/// Construct Standard Library
/// This header declares all runtime support functions available to Construct programs
///
/// These functions are linked into compiled Construct binaries and provide
/// I/O, type conversion, and other core functionality.

#ifndef CONSTRUCT_STDLIB_H
#define CONSTRUCT_STDLIB_H

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// Output Functions (Stream I/O)
// ============================================================================

/// Dump an integer value to stdout without newline
void construct_dump_int(int32_t value);

/// Dump a floating-point value to stdout without newline
void construct_dump_float(double value);

/// Dump a boolean value to stdout (prints "true" or "false")
void construct_dump_bool(int8_t value);

/// Dump a string to stdout without newline
void construct_dump_string(const char* str);

/// Print a newline character
void construct_newline(void);

/// Print a formatted string
int construct_print(const char* fmt, ...);

// ============================================================================
// Input Functions (Future)
// ============================================================================

// Planned:
// int32_t construct_read_int(void);
// double construct_read_float(void);
// const char* construct_read_string(void);

// ============================================================================
// Type Conversion Functions (Future)
// ============================================================================



/// Convert int to string (returns pointer to static buffer)
const char* construct_int_to_string(int32_t value);

/// Convert float to string (returns pointer to static buffer)
const char* construct_float_to_string(double value);

/// Convert bool to string (returns pointer to static buffer)
const char* construct_bool_to_string(int8_t value);

/// Get length of a string
int32_t construct_string_length(const char* str);

/// Concatenate two strings (returns pointer to static buffer)
const char* construct_string_concat(const char* a, const char* b);

/// Compare two strings for equality (returns 1 if equal, 0 otherwise)
int8_t construct_string_equals(const char* a, const char* b);

/// Convert string to int (returns parsed value, 0 on error)
int32_t construct_string_to_int(const char* str);

/// Convert string to float (returns parsed value, 0.0 on error)
double construct_string_to_float(const char* str);

/// Convert string to bool (returns 1 for "true", 0 for "false" or other)
int8_t construct_string_to_bool(const char* str);

// Future:
// int32_t construct_string_to_int(const char* str);
// double construct_string_to_float(const char* str);

#ifdef __cplusplus
}
#endif

#endif // CONSTRUCT_STDLIB_H
