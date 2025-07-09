# Duplicate File Finder Tool

## Overview
The Duplicate File Finder is a comprehensive C++ application designed to scan directories for duplicate files using advanced hash comparison techniques. It supports multiple hash algorithms (MD5 and SHA-256) and provides flexible options for handling duplicate files including deletion, moving, and hard link creation.

## Features
- **Multi-Algorithm Support**: Choose between MD5 (faster) and SHA-256 (more secure) hashing
- **Flexible Scanning**: Recursive or non-recursive directory scanning
- **Multiple Actions**: Delete, move, or create hard links for duplicate files
- **Interactive Mode**: Review and handle each duplicate group individually
- **Automatic Mode**: Apply actions to all duplicates automatically
- **Statistics**: View scan statistics including file counts and sizes
- **Cross-Platform**: Works on Windows, Linux, and macOS
- **Safe Operations**: Handles edge cases like name conflicts and permission issues

## Prerequisites
- C++17 compatible compiler (GCC 8+, Clang 7+, MSVC 2017+)
- OpenSSL library for hash calculations
- CMake 3.10 or higher (for CMake build)

### Installing OpenSSL
**Windows (vcpkg):**
```bash
vcpkg install openssl
```

**Ubuntu/Debian:**
```bash
sudo apt-get install libssl-dev
```

**macOS (Homebrew):**
```bash
brew install openssl
```

## Installation

### Option 1: Using CMake (Recommended)
1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/duplicate-file-finder.git
   cd duplicate-file-finder
   ```

2. Create build directory and compile:
   ```bash
   mkdir build
   cd build
   cmake ..
   make
   ```

### Option 2: Using Makefile
1. Clone the repository:
   ```bash
   git clone[https://github.com/aryanmishra11/DuplicateRemover]
   cd duplicate-file-finder
   ```

2. Compile using make:
   ```bash
   make
   ```

### Option 3: Manual Compilation
```bash
g++ -std=c++17 -Iinclude -O2 -Wall -Wextra \
    src/main.cpp src/file_scanner.cpp src/hash_calculator.cpp src/duplicate_handler.cpp \
    -o duplicate_file_finder -lssl -lcrypto
```

## Usage

### Running the Application
```bash
./duplicate_file_finder
```

### Menu Options
1. **Scan Directory for Duplicates**: Main functionality to find and handle duplicates
2. **Configure Settings**: Customize hash algorithm, scanning mode, and default actions
3. **Show Statistics**: View information about the last scan
4. **Exit**: Close the application

### Configuration Options
- **Hash Algorithm**: Choose between MD5 (faster) or SHA-256 (more secure)
- **Recursive Scan**: Enable/disable recursive directory scanning
- **Default Action**: Set automatic action for duplicates (show only, delete, move, hard link)

### Handling Duplicates
When duplicates are found, you can:
1. **Show Only**: Display duplicates without taking action
2. **Delete**: Remove duplicate files (keeps the first occurrence)
3. **Move**: Move duplicates to a specified directory
4. **Hard Link**: Replace duplicates with hard links to save space

## Examples

### Basic Usage
```
=== Duplicate File Finder ===
1. Scan Directory for Duplicates
2. Configure Settings
3. Show Statistics
4. Exit
Choose an option: 1
Enter directory path to scan: /home/user/Documents
```

### Configuration Example
```
=== Configure Settings ===
1. Change Hash Algorithm
2. Toggle Recursive Scan
3. Change Default Action
4. Back to Main Menu
Choose option: 1
Select Hash Algorithm:
1. MD5 (faster)
2. SHA256 (more secure)
Choice: 2
```

## File Structure
```
duplicate-file-finder/
├── src/
│   ├── main.cpp              # Main application entry point
│   ├── file_scanner.cpp      # Directory scanning and duplicate detection
│   ├── file_scanner.h
│   ├── hash_calculator.cpp   # Hash calculation utilities
│   ├── hash_calculator.h
│   ├── duplicate_handler.cpp # Duplicate file handling operations
│   └── duplicate_handler.h
├── include/                  # Header files
├── tests/                    # Unit tests
├── CMakeLists.txt           # CMake build configuration
├── Makefile                 # Alternative build system
└── README.md
```

## API Reference

### HashCalculator Class
```cpp
enum class HashAlgorithm { MD5, SHA256 };

class HashCalculator {
public:
    static std::string calculateMD5(const std::string& filePath);
    static std::string calculateSHA256(const std::string& filePath);
    static std::string calculateHash(const std::string& filePath, HashAlgorithm algorithm);
    static bool compareFiles(const std::string& file1, const std::string& file2, HashAlgorithm algorithm);
};
```

### FileScanner Class
```cpp
class FileScanner {
public:
    std::vector<std::vector<std::string>> findDuplicates(
        const std::string& directoryPath, 
        HashAlgorithm algorithm = HashAlgorithm::SHA256,
        bool recursive = true);
    
    size_t getTotalFilesScanned() const;
    size_t getTotalDuplicateGroups() const;
};
```

### DuplicateHandler Class
```cpp
enum class DuplicateAction { DELETE, MOVE, HARD_LINK, SHOW_ONLY };

class DuplicateHandler {
public:
    bool deleteDuplicate(const std::string& filePath);
    bool moveDuplicate(const std::string& filePath, const std::string& targetDirectory);
    bool createHardLink(const std::string& originalPath, const std::string& linkPath);
    void handleDuplicates(const std::vector<std::string>& duplicateFiles, 
                         DuplicateAction action, 
                         const std::string& targetDirectory = "");
};
```

## Testing
Run the test suite:
```bash
# Using CMake
cd build
make test

# Or run tests directly
./DuplicateFileFinderTests
```

## Performance Considerations
- **MD5 vs SHA-256**: MD5 is faster but less secure; SHA-256 is slower but more secure
- **File Size**: Large files will take longer to hash
- **Storage**: Hard links save space but are limited to the same filesystem
- **Memory**: The application loads file information into memory during scanning

## Troubleshooting

### Common Issues
1. **OpenSSL not found**: Ensure OpenSSL is installed and in your system PATH
2. **Permission denied**: Run with appropriate permissions for the target directory
3. **File system errors**: Check that the target directory exists and is accessible
4. **Hard link creation fails**: Ensure target is on the same filesystem

### Error Messages
- `"Unable to open file"`: File may be locked or permissions insufficient
- `"Exception creating hard link"`: Files may be on different filesystems
- `"Error scanning directory"`: Directory may not exist or be inaccessible

## Contributing
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests for new functionality
5. Submit a pull request

## License
This project is licensed under the MIT License - see the LICENSE file for details.

## Future Enhancements
- GUI interface
- Configuration file support
- Network location support
- Progress bars for large scans
- Resume interrupted scans
- Exclude patterns/filters
- Duplicate size statistics
- Batch operations from command line
```
./duplicate-file-finder <directory_path>
```
Replace `<directory_path>` with the path of the directory you want to scan for duplicates.

## Options
After scanning, you will be presented with options to handle the duplicates:
- **Delete**: Permanently remove the duplicate files.
- **Move**: Move the duplicate files to a specified directory.
- **Create Hard Link**: Create hard links for the duplicate files.

## Contributing
Contributions are welcome! Please open an issue or submit a pull request for any enhancements or bug fixes.
