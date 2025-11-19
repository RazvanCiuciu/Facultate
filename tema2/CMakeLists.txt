# CMakeLists.txt
# Setează versiunea minimă necesară de CMake
cmake_minimum_required(VERSION 3.10)

# 1. Numele proiectului
project(Tema_CPP_Management_Memorie VERSION 1.0)

# 2. Setează standardul C++ la C++11 sau mai nou (recomandat C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 3. Specifică directoarele care conțin fișierele header (.h)
# CMake va căuta aici definițiile claselor/funcțiilor.
# ${PROJECT_SOURCE_DIR} este directorul rădăcină (unde e CMakeLists.txt)
include_directories(${PROJECT_SOURCE_DIR}/include)

# 4. Definește toate fișierele sursă (.cpp)
# Acestea sunt fișierele pe care CMake trebuie să le compileze.
set(SOURCE_FILES
    src/Main.cpp
    # Adaugă aici orice alte fișiere .cpp din src/
)

# 5. Creează executabilul
# Adaugă un nume pentru executabil și lista de fișiere sursă
add_executable(tema_executabil ${SOURCE_FILES})

# 6. (Opțional dar Recomandat) Specifică un director de ieșire
# Așa vă asigurați că executabilul este plasat în directorul 'build'
set_target_properties(tema_executabil PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/build"
)
