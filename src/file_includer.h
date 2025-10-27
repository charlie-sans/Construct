#ifndef CONSTRUCT_FILE_INCLUDER_H
#define CONSTRUCT_FILE_INCLUDER_H

#include "ast.h"
#include <string>
#include <vector>
#include <memory>

namespace construct {

/**
 * FileIncluder - Handles file inclusion directives in Construct
 * 
 * Supports:
 * - include ["file.ct"] - Include a single file
 * - include ["*.ct"] - Include all .ct files in current directory
 * - include ["lib" / "*.ct"] - Include all .ct files in lib directory
 * - include ["*"] - Include all files (with supported extensions)
 * - include ["file1.ct", "file2.ct"] - Include multiple files
 * 
 * Path Resolution:
 * - Checks include paths in order (like C's -I flag)
 * - Validates paths to prevent directory traversal attacks
 * - Supports both absolute and relative paths
 */
class FileIncluder {
public:
    FileIncluder();
    
    /**
     * Add an include path for file resolution
     * Include paths are searched in order when resolving patterns
     * @param path Directory path to add to include search path
     */
    void addIncludePath(const std::string& path);
    
    /**
     * Set the include paths (replaces existing ones)
     * @param paths Vector of directory paths
     */
    void setIncludePaths(const std::vector<std::string>& paths);
    
    /**
     * Get current include paths
     * @return Vector of include paths
     */
    std::vector<std::string> getIncludePaths() const;
    
    /**
     * Resolve include paths and load files
     * @param patterns List of file patterns (e.g., ["*.ct"], ["lib/io.ct"])
     * @param base_dir Base directory for relative paths
     * @return List of resolved file paths
     */
    std::vector<std::string> resolveFiles(
        const std::vector<std::string>& patterns,
        const std::string& base_dir = "."
    );
    
    /**
     * Read and parse an included file, returning its AST
     * @param filepath Path to the file
     * @return Program AST from the file
     */
    Program readAndParseFile(const std::string& filepath);
    
    /**
     * Merge included programs into a main program
     * @param main_program The main program
     * @param included_programs Programs to include
     * @return Merged program with all statements
     */
    static Program mergePrograms(
        const Program& main_program,
        const std::vector<Program>& included_programs
    );

private:
    /// Include search paths (like C's -I flag)
    std::vector<std::string> include_paths;
    
    /**
     * Expand wildcard patterns into concrete file paths
     * @param pattern Pattern like "*.ct" or lib path
     * @param base_dir Base directory
     * @return List of matching file paths
     */
    std::vector<std::string> expandPattern(
        const std::string& pattern,
        const std::string& base_dir
    );
    
    /**
     * Resolve a file path using include paths
     * Searches include_paths in order if file not found at direct path
     * @param filepath The file path to resolve
     * @return The resolved absolute path
     * @throws std::runtime_error if file not found
     */
    std::string resolveFilePath(const std::string& filepath);
    
    /**
     * Check if a file exists and is readable
     */
    bool fileExists(const std::string& filepath);
    
    /**
     * Get file extension
     */
    std::string getExtension(const std::string& filepath);
    
    /**
     * Validate path to prevent directory traversal attacks
     * @param path Path to validate
     * @return true if path is safe
     */
    bool isPathSafe(const std::string& path);
    
    /**
     * Normalize a file path (resolve . and .. components)
     * @param path Path to normalize
     * @return Normalized path
     */
    std::string normalizePath(const std::string& path);
};

}  // namespace construct

#endif  // CONSTRUCT_FILE_INCLUDER_H
