#include <iostream>
#include <string>
#include "range.hpp"

int port_start = 0;
int port_end = 0;

void handle_range_command(const std::string& command) {
    if (command.rfind("set range start : ", 0) == 0) {
        try {
            int p = std::stoi(command.substr(18));
            if (p > 0 && p <= 65535) {
                // Vérification croisée : start ne peut pas être supérieur à end (si end est déjà défini)
                if (port_end != 0 && p > port_end) {
                    std::cout << "\033[1;31m[-] Erreur : Le port de début ne peut pas être supérieur au port de fin (" << port_end << ").\033[0m\n";
                } else {
                    port_start = p;
                    std::cout << "\033[1;32m[+] Port de début défini : " << port_start << "\033[0m\n";
                }
            } else {
                std::cout << "\033[1;31m[-] Erreur : Port hors limites (1-65535).\033[0m\n";
            }
        } catch (...) {
            std::cout << "\033[1;31m[-] Erreur : Format de port invalide.\033[0m\n";
        }
    }
    else if (command.rfind("set range end : ", 0) == 0) {
        try {
            int p = std::stoi(command.substr(16));
            if (p > 0 && p <= 65535) {
                // Vérification croisée : end ne peut pas être inférieur à start
                if (port_start != 0 && p < port_start) {
                    std::cout << "\033[1;31m[-] Erreur : Le port de fin ne peut pas être inférieur au port de début (" << port_start << ").\033[0m\n";
                } else {
                    port_end = p;
                    std::cout << "\033[1;32m[+] Port de fin défini : " << port_end << "\033[0m\n";
                }
            } else {
                std::cout << "\033[1;31m[-] Erreur : Port hors limites (1-65535).\033[0m\n";
            }
        } catch (...) {
            std::cout << "\033[1;31m[-] Erreur : Format de port invalide.\033[0m\n";
        }
    }
}
