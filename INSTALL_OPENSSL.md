# Installing OpenSSL on Windows

## Option 1: Download Pre-compiled Binaries (Recommended)

1. Go to: https://slproweb.com/products/Win32OpenSSL.html
2. Download the appropriate version:
   - For 64-bit Windows: `Win64 OpenSSL v3.x.x` (Full version, not Light)
   - For 32-bit Windows: `Win32 OpenSSL v3.x.x` (Full version, not Light)
3. Run the installer and install to the default location (usually `C:\Program Files\OpenSSL-Win64\`)
4. Add OpenSSL to your PATH:
   - Open System Properties > Environment Variables
   - Add `C:\Program Files\OpenSSL-Win64\bin` to your PATH
   - Add `C:\Program Files\OpenSSL-Win64\lib` to your library path

## Option 2: Using Chocolatey (If installed)

```powershell
# Install Chocolatey first if not installed:
Set-ExecutionPolicy Bypass -Scope Process -Force; [System.Net.ServicePointManager]::SecurityProtocol = [System.Net.ServicePointManager]::SecurityProtocol -bor 3072; iex ((New-Object System.Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))

# Then install OpenSSL:
choco install openssl
```

## Option 3: Using vcpkg

```powershell
# Clone vcpkg
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

# Install OpenSSL
.\vcpkg install openssl:x64-windows
```

## Option 4: Using MSYS2

```bash
# In MSYS2 terminal:
pacman -S mingw-w64-x86_64-openssl
```

## After Installation

1. Verify installation:
   ```powershell
   openssl version
   ```

2. Update your build configuration:
   - For CMake: The CMakeLists.txt should automatically find OpenSSL
   - For Makefile: Update library paths if needed

## Build the Project

After installing OpenSSL, you can build the project:

```powershell
# Using CMake (Recommended)
mkdir build
cd build
cmake ..
cmake --build .

# Or using the Makefile
make
```

## Troubleshooting

If you get linker errors:
1. Make sure OpenSSL is in your PATH
2. Check that you installed the full version (not Light)
3. Verify that the architecture matches your compiler (x64 vs x86)
4. You may need to specify OpenSSL paths manually in CMake:
   ```
   cmake -DOPENSSL_ROOT_DIR="C:/Program Files/OpenSSL-Win64" ..
   ```
