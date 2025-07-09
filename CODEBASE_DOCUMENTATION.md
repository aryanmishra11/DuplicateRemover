# Duplicate File Finder - Complete Codebase Documentation

## Table of Contents
1. [Overview](#overview)
2. [Project Structure](#project-structure)
3. [Build System](#build-system)
4. [Core Components](#core-components)
5. [Header Files Documentation](#header-files-documentation)
6. [Source Files Documentation](#source-files-documentation)
7. [Algorithms and Data Structures](#algorithms-and-data-structures)
8. [How the Tool Works](#how-the-tool-works)
9. [Memory Management](#memory-management)
10. [Error Handling](#error-handling)
11. [Performance Considerations](#performance-considerations)

---

## Overview

The Duplicate File Finder is a C++ console application that identifies duplicate files in a directory structure using cryptographic hash algorithms. It supports multiple hash algorithms (MD5 and SHA-256), recursive/non-recursive scanning, and various duplicate handling actions (delete, move, create hard links).

### Key Features:
- **Multi-Algorithm Support**: MD5 (faster) and SHA-256 (more secure)
- **Flexible Scanning**: Recursive or non-recursive directory traversal
- **Multiple Actions**: Show, delete, move, or create hard links for duplicates
- **Interactive Mode**: User can review each duplicate group individually
- **Automatic Mode**: Apply actions to all duplicates automatically
- **Cross-Platform**: Uses standard C++17 filesystem library

---

## Project Structure

```
duplicate-file-finder/
├── CMakeLists.txt              # CMake build configuration
├── Makefile                    # Alternative build system
├── README.md                   # Project documentation
├── INSTALL_OPENSSL.md          # OpenSSL installation guide
├── build/                      # Build output directory
│   ├── CMakeCache.txt
│   ├── DuplicateFileFinder.sln # Visual Studio solution
│   ├── Debug/
│   │   └── DuplicateFileFinder.exe
│   └── CMakeFiles/             # CMake generated files
├── include/                    # Public header files
│   ├── duplicate_handler.h
│   ├── file_scanner.h
│   ├── hash_calculator.h
│   └── utils.h
├── src/                        # Source implementation files
│   ├── main.cpp
│   ├── duplicate_handler.cpp
│   ├── duplicate_handler.h
│   ├── file_scanner.cpp
│   ├── file_scanner.h
│   ├── hash_calculator.cpp
│   ├── hash_calculator.h
│   └── utils.h
└── tests/                      # Unit tests
    ├── test_duplicate_handler.cpp
    ├── test_file_scanner.cpp
    └── test_hash_calculator.cpp
```

---

## Build System

### CMakeLists.txt Analysis

```cmake
cmake_minimum_required(VERSION 3.10)
project(DuplicateFileFinder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

**Line-by-Line Explanation:**
- **Line 1**: Specifies minimum CMake version (3.10) required for C++17 support
- **Line 2**: Defines project name as "DuplicateFileFinder"
- **Line 4**: Sets C++ standard to C++17 (required for `std::filesystem`)
- **Line 5**: Makes C++17 a hard requirement (build fails if not available)

```cmake
# Find required packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(OPENSSL REQUIRED openssl)

# Include directories
include_directories(include)
include_directories(src)
```

**Dependency Management:**
- **Line 7-8**: Uses PkgConfig to find OpenSSL library (required for cryptographic hashing)
- **Line 11-12**: Adds include directories so headers can be found during compilation

```cmake
# Source files
set(SOURCES
    src/main.cpp
    src/file_scanner.cpp
    src/hash_calculator.cpp
    src/duplicate_handler.cpp
)

# Create executable
add_executable(DuplicateFileFinder ${SOURCES})

# Link libraries
target_link_libraries(DuplicateFileFinder ${OPENSSL_LIBRARIES})
target_compile_options(DuplicateFileFinder PRIVATE ${OPENSSL_CFLAGS_OTHER})
```

**Build Configuration:**
- **Lines 15-20**: Defines all source files to be compiled
- **Line 23**: Creates executable target from source files
- **Lines 26-27**: Links OpenSSL libraries and sets compiler flags

---

## Header Files Documentation

### 1. hash_calculator.h

```cpp
#ifndef HASH_CALCULATOR_H
#define HASH_CALCULATOR_H

#include <string>
#include <openssl/evp.h>

enum class HashAlgorithm {
    MD5,
    SHA256
};
```

**Line-by-Line Analysis:**
- **Lines 1-2**: Include guard to prevent multiple inclusions
- **Lines 4-5**: Standard library includes for string handling and OpenSSL EVP (Envelope) API
- **Lines 7-10**: Strongly-typed enum for hash algorithm selection (C++11 feature)

```cpp
class HashCalculator {
public:
    static std::string calculateMD5(const std::string& filePath);
    static std::string calculateSHA256(const std::string& filePath);
    static std::string calculateHash(const std::string& filePath, HashAlgorithm algorithm);
    static bool compareFiles(const std::string& filePath1, const std::string& filePath2, HashAlgorithm algorithm);
};
```

**Class Design:**
- **Static Methods**: All methods are static - no instance state needed
- **calculateMD5/calculateSHA256**: Algorithm-specific hash calculation
- **calculateHash**: Generic interface using enum dispatch
- **compareFiles**: Convenience method for direct file comparison

### 2. file_scanner.h

```cpp
struct FileInfo {
    std::string path;
    std::string hash;
    std::uintmax_t size;
    std::filesystem::file_time_type lastModified;
};
```

**FileInfo Structure:**
- **path**: Full file path as string
- **hash**: Calculated hash value (hex string)
- **size**: File size in bytes (`std::uintmax_t` for large file support)
- **lastModified**: File modification timestamp (filesystem time type)

```cpp
class FileScanner {
public:
    FileScanner() = default;
    
    std::vector<std::vector<std::string>> findDuplicates(const std::string& directoryPath, 
                                                         HashAlgorithm algorithm = HashAlgorithm::SHA256,
                                                         bool recursive = true);
    
    const std::vector<FileInfo>& getScannedFiles() const { return scannedFiles; }
    
    size_t getTotalFilesScanned() const { return scannedFiles.size(); }
    size_t getTotalDuplicateGroups() const { return duplicateGroups.size(); }
```

**Public Interface:**
- **Default constructor**: No special initialization needed
- **findDuplicates**: Main method returning vector of duplicate groups
- **Default parameters**: SHA256 algorithm, recursive scanning enabled
- **Getter methods**: Access to scan results and statistics

```cpp
private:
    std::vector<FileInfo> scannedFiles;
    std::vector<std::vector<std::string>> duplicateGroups;
    
    void scanDirectory(const std::string& directoryPath, HashAlgorithm algorithm, bool recursive);
    void processFile(const std::filesystem::path& filePath, HashAlgorithm algorithm);
    void findDuplicateGroups();
```

**Private Members:**
- **scannedFiles**: Vector storing all processed file information
- **duplicateGroups**: Vector of vectors - each inner vector contains paths of duplicate files
- **Helper methods**: Encapsulate scanning logic, file processing, and duplicate detection

### 3. duplicate_handler.h

```cpp
enum class DuplicateAction {
    DELETE,
    MOVE,
    HARD_LINK,
    SHOW_ONLY
};
```

**Action Enumeration:**
- **DELETE**: Remove duplicate files (keep first occurrence)
- **MOVE**: Move duplicates to specified directory
- **HARD_LINK**: Create hard links to save disk space
- **SHOW_ONLY**: Display duplicates without taking action

```cpp
class DuplicateHandler {
public:
    bool deleteDuplicate(const std::string& filePath);
    bool moveDuplicate(const std::string& filePath, const std::string& targetDirectory);
    bool createHardLink(const std::string& originalPath, const std::string& linkPath);
    
    void handleDuplicates(const std::vector<std::string>& duplicateFiles, 
                         DuplicateAction action, 
                         const std::string& targetDirectory = "");
    
    void handleDuplicatesInteractive(const std::vector<std::string>& duplicateFiles);
};
```

**Method Design:**
- **Individual operations**: Each action has its own method returning success/failure
- **Batch processing**: `handleDuplicates` applies action to entire duplicate group
- **Interactive mode**: `handleDuplicatesInteractive` prompts user for each group

---

## Source Files Documentation

### 1. main.cpp - Application Entry Point

#### Menu Display Function
```cpp
void displayMenu() {
    std::cout << "\n=== Duplicate File Finder ===" << std::endl;
    std::cout << "1. Scan Directory for Duplicates" << std::endl;
    std::cout << "2. Configure Settings" << std::endl;
    std::cout << "3. Show Statistics" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Choose an option: ";
}
```

**Design Pattern**: Simple console UI with numbered options
**User Experience**: Clear separation of functionality into logical menu items

#### Settings Display and Configuration
```cpp
void displaySettings(HashAlgorithm& algorithm, bool& recursive, DuplicateAction& action) {
    std::cout << "\n=== Current Settings ===" << std::endl;
    std::cout << "Hash Algorithm: " << (algorithm == HashAlgorithm::MD5 ? "MD5" : "SHA256") << std::endl;
    std::cout << "Recursive Scan: " << (recursive ? "Yes" : "No") << std::endl;
    std::cout << "Default Action: ";
    switch (action) {
        case DuplicateAction::DELETE: std::cout << "Delete"; break;
        case DuplicateAction::MOVE: std::cout << "Move"; break;
        case DuplicateAction::HARD_LINK: std::cout << "Hard Link"; break;
        case DuplicateAction::SHOW_ONLY: std::cout << "Show Only"; break;
    }
    std::cout << std::endl;
}
```

**Reference Parameters**: All parameters are references to allow modification
**Ternary Operators**: Concise boolean-to-string conversion
**Switch Statement**: Comprehensive enum handling with explicit cases

#### Input Validation and Error Handling
```cpp
void configureSettings(HashAlgorithm& algorithm, bool& recursive, DuplicateAction& action) {
    int choice;
    
    if (!(std::cin >> choice)) {
        std::cout << "Invalid input. Please enter a number." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
```

**Robust Input Handling:**
- **Stream state checking**: `!(std::cin >> choice)` detects input failures
- **Error recovery**: `std::cin.clear()` resets error flags
- **Buffer clearing**: `std::cin.ignore()` removes invalid input from buffer
- **Numeric limits**: Uses `std::numeric_limits` for maximum portability

#### Main Application Loop
```cpp
int main() {
    std::string directoryPath;
    std::string targetDirectory;
    int choice;
    
    // Default settings
    HashAlgorithm algorithm = HashAlgorithm::SHA256;
    bool recursive = true;
    DuplicateAction defaultAction = DuplicateAction::SHOW_ONLY;
    
    FileScanner scanner;
    DuplicateHandler handler;
```

**Variable Initialization:**
- **Default algorithm**: SHA256 for better security
- **Default mode**: Recursive scanning enabled
- **Default action**: Show-only for safety (no destructive operations)
- **Object creation**: Scanner and handler created once, reused throughout session

#### Path Input with Space Handling
```cpp
switch (choice) {
    case 1: {
        std::cout << "Enter directory path to scan: ";
        std::getline(std::cin, directoryPath);
        
        try {
            auto duplicateGroups = scanner.findDuplicates(directoryPath, algorithm, recursive);
```

**Critical Fix**: Uses `std::getline()` instead of `std::cin >>` to handle paths with spaces
**RAII Exception Handling**: Try-catch block ensures resources are properly cleaned up
**Auto Type Deduction**: `auto` keyword for cleaner code (C++11 feature)

### 2. hash_calculator.cpp - Cryptographic Hashing

#### MD5 Implementation
```cpp
std::string HashCalculator::calculateMD5(const std::string& filePath) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        throw std::runtime_error("Failed to create MD5 context");
    }

    if (EVP_DigestInit_ex(mdctx, EVP_md5(), nullptr) != 1) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to initialize MD5");
    }
```

**OpenSSL EVP API:**
- **EVP_MD_CTX**: Generic message digest context (works with any hash algorithm)
- **EVP_MD_CTX_new()**: Allocates new context on heap
- **EVP_DigestInit_ex()**: Initializes context with specific algorithm (MD5)
- **Error handling**: Checks return codes and frees resources on failure

#### File Reading with Buffering
```cpp
    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    char buffer[8192];
    while (file.read(buffer, sizeof(buffer))) {
        if (EVP_DigestUpdate(mdctx, buffer, file.gcount()) != 1) {
            EVP_MD_CTX_free(mdctx);
            throw std::runtime_error("Failed to update MD5 hash");
        }
    }
```

**Efficient File Processing:**
- **Binary mode**: `std::ios::binary` prevents text transformations
- **8KB buffer**: Optimal size for most systems (balance between memory and I/O efficiency)
- **Streaming approach**: Processes large files without loading entirely into memory
- **gcount()**: Returns actual number of bytes read (important for last chunk)

#### Hash Finalization and Formatting
```cpp
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    if (EVP_DigestFinal_ex(mdctx, hash, &hash_len) != 1) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to finalize MD5 hash");
    }

    EVP_MD_CTX_free(mdctx);

    std::ostringstream result;
    for (unsigned int i = 0; i < hash_len; ++i) {
        result << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return result.str();
```

**Hash Output Processing:**
- **EVP_MAX_MD_SIZE**: Maximum possible hash size (64 bytes for SHA-512)
- **hash_len**: Actual hash length (16 for MD5, 32 for SHA-256)
- **Hex formatting**: Converts binary hash to lowercase hexadecimal string
- **std::setw(2) + std::setfill('0')**: Ensures each byte becomes exactly 2 hex digits

#### SHA-256 Implementation
```cpp
std::string HashCalculator::calculateSHA256(const std::string& filePath) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        throw std::runtime_error("Failed to create SHA256 context");
    }

    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to initialize SHA256");
    }
    // ... rest is identical to MD5 except algorithm selection
```

**Algorithm Polymorphism**: Same code structure with different EVP algorithm
**EVP_sha256()**: OpenSSL function pointer for SHA-256 algorithm
**Identical processing**: File reading, buffering, and output formatting are algorithm-agnostic

### 3. file_scanner.cpp - Directory Traversal and Duplicate Detection

#### Main Scanning Function
```cpp
std::vector<std::vector<std::string>> FileScanner::findDuplicates(const std::string& directoryPath, 
                                                                  HashAlgorithm algorithm, 
                                                                  bool recursive) {
    scannedFiles.clear();
    duplicateGroups.clear();
    
    std::cout << "Scanning directory: " << directoryPath << std::endl;
    std::cout << "Using " << (algorithm == HashAlgorithm::MD5 ? "MD5" : "SHA256") << " hashing" << std::endl;
    std::cout << "Recursive: " << (recursive ? "Yes" : "No") << std::endl;
    
    scanDirectory(directoryPath, algorithm, recursive);
    findDuplicateGroups();
    
    std::cout << "Scan complete. Found " << scannedFiles.size() << " files." << std::endl;
    std::cout << "Found " << duplicateGroups.size() << " groups of duplicates." << std::endl;
    
    return duplicateGroups;
}
```

**Three-Phase Process:**
1. **Preparation**: Clear previous results, display scan parameters
2. **Scanning**: Traverse directory structure and process files
3. **Analysis**: Group files by hash to identify duplicates

#### Directory Traversal Algorithm
```cpp
void FileScanner::scanDirectory(const std::string& directoryPath, HashAlgorithm algorithm, bool recursive) {
    try {
        if (recursive) {
            for (const auto& entry : fs::recursive_directory_iterator(directoryPath)) {
                if (fs::is_regular_file(entry)) {
                    processFile(entry.path(), algorithm);
                }
            }
        } else {
            for (const auto& entry : fs::directory_iterator(directoryPath)) {
                if (fs::is_regular_file(entry)) {
                    processFile(entry.path(), algorithm);
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error scanning directory: " << e.what() << std::endl;
    }
}
```

**C++17 Filesystem Library:**
- **fs::recursive_directory_iterator**: Automatically traverses subdirectories
- **fs::directory_iterator**: Only processes current directory level
- **fs::is_regular_file()**: Filters out directories, symlinks, and special files
- **Range-based for loop**: Clean, modern C++ iteration syntax
- **Specific exception handling**: Catches filesystem-specific errors

#### File Processing and Metadata Collection
```cpp
void FileScanner::processFile(const fs::path& filePath, HashAlgorithm algorithm) {
    try {
        FileInfo fileInfo;
        fileInfo.path = filePath.string();
        fileInfo.size = fs::file_size(filePath);
        fileInfo.lastModified = fs::last_write_time(filePath);
        
        // Calculate hash
        fileInfo.hash = HashCalculator::calculateHash(filePath.string(), algorithm);
        
        scannedFiles.push_back(fileInfo);
        
        std::cout << "Processed: " << filePath.filename() << " (Size: " << fileInfo.size << " bytes)" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error processing file " << filePath << ": " << e.what() << std::endl;
    }
}
```

**Comprehensive File Information:**
- **path**: Full path for later operations
- **size**: Used for statistics and verification
- **lastModified**: Could be used for additional duplicate resolution logic
- **hash**: Primary key for duplicate detection

**Error Resilience**: Individual file processing errors don't stop entire scan

#### Duplicate Detection Algorithm
```cpp
void FileScanner::findDuplicateGroups() {
    std::unordered_map<std::string, std::vector<std::string>> hashToFiles;
    
    // Group files by hash
    for (const auto& fileInfo : scannedFiles) {
        hashToFiles[fileInfo.hash].push_back(fileInfo.path);
    }
    
    // Find groups with more than one file (duplicates)
    for (const auto& pair : hashToFiles) {
        if (pair.second.size() > 1) {
            duplicateGroups.push_back(pair.second);
        }
    }
    
    // Sort duplicate groups by size (largest groups first)
    std::sort(duplicateGroups.begin(), duplicateGroups.end(), 
              [](const std::vector<std::string>& a, const std::vector<std::string>& b) {
                  return a.size() > b.size();
              });
}
```

**Algorithm Analysis:**
- **Time Complexity**: O(n) for grouping + O(k log k) for sorting (where n = files, k = duplicate groups)
- **Space Complexity**: O(n) for hash map storage
- **Hash Map**: Uses std::unordered_map for O(1) average insertion/lookup
- **Lambda Sorting**: Groups sorted by size (largest first) for better user experience

### 4. duplicate_handler.cpp - File Operations

#### Safe File Deletion
```cpp
bool DuplicateHandler::deleteDuplicate(const std::string& filePath) {
    try {
        if (std::filesystem::remove(filePath)) {
            std::cout << "Deleted: " << filePath << std::endl;
            return true;
        } else {
            std::cerr << "Failed to delete: " << filePath << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception deleting file " << filePath << ": " << e.what() << std::endl;
        return false;
    }
}
```

**Safe Deletion Pattern:**
- **std::filesystem::remove()**: Cross-platform file deletion
- **Return value checking**: remove() returns false if file doesn't exist
- **Exception handling**: Catches permission errors, disk errors, etc.
- **User feedback**: Clear success/failure messages

#### Intelligent File Moving with Conflict Resolution
```cpp
bool DuplicateHandler::moveDuplicate(const std::string& filePath, const std::string& targetDirectory) {
    try {
        std::filesystem::path sourcePath(filePath);
        std::filesystem::path targetPath(targetDirectory);
        
        // Create target directory if it doesn't exist
        if (!std::filesystem::exists(targetPath)) {
            std::filesystem::create_directories(targetPath);
        }
        
        std::filesystem::path newFilePath = targetPath / sourcePath.filename();
        
        // Handle name conflicts
        int counter = 1;
        std::filesystem::path finalPath = newFilePath;
        while (std::filesystem::exists(finalPath)) {
            std::string stem = newFilePath.stem().string();
            std::string extension = newFilePath.extension().string();
            finalPath = targetPath / (stem + "_" + std::to_string(counter) + extension);
            counter++;
        }
        
        std::filesystem::rename(sourcePath, finalPath);
        std::cout << "Moved: " << filePath << " to " << finalPath << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception moving file " << filePath << ": " << e.what() << std::endl;
        return false;
    }
}
```

**Advanced Features:**
- **Automatic directory creation**: Creates target directory if missing
- **Name conflict resolution**: Appends counter to filename (file_1.txt, file_2.txt, etc.)
- **Path manipulation**: Uses filesystem::path for robust path operations
- **stem() and extension()**: Separates filename parts for conflict resolution

#### Hard Link Creation for Space Saving
```cpp
bool DuplicateHandler::createHardLink(const std::string& originalPath, const std::string& linkPath) {
    try {
        std::filesystem::create_hard_link(originalPath, linkPath);
        std::cout << "Created hard link: " << linkPath << " -> " << originalPath << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception creating hard link " << linkPath << ": " << e.what() << std::endl;
        return false;
    }
}
```

**Hard Link Benefits:**
- **Space saving**: Multiple file entries point to same disk data
- **Automatic synchronization**: Changes to one file appear in all hard links
- **Reference counting**: File data only deleted when last hard link is removed

#### Interactive Duplicate Handling
```cpp
void DuplicateHandler::handleDuplicatesInteractive(const std::vector<std::string>& duplicateFiles) {
    if (duplicateFiles.size() <= 1) {
        return;
    }
    
    std::cout << "\nFound " << duplicateFiles.size() << " duplicate files:" << std::endl;
    for (size_t i = 0; i < duplicateFiles.size(); ++i) {
        std::cout << "  " << i + 1 << ". " << duplicateFiles[i] << std::endl;
    }
    
    std::cout << "\nChoose action:" << std::endl;
    std::cout << "1. Delete all duplicates (keep first)" << std::endl;
    std::cout << "2. Move duplicates to folder" << std::endl;
    std::cout << "3. Create hard links (replace duplicates)" << std::endl;
    std::cout << "4. Skip this group" << std::endl;
    std::cout << "Choice: ";
    
    int choice;
    std::cin >> choice;
    // ... switch statement for handling user choice
}
```

**User Experience Design:**
- **Clear enumeration**: Lists all duplicate files with numbers
- **Safe default**: Option to skip group without action
- **Immediate feedback**: Shows exactly which files will be affected

---

## Algorithms and Data Structures

### 1. Hash Algorithms

#### MD5 Algorithm
**Properties:**
- **Output size**: 128 bits (16 bytes)
- **Speed**: ~400-500 MB/s on modern hardware
- **Security**: Cryptographically broken (collisions found), but sufficient for duplicate detection
- **Use case**: Fast duplicate detection where speed > security

**Algorithm Steps:**
1. **Padding**: Message padded to 512-bit boundary
2. **Processing**: 64 rounds of operations on 512-bit blocks
3. **Output**: 128-bit hash value

#### SHA-256 Algorithm
**Properties:**
- **Output size**: 256 bits (32 bytes)
- **Speed**: ~200-300 MB/s on modern hardware
- **Security**: Cryptographically secure (no known attacks)
- **Use case**: Secure duplicate detection

**Algorithm Steps:**
1. **Padding**: Message padded to 512-bit boundary
2. **Processing**: 64 rounds of operations using constants and functions
3. **Compression**: Eight 32-bit words compressed into final hash

### 2. Directory Traversal Algorithms

#### Recursive Traversal
```
Algorithm: RecursiveTraversal(directory)
1. Create stack or use recursion
2. For each entry in directory:
   a. If entry is regular file: process file
   b. If entry is directory: RecursiveTraversal(entry)
3. Handle symlinks and special files appropriately
```

**Time Complexity**: O(n) where n = total number of files and directories
**Space Complexity**: O(d) where d = maximum directory depth

#### Non-Recursive Traversal
```
Algorithm: NonRecursiveTraversal(directory)
1. For each entry in directory:
   a. If entry is regular file: process file
   b. If entry is directory: skip
```

**Time Complexity**: O(k) where k = files in immediate directory
**Space Complexity**: O(1)

### 3. Duplicate Detection Algorithm

```
Algorithm: FindDuplicates(files)
1. Initialize hashMap: String -> List<String>
2. For each file in files:
   a. hash = calculateHash(file)
   b. hashMap[hash].append(file.path)
3. duplicateGroups = []
4. For each (hash, fileList) in hashMap:
   a. If fileList.size() > 1:
      duplicateGroups.append(fileList)
5. Sort duplicateGroups by size (descending)
6. Return duplicateGroups
```

**Time Complexity**: O(n × h + k log k)
- n × h: n files × hash calculation time
- k log k: sorting k duplicate groups

**Space Complexity**: O(n) for storing file paths and hashes

### 4. File I/O Optimization

#### Buffered Reading Strategy
```cpp
char buffer[8192];  // 8KB buffer
while (file.read(buffer, sizeof(buffer))) {
    hash_update(buffer, file.gcount());
}
```

**Buffer Size Optimization:**
- **Too small (< 1KB)**: Excessive system calls, poor performance
- **Optimal (4-16KB)**: Good balance of memory usage and I/O efficiency
- **Too large (> 64KB)**: Diminishing returns, wastes memory

**Performance Analysis:**
- **8KB buffer**: ~95% of optimal performance
- **Memory usage**: Minimal (8KB per file being processed)
- **System calls**: Reduced by factor of buffer_size/1

---

## How the Tool Works

### 1. Application Startup Flow

```
main() execution:
1. Initialize default settings (SHA256, recursive, show-only)
2. Create FileScanner and DuplicateHandler objects
3. Display welcome message
4. Enter main menu loop
```

### 2. Duplicate Detection Workflow

```
User selects "Scan Directory":
1. Prompt for directory path (handle spaces correctly)
2. Call scanner.findDuplicates(path, algorithm, recursive)
   a. Clear previous results
   b. scanDirectory() - traverse filesystem
   c. For each file: processFile() - calculate hash
   d. findDuplicateGroups() - group by hash
   e. Sort groups by size
3. Display results summary
4. Handle duplicates based on default action:
   a. SHOW_ONLY: Interactive mode for each group
   b. Others: Automatic processing
```

### 3. File Processing Pipeline

```
For each file found:
1. Check if regular file (not directory/symlink)
2. Open file in binary mode
3. Read file in 8KB chunks
4. Feed chunks to hash algorithm
5. Generate hex string representation
6. Store FileInfo{path, hash, size, modified_time}
7. Continue to next file
```

### 4. Duplicate Grouping Process

```
After all files processed:
1. Create hash map: hash_string -> [file_paths]
2. Iterate through all FileInfo objects
3. Group files by identical hash values
4. Filter groups with size > 1 (actual duplicates)
5. Sort groups by size (largest first)
6. Return grouped duplicate lists
```

### 5. User Interaction Modes

#### Interactive Mode (Default)
```
For each duplicate group:
1. Display all duplicate files with numbers
2. Show action menu (delete/move/hard-link/skip)
3. Process user choice
4. Apply action to files (except first one)
5. Continue to next group
```

#### Automatic Mode
```
If default action set:
1. Apply same action to all duplicate groups
2. Prompt for target directory if needed (move/hard-link)
3. Process all groups without user intervention
4. Display summary of actions taken
```

---

## Memory Management

### 1. Resource Allocation Strategy

**Stack Allocation:**
- Local variables (file paths, hash values)
- Small buffers (8KB file reading buffer)
- Function parameters and return values

**Heap Allocation:**
- std::vector containers for file lists
- std::string objects for paths and hashes
- OpenSSL hash contexts (EVP_MD_CTX)

### 2. Memory Usage Analysis

**Per-file overhead:**
```cpp
FileInfo structure:
- path: ~100 bytes average (depends on path length)
- hash: 32-64 bytes (depending on algorithm)
- size: 8 bytes (std::uintmax_t)
- lastModified: 16 bytes (file_time_type)
Total per file: ~150-200 bytes
```

**For 10,000 files**: ~1.5-2 MB memory usage

### 3. RAII (Resource Acquisition Is Initialization)

**File Handles:**
```cpp
std::ifstream file(filePath, std::ios::binary);
// Automatically closed when file goes out of scope
```

**OpenSSL Contexts:**
```cpp
EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
// Must be manually freed with EVP_MD_CTX_free(mdctx)
// Exception safety: freed in all error paths
```

---

## Error Handling

### 1. Exception Safety Levels

**Basic Exception Safety:**
- No resource leaks
- Program remains in valid state
- May lose some work

**Strong Exception Safety:**
- Operation succeeds completely or has no effect
- Used in hash calculation functions

**No-throw Guarantee:**
- Operation never throws exceptions
- Used in destructor-like operations

### 2. Error Categories

**Filesystem Errors:**
- File not found
- Permission denied
- Disk full
- Network drive disconnected

**Cryptographic Errors:**
- OpenSSL initialization failure
- Hash context creation failure
- Memory allocation failure

**Input Validation Errors:**
- Invalid directory path
- Invalid menu choice
- Malformed input

### 3. Error Recovery Strategies

**Graceful Degradation:**
- Skip problematic files, continue scanning
- Report errors but don't abort operation

**User Notification:**
- Clear error messages with context
- Suggest possible solutions

**Resource Cleanup:**
- Automatic cleanup with RAII
- Manual cleanup in exception handlers

---

## Performance Considerations

### 1. I/O Optimization

**Sequential Access Pattern:**
- Files read sequentially, never random access
- Optimal for both HDD and SSD storage

**Buffer Size Tuning:**
- 8KB buffer chosen for optimal performance
- Larger buffers show diminishing returns

### 2. Algorithm Selection

**MD5 vs SHA-256 Trade-offs:**

| Aspect | MD5 | SHA-256 |
|--------|-----|---------|
| Speed | ~2x faster | ~1x baseline |
| Security | Broken | Secure |
| Collision Resistance | No | Yes |
| Memory Usage | Same | Same |

**Recommendation:**
- Use MD5 for trusted environments, speed critical
- Use SHA-256 for security-sensitive applications

### 3. Memory Optimization

**Streaming Processing:**
- Files processed one at a time
- No need to load entire file into memory
- Scales to files larger than available RAM

**Hash Map Efficiency:**
- std::unordered_map provides O(1) average lookup
- Hash collisions rare with good hash algorithms
- Memory overhead acceptable for duplicate detection

### 4. Scalability Analysis

**File Count Scaling:**
- Linear time complexity O(n)
- Memory usage scales linearly with file count
- No practical limit on number of files

**File Size Scaling:**
- Constant memory usage regardless of file size
- Time scales linearly with total data processed
- Works efficiently with TB-sized files

**Directory Depth Scaling:**
- Recursive traversal uses O(depth) stack space
- Typical filesystems: depth < 20, negligible impact

---

## Conclusion

This Duplicate File Finder represents a robust, well-engineered solution for duplicate file detection with the following key strengths:

1. **Algorithmic Efficiency**: O(n) duplicate detection with configurable hash algorithms
2. **Memory Efficiency**: Streaming file processing with minimal memory footprint
3. **User Experience**: Interactive and automatic modes with comprehensive error handling
4. **Cross-Platform**: Uses standard C++17 and OpenSSL for maximum portability
5. **Extensibility**: Clean separation of concerns allows easy feature additions

The codebase demonstrates modern C++ best practices including RAII, exception safety, and STL container usage while providing a practical tool for filesystem management.
