#ifndef FILE_SCANNER_H
#define FILE_SCANNER_H

#include <string>
#include <vector>

class FileScanner {
public:
    FileScanner(const std::string& directory);
    void scanDirectory();
    const std::vector<std::string>& getFiles() const;

private:
    std::string directoryPath;
    std::vector<std::string> files;

    void traverseDirectory(const std::string& dir);
};

#endif // FILE_SCANNER_H