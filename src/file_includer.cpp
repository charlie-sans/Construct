#include "file_includer.h"
#include "lexer.h"
#include "parser.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

namespace fs = std::filesystem;
using namespace construct;

FileIncluder::FileIncluder() {
    // Initialize with default include paths
    include_paths.push_back(".");
}

void FileIncluder::addIncludePath(const std::string& path) {
    if (!isPathSafe(path)) {
        throw std::runtime_error("Unsafe include path: " + path);
    }
    include_paths.push_back(path);
}

void FileIncluder::setIncludePaths(const std::vector<std::string>& paths) {
    include_paths.clear();
    for (const auto& path : paths) {
        if (!isPathSafe(path)) {
            throw std::runtime_error("Unsafe include path: " + path);
        }
        include_paths.push_back(path);
    }
}

std::vector<std::string> FileIncluder::getIncludePaths() const {
    return include_paths;
}

std::vector<std::string> FileIncluder::resolveFiles(
    const std::vector<std::string>& patterns,
    const std::string& base_dir
) {
    std::vector<std::string> result;
    
    for (const auto& pattern : patterns) {
        if (pattern == "*") {
            // Include all .ct and .cst files in base directory
            auto expanded = expandPattern("*.ct", base_dir);
            result.insert(result.end(), expanded.begin(), expanded.end());
            expanded = expandPattern("*.cst", base_dir);
            result.insert(result.end(), expanded.begin(), expanded.end());
        } else {
            // Check if pattern contains wildcards
            if (pattern.find('*') != std::string::npos || pattern.find('?') != std::string::npos) {
                auto expanded = expandPattern(pattern, base_dir);
                result.insert(result.end(), expanded.begin(), expanded.end());
            } else {
                // Direct file path - try to resolve
                try {
                    std::string resolved = resolveFilePath(pattern);
                    result.push_back(resolved);
                } catch (const std::exception& e) {
                    throw std::runtime_error("Include file not found: " + pattern + " (" + e.what() + ")");
                }
            }
        }
    }
    
    // Remove duplicates
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    
    return result;
}

std::vector<std::string> FileIncluder::expandPattern(
    const std::string& pattern,
    const std::string& base_dir
) {
    std::vector<std::string> result;
    
    try {
        // Use filesystem to match patterns
        for (const auto& entry : fs::recursive_directory_iterator(base_dir)) {
            if (entry.is_regular_file()) {
                std::string relative = fs::relative(entry.path(), base_dir).string();
                std::string filename = entry.path().filename().string();
                
                // Simple pattern matching
                bool matches = false;
                if (pattern == "*.ct" && getExtension(filename) == ".ct") {
                    matches = true;
                } else if (pattern == "*.cst" && getExtension(filename) == ".cst") {
                    matches = true;
                } else if (pattern.find("*") != std::string::npos) {
                    // Simple wildcard matching
                    size_t star_pos = pattern.find('*');
                    std::string prefix = pattern.substr(0, star_pos);
                    std::string suffix = pattern.substr(star_pos + 1);
                    
                    if (relative.find(prefix) == 0 && 
                        (suffix.empty() || relative.find(suffix) != std::string::npos)) {
                        matches = true;
                    }
                }
                
                if (matches) {
                    result.push_back(entry.path().string());
                }
            }
        }
    } catch (const std::exception& e) {
        throw std::runtime_error(std::string("Error expanding include pattern: ") + e.what());
    }
    
    return result;
}

bool FileIncluder::fileExists(const std::string& filepath) {
    return fs::exists(filepath) && fs::is_regular_file(filepath);
}

std::string FileIncluder::getExtension(const std::string& filepath) {
    size_t dot_pos = filepath.rfind('.');
    if (dot_pos == std::string::npos) {
        return "";
    }
    return filepath.substr(dot_pos);
}

Program FileIncluder::readAndParseFile(const std::string& filepath) {
    // Read file
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open file: " + filepath);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    
    // Tokenize
    Lexer lexer(source);
    auto tokens = lexer.tokenize();
    
    // Parse
    Parser parser(tokens);
    return parser.parse();
}

Program FileIncluder::mergePrograms(
    const Program& main_program,
    const std::vector<Program>& included_programs
) {
    Program result = main_program;
    
    // Prepend all included statements to the main program
    for (const auto& included : included_programs) {
        for (const auto& stmt : included.statements) {
            result.statements.insert(result.statements.begin(), stmt);
        }
    }
    
    return result;
}

std::string FileIncluder::resolveFilePath(const std::string& filepath) {
    // First try direct path
    if (fileExists(filepath)) {
        return fs::absolute(filepath).string();
    }
    
    // Try each include path
    for (const auto& include_path : include_paths) {
        fs::path candidate = fs::path(include_path) / filepath;
        if (fileExists(candidate.string())) {
            return fs::absolute(candidate).string();
        }
    }
    
    // Not found anywhere
    throw std::runtime_error("file not found in any include path");
}

bool FileIncluder::isPathSafe(const std::string& path) {
    // Normalize the path
    try {
        fs::path p = fs::absolute(path);
        std::string normalized = normalizePath(p.string());
        
        // Check for directory traversal attempts (.. components)
        if (normalized.find("..") != std::string::npos) {
            return false;
        }
        
        return true;
    } catch (...) {
        return false;
    }
}

std::string FileIncluder::normalizePath(const std::string& path) {
    try {
        return fs::absolute(fs::path(path)).lexically_normal().string();
    } catch (...) {
        return path;
    }
}
