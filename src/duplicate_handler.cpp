#include "duplicate_handler.h"
#include <filesystem>
#include <iostream>
#include <stdexcept>

bool DuplicateHandler::deleteDuplicate(const std::string& filePath) {
    try {
        if (std::filesystem::remove(filePath)) {
            std::cout << "Deleted: " << filePath << std::endl;
            return true;
        } else {
            std::cerr << "Error deleting file: " << filePath << std::endl;
            return false;
        }
    } catch (const std::exception& e) {
        std::cerr << "Exception deleting file " << filePath << ": " << e.what() << std::endl;
        return false;
    }
}

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

bool DuplicateHandler::createHardLink(const std::string& originalPath, const std::string& linkPath) {
    try {
        std::filesystem::path linkDir = std::filesystem::path(linkPath).parent_path();
        if (!std::filesystem::exists(linkDir)) {
            std::filesystem::create_directories(linkDir);
        }
        
        std::filesystem::create_hard_link(originalPath, linkPath);
        std::cout << "Created hard link: " << linkPath << " for " << originalPath << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Exception creating hard link for " << originalPath << ": " << e.what() << std::endl;
        return false;
    }
}

void DuplicateHandler::handleDuplicates(const std::vector<std::string>& duplicateFiles, 
                                       DuplicateAction action, 
                                       const std::string& targetDirectory) {
    if (duplicateFiles.size() <= 1) {
        return;
    }
    
    std::cout << "\nFound " << duplicateFiles.size() << " duplicate files:" << std::endl;
    for (size_t i = 0; i < duplicateFiles.size(); ++i) {
        std::cout << "  " << i + 1 << ". " << duplicateFiles[i] << std::endl;
    }
    
    // Keep the first file, handle the rest
    for (size_t i = 1; i < duplicateFiles.size(); ++i) {
        const std::string& filePath = duplicateFiles[i];
        
        switch (action) {
            case DuplicateAction::DELETE:
                deleteDuplicate(filePath);
                break;
            case DuplicateAction::MOVE:
                if (!targetDirectory.empty()) {
                    moveDuplicate(filePath, targetDirectory);
                }
                break;
            case DuplicateAction::HARD_LINK:
                if (!targetDirectory.empty()) {
                    std::filesystem::path linkPath = std::filesystem::path(targetDirectory) / std::filesystem::path(filePath).filename();
                    createHardLink(duplicateFiles[0], linkPath.string());
                    deleteDuplicate(filePath);
                }
                break;
            case DuplicateAction::SHOW_ONLY:
                // Already shown above
                break;
        }
    }
}

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
    
    switch (choice) {
        case 1:
            handleDuplicates(duplicateFiles, DuplicateAction::DELETE);
            break;
        case 2: {
            std::string targetDir;
            std::cout << "Enter target directory: ";
            std::cin >> targetDir;
            handleDuplicates(duplicateFiles, DuplicateAction::MOVE, targetDir);
            break;
        }
        case 3: {
            std::string targetDir;
            std::cout << "Enter target directory for hard links: ";
            std::cin >> targetDir;
            handleDuplicates(duplicateFiles, DuplicateAction::HARD_LINK, targetDir);
            break;
        }
        case 4:
            std::cout << "Skipping this group." << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Skipping this group." << std::endl;
            break;
    }
}