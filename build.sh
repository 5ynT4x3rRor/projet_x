#!/bin/bash

# Configuration des dossiers
SRC_DIR="src"
BIN_DIR="bin"
BACKUP_DIR="backup"
BINARY_NAME="projet_x"

# Création des dossiers si besoin
mkdir -p "$BIN_DIR" "$BACKUP_DIR"

# 1. Sauvegarde du dernier binaire s'il existe et n'est pas vide
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
if [ -f "$BIN_DIR/$BINARY_NAME" ]; then
    cp "$BIN_DIR/$BINARY_NAME" "$BACKUP_DIR/${BINARY_NAME}_$TIMESTAMP"
    echo "[+] Ancien binaire sauvegardé dans $BACKUP_DIR/${BINARY_NAME}_$TIMESTAMP"
fi

# 2. Compilation avec -Isrc pour gérer proprement les includes
echo "[*] Compilation en cours..."
g++ -Isrc \
    "$SRC_DIR/main.cpp" \
    "$SRC_DIR/utils/utils.cpp" \
    "$SRC_DIR/target/target.cpp" \
    "$SRC_DIR/target/port.cpp" \
    "$SRC_DIR/target/range.cpp" \
    "$SRC_DIR/clear/clear.cpp" \
    "$SRC_DIR/scan/scan.cpp" \
    -o "$BIN_DIR/$BINARY_NAME"

# 3. Vérification, sauvegarde des sources et lancement
if [ $? -eq 0 ]; then
    echo "[+] Compilation réussie ! Binaire généré dans $BIN_DIR/$BINARY_NAME"
    
    # Sauvegarde légère des sources de cette version
    SOURCE_BACKUP_DIR="$BACKUP_DIR/sources_$TIMESTAMP"
    mkdir -p "$SOURCE_BACKUP_DIR"
    cp -r "$SRC_DIR" "$SOURCE_BACKUP_DIR/"
    echo "[+] Sources sauvegardées dans $SOURCE_BACKUP_DIR"

    echo "[*] Lancement de l'application..."
    echo "-----------------------------------"
    "./$BIN_DIR/$BINARY_NAME"
else
    echo "[-] Erreur lors de la compilation, camarade !"
    exit 1
fi
