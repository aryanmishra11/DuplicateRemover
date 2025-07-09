#ifndef FILE_SCANNER_H
#define FILE_SCANNER_H

#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include "hash_calculator.h"

struct FileInfo {
    std::string path;
    std::string hash;
    std::uintmax_t size;
    std::filesystem::file_time_type lastModified;
};

class FileScanner {
public:
    FileScanner() = default;
    
    // Scan directory and return groups of duplicate files
    std::vector<std::vector<std::string>> findDuplicates(const std::string& directoryPath, 
                                                         HashAlgorithm algorithm = HashAlgorithm::SHA256,
                                                         bool recursive = true);
    
    // Get all scanned files
    const std::vector<FileInfo>& getScannedFiles() const { return scannedFiles; }
    
    // Get statistics
    size_t getTotalFilesScanned() const { return scannedFiles.size(); }
    size_t getTotalDuplicateGroups() const { return duplicateGroups.size(); }

private:
    std::vector<FileInfo> scannedFiles;
    std::vector<std::vector<std::string>> duplicateGroups;
    
    void scanDirectory(const std::string& directoryPath, HashAlgorithm algorithm, bool recursive);
    void processFile(const std::filesystem::path& filePath, HashAlgorithm algorithm);
    void findDuplicateGroups();
};

#endif // FILE_SCANNER_H