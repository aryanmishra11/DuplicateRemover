#ifndef DUPLICATE_HANDLER_H
#define DUPLICATE_HANDLER_H

#include <string>
#include <vector>

class DuplicateHandler {
public:
    void deleteDuplicate(const std::string& filePath);
    void moveDuplicate(const std::string& filePath, const std::string& destination);
    void createHardLink(const std::string& filePath, const std::string& linkPath);
};

#endif // DUPLICATE_HANDLER_H