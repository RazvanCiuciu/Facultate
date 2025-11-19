#!/bin/bash

# Definește variabila pentru directorul de build
BUILD_DIR="build"
# Definește numele executabilului (trebuie să fie același cu cel din CMakeLists.txt)
EXECUTABLE_NAME="tema_executabil" # <-- MODIFICĂ AICI!

# --- PASUL 1: Curățarea directorului de build (Opțional) ---
echo "Curățare director $BUILD_DIR..."
# Asigură-te că nu ștergi fișiere esențiale din alte proiecte!
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

# --- PASUL 2: Generarea fișierelor de build (CMake) ---
echo "Generare fișiere de build..."
# -S . specifică directorul sursă (curent)
# -B $BUILD_DIR specifică directorul de build
cmake -S . -B "$BUILD_DIR"

# Verifică dacă comanda CMake a reușit
if [ $? -ne 0 ]; then
    echo "❌ Eroare: Generarea fișierelor de build a eșuat."
    exit 1
fi

# --- PASUL 3: Compilarea proiectului ---
echo "Compilare proiect..."
cmake --build "$BUILD_DIR"

# Verifică dacă compilarea a reușit
if [ $? -ne 0 ]; then
    echo "❌ Eroare: Compilarea proiectului a eșuat."
    exit 1
fi

cd "$BUILD_DIR"
make

# --- PASUL 4: Rularea executabilului ---
echo "✅ Compilare reușită! Rulare program..."
# Executabilul va fi, de obicei, în BUILD_DIR, dar structura exactă depinde
# de generatorul CMake (de ex. Makefiles sau Ninja)
./"$BUILD_DIR"/"$EXECUTABLE_NAME"

# Verifică dacă rularea a reușit (opțional)
if [ $? -ne 0 ]; then
    echo "⚠️ Avertisment: Programul a returnat un cod de eroare la rulare."
fi

echo "--- SFÂRȘIT ---"
