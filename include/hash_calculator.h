#ifndef HASH_CALCULATOR_H
#define HASH_CALCULATOR_H

#include <string>
#include <openssl/evp.h>

enum class HashAlgorithm {
    MD5,
    SHA256
};

class HashCalculator {
public:
    static std::string calculateMD5(const std::string& filePath);
    static std::string calculateSHA256(const std::string& filePath);
    static std::string calculateHash(const std::string& filePath, HashAlgorithm algorithm);
    static bool compareFiles(const std::string& filePath1, const std::string& filePath2, HashAlgorithm algorithm);
};

#endif // HASH_CALCULATOR_H