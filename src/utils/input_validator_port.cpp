#include "input_validator_port.hpp"
#include <iostream>
#include <cctype>

bool validate_port(const std::string& port_str) {
    if (port_str.empty() || port_str.length() > 5) {
        return false;
    }

    // Vérifier que chaque caractère est bien un chiffre
    for (char c : port_str) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }

    // Convertir et vérifier la plage valide des ports (1 - 65535)
    try {
        int port = std::stoi(port_str);
        if (port < 1 || port > 65535) {
            return false;
        }
    } catch (...) {
        return false;
    }

    return true;
}
