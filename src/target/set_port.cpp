#include "set_port.hpp"
#include "../utils/input_validator_port.hpp"
#include <iostream>

void handle_set_port(const std::string& port_arg) {
    if (!validate_port(port_arg)) {
        std::cout << "[-] Erreur : Port invalide (doit être un nombre entre 1 et 65535).\n";
        return;
    }

    std::cout << "[+] Port verrouillé avec succès : " << port_arg << "\n";
    // Suite de ta logique...
}
