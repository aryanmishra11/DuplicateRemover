#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include "file_scanner.h"
#include "hash_calculator.h"
#include "duplicate_handler.h"

void displayMenu() {
    std::cout << "\n=== Duplicate File Finder ===" << std::endl;
    std::cout << "1. Scan Directory for Duplicates" << std::endl;
    std::cout << "2. Configure Settings" << std::endl;
    std::cout << "3. Show Statistics" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Choose an option: ";
}

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

void configureSettings(HashAlgorithm& algorithm, bool& recursive, DuplicateAction& action) {
    int choice;
    
    std::cout << "\n=== Configure Settings ===" << std::endl;
    std::cout << "1. Change Hash Algorithm" << std::endl;
    std::cout << "2. Toggle Recursive Scan" << std::endl;
    std::cout << "3. Change Default Action" << std::endl;
    std::cout << "4. Back to Main Menu" << std::endl;
    std::cout << "Choose option: ";
    
    if (!(std::cin >> choice)) {
        std::cout << "Invalid input. Please enter a number." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    
    switch (choice) {
        case 1: {
            std::cout << "Select Hash Algorithm:" << std::endl;
            std::cout << "1. MD5 (faster)" << std::endl;
            std::cout << "2. SHA256 (more secure)" << std::endl;
            std::cout << "Choice: ";
            int algoChoice;
            if (std::cin >> algoChoice) {
                algorithm = (algoChoice == 1) ? HashAlgorithm::MD5 : HashAlgorithm::SHA256;
                std::cout << "Hash algorithm updated." << std::endl;
            } else {
                std::cout << "Invalid input." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            break;
        }
        case 2:
            recursive = !recursive;
            std::cout << "Recursive scan " << (recursive ? "enabled" : "disabled") << std::endl;
            break;
        case 3: {
            std::cout << "Select Default Action:" << std::endl;
            std::cout << "1. Show Only" << std::endl;
            std::cout << "2. Delete Duplicates" << std::endl;
            std::cout << "3. Move Duplicates" << std::endl;
            std::cout << "4. Create Hard Links" << std::endl;
            std::cout << "Choice: ";
            int actionChoice;
            if (std::cin >> actionChoice) {
                switch (actionChoice) {
                    case 1: action = DuplicateAction::SHOW_ONLY; break;
                    case 2: action = DuplicateAction::DELETE; break;
                    case 3: action = DuplicateAction::MOVE; break;
                    case 4: action = DuplicateAction::HARD_LINK; break;
                    default: std::cout << "Invalid choice." << std::endl; break;
                }
                if (actionChoice >= 1 && actionChoice <= 4) {
                    std::cout << "Default action updated." << std::endl;
                }
            } else {
                std::cout << "Invalid input." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            break;
        }
        case 4:
            break;
        default:
            std::cout << "Invalid option." << std::endl;
            break;
    }
}

void showStatistics(const FileScanner& scanner) {
    std::cout << "\n=== Scan Statistics ===" << std::endl;
    std::cout << "Total files scanned: " << scanner.getTotalFilesScanned() << std::endl;
    std::cout << "Duplicate groups found: " << scanner.getTotalDuplicateGroups() << std::endl;
    
    const auto& files = scanner.getScannedFiles();
    if (!files.empty()) {
        std::uintmax_t totalSize = 0;
        for (const auto& file : files) {
            totalSize += file.size;
        }
        std::cout << "Total size scanned: " << std::fixed << std::setprecision(2) 
                  << static_cast<double>(totalSize) / (1024 * 1024) << " MB" << std::endl;
    }
}

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
    
    std::cout << "Welcome to Duplicate File Finder!" << std::endl;
    std::cout << "This tool helps you find and manage duplicate files using hash comparison." << std::endl;

    while (true) {
        displayMenu();
        
        // Clear any previous error states and handle input validation
        if (!(std::cin >> choice)) {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            std::cin.clear(); // Clear error flags
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore rest of line
            continue;
        }

        // Clear any leftover characters in buffer after reading choice
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: {
                std::cout << "Enter directory path to scan: ";
                std::getline(std::cin, directoryPath);
                
                try {
                    auto duplicateGroups = scanner.findDuplicates(directoryPath, algorithm, recursive);
                    
                    if (duplicateGroups.empty()) {
                        std::cout << "No duplicate files found!" << std::endl;
                        break;
                    }
                    
                    std::cout << "\nFound " << duplicateGroups.size() << " groups of duplicate files." << std::endl;
                    
                    if (defaultAction == DuplicateAction::SHOW_ONLY) {
                        // Interactive mode
                        for (const auto& group : duplicateGroups) {
                            handler.handleDuplicatesInteractive(group);
                        }
                    } else {
                        // Automatic mode
                        if (defaultAction == DuplicateAction::MOVE || defaultAction == DuplicateAction::HARD_LINK) {
                            std::cout << "Enter target directory: ";
                            std::getline(std::cin, targetDirectory);
                        }
                        
                        for (const auto& group : duplicateGroups) {
                            handler.handleDuplicates(group, defaultAction, targetDirectory);
                        }
                    }
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
                break;
            }
            case 2:
                displaySettings(algorithm, recursive, defaultAction);
                configureSettings(algorithm, recursive, defaultAction);
                break;
            case 3:
                showStatistics(scanner);
                break;
            case 4:
                std::cout << "Exiting..." << std::endl;
                return 0;
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
                break;
        }
    }

    return 0;
}