#!/bin/bash
OUTPUT="bin/projet_x"
SRC_DIR="src"
echo "[+] Compilation de Projet X..."
g++ -std=c++17 \
    $SRC_DIR/main.cpp \
    $SRC_DIR/utils/input_validator.cpp \
    $SRC_DIR/utils/utils.cpp \
    $SRC_DIR/clear/clear.cpp \
    $SRC_DIR/scan/scan.cpp \
    $SRC_DIR/proxy/proxy.cpp \
    $SRC_DIR/target/*.cpp \
    -I$SRC_DIR \
    -I$SRC_DIR/utils \
    -I$SRC_DIR/target \
    -I$SRC_DIR/scan \
    -I$SRC_DIR/proxy \
    -I$SRC_DIR/clear \
    -o $OUTPUT
if [ $? -eq 0 ]; then
    echo "[+] Compilation réussie : $OUTPUT"
    chmod +x $OUTPUT
else
    echo "[-] Erreur lors de la compilation."
    exit 1
fi
