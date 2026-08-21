#!/bin/bash
OUTPUT="bin/projet_x"
SRC_DIR="src"
echo "[+] Compilation de Projet X..."
g++ -std=c++17 \
    $SRC_DIR/main.cpp \
    $SRC_DIR/utils/input_validator_ipv4.cpp \
    $SRC_DIR/utils/input_validator_ipv6.cpp \
    $SRC_DIR/utils/input_validator_port.cpp \
    $SRC_DIR/utils/utils.cpp \
    $SRC_DIR/clear/clear.cpp \
    $SRC_DIR/scan/scan.cpp \
    $SRC_DIR/proxy/proxy.cpp \
    $SRC_DIR/target/set_target_ipv4.cpp \
    $SRC_DIR/target/set_target_ipv6.cpp \
    $SRC_DIR/target/set_port.cpp \
    $SRC_DIR/target/set_range_port.cpp \
    -I$SRC_DIR \
    -I$SRC_DIR/utils \
    -I$SRC_DIR/target \
    -I$SRC_DIR/scan \
    -o $OUTPUT
