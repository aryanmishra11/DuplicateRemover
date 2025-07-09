cmake_minimum_required(VERSION 3.10)

project(DuplicateFileFinder)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set OpenSSL root directory for Windows
if(WIN32)
    set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
    set(OPENSSL_USE_STATIC_LIBS TRUE)
endif()

# Find OpenSSL
find_package(OpenSSL REQUIRED)

include_directories(include)

# Source files
file(GLOB SOURCES "src/*.cpp")

# Create executable
add_executable(DuplicateFileFinder ${SOURCES})

# Link OpenSSL libraries
target_link_libraries(DuplicateFileFinder OpenSSL::SSL OpenSSL::Crypto)