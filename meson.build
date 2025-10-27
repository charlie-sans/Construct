
project('construct', 'cpp', 'c', version: '0.1.0')

# Compiler options
add_project_arguments('-std=c++17', language: 'cpp')
add_project_arguments('-Wall', '-Wextra', language: 'cpp')
add_project_arguments('-fno-rtti', language: 'cpp')  # LLVM doesn't use RTTI by default

# Find LLVM
llvm_dep = dependency('llvm', version: '>=10', method: 'config-tool', required: true)

# Include directories
include_dirs = include_directories('src')

# Build stdlib as shared library
stdlib_lib = shared_library('construct_stdlib',
    'src/srdlib/Stream.cpp',
    install: true,
)

# Source files (compiler executable - excludes stdlib)
sources = [
    'src/main.cpp',
    'src/lexer.cpp',
    'src/parser.cpp',
    'src/compiler.cpp',
    'src/llvm_codegen.cpp',
    'src/clang_compiler.cpp',
]

# Header files (for dependency tracking)
headers = [
    'src/ast.h',
    'src/lexer.h',
    'src/parser.h',
    'src/compiler.h',
    'src/llvm_codegen.h',
    'src/clang_compiler.h',
    'src/srdlib/stdlib.h',
]

# Build executable - links against stdlib
executable('construct',
    sources,
    include_directories: include_dirs,
    dependencies: [llvm_dep],
    link_with: stdlib_lib,
    install: true,
)
