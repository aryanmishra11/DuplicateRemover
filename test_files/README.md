# Test Files Directory

This directory contains various test files to test the Duplicate File Finder tool.

## File Categories:

1. **Text Files**: Multiple identical text files in different locations
2. **Binary Files**: Binary file duplicates to test binary comparison
3. **Empty Files**: Empty files to test edge cases
4. **Code Files**: C++ source files with identical content
5. **Configuration Files**: JSON files with same configuration
6. **Unique Files**: Files with no duplicates
7. **Large Files**: Larger text files to test performance
8. **Scripts**: Shell scripts with identical content
9. **Data Files**: CSV files with same data

## Expected Duplicates:
- original_text.txt, duplicate_text_1.txt, duplicate_text_2.txt
- test_image.bin, duplicate_image_1.bin, duplicate_image_2.bin
- empty_file_1.txt, empty_file_2.txt, empty_file_3.txt
- hello.cpp, hello_copy.cpp, hello_nested.cpp
- config.json, config_backup.json
- large_file.txt, large_file_copy.txt
- test_script.sh, test_script_copy.sh
- data.csv, data_backup.csv

## Testing Instructions:
1. Run the duplicate finder on this directory
2. Test both recursive and non-recursive modes
3. Test different hash algorithms (MD5 vs SHA256)
4. Test different actions (show, delete, move, hard link)
