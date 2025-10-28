#ifndef CONSTRUCT_CLANG_COMPILER_H
#define CONSTRUCT_CLANG_COMPILER_H

#include <string>
#include <vector>

namespace construct {

/**
 * Clang-based Compilation Backend
 * Converts LLVM IR to object files, assembly, or executables
 */
class ClangCompiler {
public:
    ClangCompiler(const std::string& compiler_path = "");
    ~ClangCompiler();
    
    /**
     * Compile LLVM IR to object file
     * @param ir_code The LLVM IR code as a string
     * @param output_file Path to output .o file
     * @param optimize_level Optimization level: 0, 1, 2, 3, s, z
     * @return true if successful
     */
    bool compileToObjectFile(const std::string& ir_code, 
                           const std::string& output_file,
                           int optimize_level = 0);
    
    /**
     * Compile LLVM IR to assembly
     * @param ir_code The LLVM IR code as a string
     * @param output_file Path to output .s file
     * @param optimize_level Optimization level: 0, 1, 2, 3, s, z
     * @return true if successful
     */
    bool compileToAssembly(const std::string& ir_code,
                          const std::string& output_file,
                          int optimize_level = 0);
    
    /**
     * Compile LLVM IR to executable
     * @param ir_code The LLVM IR code as a string
     * @param output_file Path to output executable
     * @param stdlib_objects Vector of stdlib object files to link
     * @param optimize_level Optimization level: 0, 1, 2, 3, s, z
     * @param libraries Vector of libraries to link (without -l prefix)
     * @return true if successful
     */
    bool compileToExecutable(const std::string& ir_code,
                            const std::string& output_file,
                            const std::vector<std::string>& stdlib_objects = {},
                            int optimize_level = 0,
                            const std::vector<std::string>& libraries = {});
    
    /**
     * Get the last error message
     */
    std::string getLastError() const { return last_error; }
    
    /**
     * Set whether to keep temporary files
     */
    void setKeepTemps(bool keep) { keep_temps = keep; }

private:
    std::string last_error;
    bool keep_temps = false;
    std::string compiler_dir;
    
    // Helper methods
    std::string createTemporaryFile(const std::string& suffix);
    bool writeToFile(const std::string& filename, const std::string& content);
    bool executeCommand(const std::string& command);
    bool compileViaLLVM(const std::string& ir_code,
                       const std::string& output_file,
                       const std::string& output_type,
                       int optimize_level);
};

}  // namespace construct

#endif  // CONSTRUCT_CLANG_COMPILER_H
