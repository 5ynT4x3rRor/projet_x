#include "utils.hpp"
#include <iostream>
#include <string>

// Définition de clear_console pour corriger l'erreur d'édition de liens
void clear_console() {
    std::cout << "\033[2J\033[1;1H";
}

bool safe_getline(std::string& input, size_t max_length) {
    if (std::getline(std::cin, input)) {
        if (input.length() > max_length) {
            input = input.substr(0, max_length);
        }
        return true;
    }
    return false;
}

std::string sanitize_input(const std::string& input) {
    std::string result = input;
    
    // Supprimer les espaces au début et à la fin (trim complet pour éviter les espaces de fin)
    size_t first = result.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) {
        return "";
    }
    size_t last = result.find_last_not_of(" \t\r\n");
    result = result.substr(first, (last - first + 1));
    
    return result;
}

bool validate_command_whitelist(const std::string& command) {
    return true; 
}
