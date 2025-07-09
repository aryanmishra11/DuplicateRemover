#ifndef DUPLICATE_HANDLER_H
#define DUPLICATE_HANDLER_H

#include <string>
#include <vector>
#include <filesystem>

enum class DuplicateAction {
    DELETE,
    MOVE,
    HARD_LINK,
    SHOW_ONLY
};

class DuplicateHandler {
public:
    // Deletes a duplicate file
    bool deleteDuplicate(const std::string& filePath);

    // Moves a duplicate file to a specified directory
    bool moveDuplicate(const std::string& filePath, const std::string& targetDirectory);

    // Creates a hard link for a duplicate file
    bool createHardLink(const std::string& originalPath, const std::string& linkPath);

    // Handle duplicates with specified action
    void handleDuplicates(const std::vector<std::string>& duplicateFiles, 
                         DuplicateAction action, 
                         const std::string& targetDirectory = "");

    // Interactive duplicate handling
    void handleDuplicatesInteractive(const std::vector<std::string>& duplicateFiles);
};

#endif // DUPLICATE_HANDLER_H