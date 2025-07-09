#include "file_scanner.h"
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <algorithm>

namespace fs = std::filesystem;

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