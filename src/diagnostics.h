#ifndef CONSTRUCT_DIAGNOSTICS_H
#define CONSTRUCT_DIAGNOSTICS_H

#include <string>
#include <iostream>
#include <vector>

namespace construct {

/**
 * ANSI Color and Style Codes for Terminal Output
 * Similar to Rust's rustc compiler diagnostics
 */
class TerminalStyle {
public:
    // ANSI color codes
    static constexpr const char* RESET = "\033[0m";
    static constexpr const char* BOLD = "\033[1m";
    static constexpr const char* DIM = "\033[2m";
    static constexpr const char* UNDERLINE = "\033[4m";
    
    // Foreground colors
    static constexpr const char* BLACK = "\033[30m";
    static constexpr const char* RED = "\033[31m";
    static constexpr const char* GREEN = "\033[32m";
    static constexpr const char* YELLOW = "\033[33m";
    static constexpr const char* BLUE = "\033[34m";
    static constexpr const char* MAGENTA = "\033[35m";
    static constexpr const char* CYAN = "\033[36m";
    static constexpr const char* WHITE = "\033[37m";
    
    // Bright foreground colors
    static constexpr const char* BRIGHT_BLACK = "\033[90m";
    static constexpr const char* BRIGHT_RED = "\033[91m";
    static constexpr const char* BRIGHT_GREEN = "\033[92m";
    static constexpr const char* BRIGHT_YELLOW = "\033[93m";
    static constexpr const char* BRIGHT_BLUE = "\033[94m";
    static constexpr const char* BRIGHT_MAGENTA = "\033[95m";
    static constexpr const char* BRIGHT_CYAN = "\033[96m";
    static constexpr const char* BRIGHT_WHITE = "\033[97m";
    
    /**
     * Detect if stdout is a terminal that supports colors
     */
    static bool supportsColor();
    
    /**
     * Enable or disable color output globally
     */
    static void setColorEnabled(bool enabled);
    static bool isColorEnabled();
    
private:
    static bool color_enabled;
};

/**
 * Diagnostic Level and formatting
 */
enum class DiagnosticLevel {
    INFO,
    SUCCESS,
    WARNING,
    ERROR,
    NOTE
};

/**
 * Pretty-print compiler status messages with colors
 */
class Diagnostics {
public:
    /**
     * Print an info message
     * Format: [info] message
     */
    static void info(const std::string& message);
    
    /**
     * Print a success message (green checkmark)
     * Format: ✓ message
     */
    static void success(const std::string& message);
    
    /**
     * Print a warning message
     * Format: warning: message
     */
    static void warning(const std::string& message);
    
    /**
     * Print an error message
     * Format: error: message
     */
    static void error(const std::string& message);
    
    /**
     * Print a note message
     * Format: note: message
     */
    static void note(const std::string& message);
    
    /**
     * Print a status step with description
     * Format: [step] description (used for compilation phases)
     */
    static void step(const std::string& step_name, const std::string& description);
    
    /**
     * Print a stat with value
     * Format: "  • statistic: value" (indented)
     */
    static void stat(const std::string& name, const std::string& value);
    static void stat(const std::string& name, int value);
    
    /**
     * Print a generic diagnostic with custom level and formatting
     */
    static void print(DiagnosticLevel level, const std::string& message);
    
    /**
     * Print a section header
     * Format: === Header Name ===
     */
    static void header(const std::string& title);
    
    /**
     * Print a separator line
     */
    static void separator();
    
    /**
     * Print a key-value pair with nice formatting
     */
    static void keyValue(const std::string& key, const std::string& value);
    
private:
    static std::string getPrefix(DiagnosticLevel level);
    static std::string getColor(DiagnosticLevel level);
};

/**
 * Progress indicator for multi-step compilations
 */
class ProgressIndicator {
public:
    ProgressIndicator(const std::string& title, int total_steps = 0);
    
    /**
     * Mark current step as complete
     */
    void nextStep(const std::string& step_name);
    
    /**
     * Mark a step as complete with additional info
     */
    void completeStep(const std::string& step_name, const std::string& info = "");
    
    /**
     * Mark the entire process as complete
     */
    void complete();
    
    /**
     * Mark the process as failed
     */
    void failed(const std::string& reason = "");
    
private:
    std::string title;
    int total_steps;
    int current_step;
    std::vector<std::string> completed_steps;
};

}  // namespace construct

#endif // CONSTRUCT_DIAGNOSTICS_H
