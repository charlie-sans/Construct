#include "clang_compiler.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/stat.h>
#include <filesystem>

namespace construct {

ClangCompiler::ClangCompiler() : keep_temps(false) {
    // Initialize compiler
}

ClangCompiler::~ClangCompiler() = default;

std::string ClangCompiler::createTemporaryFile(const std::string& suffix) {
    // Create temp file in /tmp
    std::string template_str = "/tmp/construct_XXXXXX";
    char* temp_file = strdup(template_str.c_str());
    
    int fd = mkstemp(temp_file);
    if (fd < 0) {
        last_error = "Failed to create temporary file";
        free(temp_file);
        return "";
    }
    
    close(fd);
    
    // Rename to add suffix
    std::string result = temp_file + suffix;
    std::rename(temp_file, result.c_str());
    free(temp_file);
    
    return result;
}

bool ClangCompiler::writeToFile(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        last_error = "Failed to open file for writing: " + filename;
        return false;
    }
    
    file << content;
    file.close();
    
    return true;
}

bool ClangCompiler::executeCommand(const std::string& command) {
    std::cout << "[Command] " << command << std::endl;
    int ret = system(command.c_str());
    
    if (ret != 0) {
        last_error = "Command failed with exit code: " + std::to_string(ret);
        return false;
    }
    
    return true;
}

bool ClangCompiler::compileViaLLVM(const std::string& ir_code,
                                  const std::string& output_file,
                                  const std::string& output_type,
                                  int optimize_level) {
    try {
        // Create temp IR file
        std::string ir_file = createTemporaryFile(".ll");
        if (ir_file.empty()) {
            return false;
        }
        
        if (!writeToFile(ir_file, ir_code)) {
            std::remove(ir_file.c_str());
            return false;
        }
        
        // Determine llc output type
        std::string llc_output_type;
        if (output_type == "object") {
            llc_output_type = "obj";
        } else if (output_type == "assembly") {
            llc_output_type = "asm";
        } else {
            last_error = "Unknown output type: " + output_type;
            std::remove(ir_file.c_str());
            return false;
        }
        
        // Build llc command
        std::stringstream cmd;
        cmd << "llc";
        
        // Add optimization level
        if (optimize_level > 0) {
            cmd << " -O" << optimize_level;
        }
        
        // Output options
        cmd << " -filetype=" << llc_output_type;
        cmd << " -o " << output_file;
        cmd << " " << ir_file;
        
        bool success = executeCommand(cmd.str());
        
        // Cleanup
        if (!keep_temps) {
            std::remove(ir_file.c_str());
        }
        
        return success;
        
    } catch (const std::exception& e) {
        last_error = std::string("LLVM compilation error: ") + e.what();
        return false;
    }
}

bool ClangCompiler::compileToObjectFile(const std::string& ir_code,
                                       const std::string& output_file,
                                       int optimize_level) {
    std::cout << "Compiling to object file: " << output_file << std::endl;
    return compileViaLLVM(ir_code, output_file, "object", optimize_level);
}

bool ClangCompiler::compileToAssembly(const std::string& ir_code,
                                     const std::string& output_file,
                                     int optimize_level) {
    std::cout << "Compiling to assembly: " << output_file << std::endl;
    return compileViaLLVM(ir_code, output_file, "assembly", optimize_level);
}

bool ClangCompiler::compileToExecutable(const std::string& ir_code,
                                       const std::string& output_file,
                                       const std::vector<std::string>& stdlib_objects,
                                       int optimize_level) {
    try {
        std::cout << "Compiling to executable: " << output_file << std::endl;
        
        // First compile IR to object file
        std::string obj_file = createTemporaryFile(".o");
        if (obj_file.empty()) {
            return false;
        }
        
        if (!compileToObjectFile(ir_code, obj_file, optimize_level)) {
            std::remove(obj_file.c_str());
            return false;
        }
        
        // Build linker command
        std::stringstream link_cmd;
        link_cmd << "gcc -o " << output_file << " " << obj_file;
        
        // Add stdlib objects (for backward compatibility)
        for (const auto& obj : stdlib_objects) {
            link_cmd << " " << obj;
        }
        
        // Link against construct_stdlib shared library
        link_cmd << " -lconstruct_stdlib";
        
        // Add library search paths and runtime library paths
        link_cmd << " -L./build -Lbuild";
        link_cmd << " -Wl,-rpath=./build -Wl,-rpath=/usr/local/lib";
        
        // Link with libc
        link_cmd << " -lc";
        
        bool success = executeCommand(link_cmd.str());
        
        // Cleanup
        if (!keep_temps) {
            std::remove(obj_file.c_str());
        }
        
        return success;
        
    } catch (const std::exception& e) {
        last_error = std::string("Linking error: ") + e.what();
        return false;
    }
}

}  // namespace construct
