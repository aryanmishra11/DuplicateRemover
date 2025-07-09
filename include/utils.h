#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

// Utility functions for file path manipulation and error handling

std::string getFileName(const std::string& filePath);
std::string getFileExtension(const std::string& filePath);
bool fileExists(const std::string& filePath);
std::vector<std::string> listFilesInDirectory(const std::string& directoryPath);

#endif // UTILS_H