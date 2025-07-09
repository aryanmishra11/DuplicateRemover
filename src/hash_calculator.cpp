#include "hash_calculator.h"
#include <openssl/evp.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

std::string HashCalculator::calculateMD5(const std::string& filePath) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        throw std::runtime_error("Failed to create MD5 context");
    }

    if (EVP_DigestInit_ex(mdctx, EVP_md5(), nullptr) != 1) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to initialize MD5");
    }

    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    char buffer[8192];
    while (file.read(buffer, sizeof(buffer))) {
        if (EVP_DigestUpdate(mdctx, buffer, file.gcount()) != 1) {
            EVP_MD_CTX_free(mdctx);
            throw std::runtime_error("Failed to update MD5 hash");
        }
    }
    if (file.gcount() > 0) {
        if (EVP_DigestUpdate(mdctx, buffer, file.gcount()) != 1) {
            EVP_MD_CTX_free(mdctx);
            throw std::runtime_error("Failed to update MD5 hash");
        }
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    if (EVP_DigestFinal_ex(mdctx, hash, &hash_len) != 1) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to finalize MD5 hash");
    }

    EVP_MD_CTX_free(mdctx);

    std::ostringstream result;
    for (unsigned int i = 0; i < hash_len; ++i) {
        result << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return result.str();
}

std::string HashCalculator::calculateSHA256(const std::string& filePath) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        throw std::runtime_error("Failed to create SHA256 context");
    }

    if (EVP_DigestInit_ex(mdctx, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to initialize SHA256");
    }

    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Unable to open file: " + filePath);
    }

    char buffer[8192];
    while (file.read(buffer, sizeof(buffer))) {
        if (EVP_DigestUpdate(mdctx, buffer, file.gcount()) != 1) {
            EVP_MD_CTX_free(mdctx);
            throw std::runtime_error("Failed to update SHA256 hash");
        }
    }
    if (file.gcount() > 0) {
        if (EVP_DigestUpdate(mdctx, buffer, file.gcount()) != 1) {
            EVP_MD_CTX_free(mdctx);
            throw std::runtime_error("Failed to update SHA256 hash");
        }
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    if (EVP_DigestFinal_ex(mdctx, hash, &hash_len) != 1) {
        EVP_MD_CTX_free(mdctx);
        throw std::runtime_error("Failed to finalize SHA256 hash");
    }

    EVP_MD_CTX_free(mdctx);

    std::ostringstream result;
    for (unsigned int i = 0; i < hash_len; ++i) {
        result << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return result.str();
}

std::string HashCalculator::calculateHash(const std::string& filePath, HashAlgorithm algorithm) {
    switch (algorithm) {
        case HashAlgorithm::MD5:
            return calculateMD5(filePath);
        case HashAlgorithm::SHA256:
            return calculateSHA256(filePath);
        default:
            throw std::invalid_argument("Unsupported hash algorithm");
    }
}

bool HashCalculator::compareFiles(const std::string& filePath1, const std::string& filePath2, HashAlgorithm algorithm) {
    std::string hash1 = calculateHash(filePath1, algorithm);
    std::string hash2 = calculateHash(filePath2, algorithm);
    return hash1 == hash2;
}