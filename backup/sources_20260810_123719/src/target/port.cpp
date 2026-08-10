#include <iostream>
#include <string>
#include "port.hpp"

int target_port = 0;

void handle_port_command(const std::string& command) {
    if (command.rfind("set port : ", 0) == 0) {
        try {
            target_port = std::stoi(command.substr(11));
            std::cout << "\033[1;32m[+] Port défini : " << target_port << "\033[0m\n";
        } catch (...) {
            std::cout << "\033[1;31m[-] Erreur : Format de port invalide.\033[0m\n";
        }
    } else if (command == "show port") {
        if (target_port == 0) std::cout << "\033[1;33m[-] Aucun port défini.\033[0m\n";
        else std::cout << "\033[1;36m[*] Port actuel : " << target_port << "\033[0m\n";
    }
}
