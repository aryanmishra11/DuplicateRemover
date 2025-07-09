@echo off
REM Test Script for Duplicate File Finder
REM This script helps validate the functionality of the duplicate finder tool

echo ========================================
echo Duplicate File Finder Test Script
echo ========================================
echo.

echo Test 1: Basic functionality test
echo Running duplicate finder on test_files directory...
echo.

cd /d "C:\Users\250018975\PPO\duplicate-file-finder\build\Debug"
echo Current directory: %CD%
echo.

echo Expected results:
echo - Should find 8 groups of duplicates
echo - Group 1: 3 text files (original_text.txt, duplicate_text_1.txt, duplicate_text_2.txt)
echo - Group 2: 3 binary files (test_image.bin, duplicate_image_1.bin, duplicate_image_2.bin)
echo - Group 3: 3 empty files (empty_file_1.txt, empty_file_2.txt, empty_file_3.txt)
echo - Group 4: 3 C++ files (hello.cpp, hello_copy.cpp, hello_nested.cpp)
echo - Group 5: 2 JSON files (config.json, config_backup.json)
echo - Group 6: 2 large text files (large_file.txt, large_file_copy.txt)
echo - Group 7: 2 shell scripts (test_script.sh, test_script_copy.sh)
echo - Group 8: 2 CSV files (data.csv, data_backup.csv)
echo.

echo Unique files (should NOT be duplicates):
echo - unique_file_1.txt
echo - unique_file_2.txt
echo - README.md
echo.

echo Press any key to run the duplicate finder...
pause >nul

echo Running DuplicateFileFinder.exe...
echo.
DuplicateFileFinder.exe

echo.
echo Test completed!
echo.
echo Additional tests you can perform:
echo 1. Test non-recursive mode (should find fewer duplicates)
echo 2. Test MD5 vs SHA256 algorithms
echo 3. Test move functionality (files will be moved to output directory)
echo 4. Test delete functionality (BE CAREFUL - files will be deleted!)
echo 5. Test hard link functionality

pause
