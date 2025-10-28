#include "diagnostics.h"
#include <unistd.h>
#include <iostream>
#include <iomanip>

namespace construct {

bool TerminalStyle::color_enabled = true;

bool TerminalStyle::supportsColor() {
    // Check if stdout is a terminal
    return isatty(STDOUT_FILENO) && isatty(STDERR_FILENO);
}

void TerminalStyle::setColorEnabled(bool enabled) {
    color_enabled = enabled;
}

bool TerminalStyle::isColorEnabled() {
    return color_enabled && supportsColor();
}

void Diagnostics::info(const std::string& message) {
    print(DiagnosticLevel::INFO, message);
}

void Diagnostics::success(const std::string& message) {
    print(DiagnosticLevel::SUCCESS, message);
}

void Diagnostics::warning(const std::string& message) {
    print(DiagnosticLevel::WARNING, message);
}

void Diagnostics::error(const std::string& message) {
    print(DiagnosticLevel::ERROR, message);
}

void Diagnostics::note(const std::string& message) {
    print(DiagnosticLevel::NOTE, message);
}

void Diagnostics::step(const std::string& step_name, const std::string& description) {
    if (TerminalStyle::isColorEnabled()) {
        std::cout << TerminalStyle::BOLD << TerminalStyle::CYAN << "→"
                  << TerminalStyle::RESET << " " << TerminalStyle::BOLD
                  << step_name << TerminalStyle::RESET << ": " << description
                  << std::endl;
    } else {
        std::cout << "→ " << step_name << ": " << description << std::endl;
    }
}

void Diagnostics::stat(const std::string& name, const std::string& value) {
    if (TerminalStyle::isColorEnabled()) {
        std::cout << "  " << TerminalStyle::DIM << "•" << TerminalStyle::RESET
                  << " " << TerminalStyle::BOLD << name << TerminalStyle::RESET
                  << ": " << TerminalStyle::BRIGHT_GREEN << value
                  << TerminalStyle::RESET << std::endl;
    } else {
        std::cout << "  • " << name << ": " << value << std::endl;
    }
}

void Diagnostics::stat(const std::string& name, int value) {
    stat(name, std::to_string(value));
}

void Diagnostics::print(DiagnosticLevel level, const std::string& message) {
    const auto& color = getColor(level);
    const auto& prefix = getPrefix(level);
    
    if (TerminalStyle::isColorEnabled()) {
        std::cerr << color << TerminalStyle::BOLD << prefix
                  << TerminalStyle::RESET << " " << message << std::endl;
    } else {
        std::cerr << prefix << " " << message << std::endl;
    }
}

void Diagnostics::header(const std::string& title) {
    if (TerminalStyle::isColorEnabled()) {
        std::cout << "\n" << TerminalStyle::BOLD << TerminalStyle::BLUE
                  << "=== " << title << " ===" << TerminalStyle::RESET
                  << std::endl;
    } else {
        std::cout << "\n=== " << title << " ===" << std::endl;
    }
}

void Diagnostics::separator() {
    if (TerminalStyle::isColorEnabled()) {
        std::cout << TerminalStyle::DIM << "────────────────────────────────────"
                  << TerminalStyle::RESET << std::endl;
    } else {
        std::cout << "────────────────────────────────────" << std::endl;
    }
}

void Diagnostics::keyValue(const std::string& key, const std::string& value) {
    if (TerminalStyle::isColorEnabled()) {
        std::cout << TerminalStyle::BOLD << key << TerminalStyle::RESET << ": "
                  << TerminalStyle::BRIGHT_WHITE << value << TerminalStyle::RESET
                  << std::endl;
    } else {
        std::cout << key << ": " << value << std::endl;
    }
}

std::string Diagnostics::getPrefix(DiagnosticLevel level) {
    switch (level) {
        case DiagnosticLevel::INFO:
            return "info";
        case DiagnosticLevel::SUCCESS:
            return "✓";
        case DiagnosticLevel::WARNING:
            return "warning";
        case DiagnosticLevel::ERROR:
            return "error";
        case DiagnosticLevel::NOTE:
            return "note";
        default:
            return "info";
    }
}

std::string Diagnostics::getColor(DiagnosticLevel level) {
    switch (level) {
        case DiagnosticLevel::INFO:
            return TerminalStyle::BRIGHT_BLUE;
        case DiagnosticLevel::SUCCESS:
            return TerminalStyle::BRIGHT_GREEN;
        case DiagnosticLevel::WARNING:
            return TerminalStyle::BRIGHT_YELLOW;
        case DiagnosticLevel::ERROR:
            return TerminalStyle::BRIGHT_RED;
        case DiagnosticLevel::NOTE:
            return TerminalStyle::BRIGHT_CYAN;
        default:
            return TerminalStyle::RESET;
    }
}

ProgressIndicator::ProgressIndicator(const std::string& title, int total_steps)
    : title(title), total_steps(total_steps), current_step(0) {}

void ProgressIndicator::nextStep(const std::string& step_name) {
    completeStep(step_name, "");
}

void ProgressIndicator::completeStep(const std::string& step_name, const std::string& info) {
    current_step++;
    
    if (TerminalStyle::isColorEnabled()) {
        std::cout << TerminalStyle::BOLD << TerminalStyle::GREEN << "✓"
                  << TerminalStyle::RESET << " " << step_name;
        if (!info.empty()) {
            std::cout << " " << TerminalStyle::DIM << "(" << info << ")"
                      << TerminalStyle::RESET;
        }
        std::cout << std::endl;
    } else {
        std::cout << "✓ " << step_name;
        if (!info.empty()) {
            std::cout << " (" << info << ")";
        }
        std::cout << std::endl;
    }
    
    completed_steps.push_back(step_name);
}

void ProgressIndicator::complete() {
    if (TerminalStyle::isColorEnabled()) {
        std::cout << "\n" << TerminalStyle::BOLD << TerminalStyle::GREEN
                  << "✓ Compilation successful!" << TerminalStyle::RESET
                  << std::endl;
    } else {
        std::cout << "\n✓ Compilation successful!" << std::endl;
    }
}

void ProgressIndicator::failed(const std::string& reason) {
    if (TerminalStyle::isColorEnabled()) {
        std::cerr << "\n" << TerminalStyle::BOLD << TerminalStyle::RED
                  << "✗ Compilation failed" << TerminalStyle::RESET;
        if (!reason.empty()) {
            std::cerr << ": " << reason;
        }
        std::cerr << std::endl;
    } else {
        std::cerr << "\n✗ Compilation failed";
        if (!reason.empty()) {
            std::cerr << ": " << reason;
        }
        std::cerr << std::endl;
    }
}

}  // namespace construct
