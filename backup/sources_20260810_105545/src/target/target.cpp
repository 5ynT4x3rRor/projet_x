#include <iostream>
#include <string>
#include "target.hpp"
#include "port.hpp" // Pour extern int port_start; si besoin
#include "../scan/scan.hpp"
#include "../clear/clear.hpp"

// Définitions globales
std::string target = "";
// int port_start = 0; // Décommente si elle n'est déclarée nulle part ailleurs

void handle_commands() {
    std::string command;
    
    while (true) {
        std::cout << "\033[1;32mProjetX>\033[0m ";
        if (!std::getline(std::cin, command)) {
            break;
        }

        // Nettoyage strict du retour chariot
        while (!command.empty() && (command.back() == '\r' || command.back() == '\n')) {
            command.pop_back();
        }

        if (command == "exit" || command == "quit") {
            std::cout << "[*] Fermeture du programme...\n";
            break;
        }
        else if (command == "clear" || command == "cls") {
            handle_clear_command();
        }
        else if (command == "scan") {
            execute_scan();
        }
        else if (command.rfind("set ", 0) == 0) {
            std::string arg = command.substr(4);
            
            if (arg.rfind("port ", 0) == 0) {
                std::string port_str = arg.substr(5);
                try {
                    int p = std::stoi(port_str);
                    if (p > 0 && p <= 65535) {
                        port_start = p;
                        std::cout << "\033[1;32m[+] Port défini sur : " << port_start << "\033[0m\n";
                    } else {
                        std::cout << "\033[1;31m[-] Erreur : Port hors limites (1-65535).\033[0m\n";
                    }
                } catch (...) {
                    std::cout << "\033[1;31m[-] Erreur : Format de port non numérique (ex: set port 80).\033[0m\n";
                }
            }
            else {
                std::string candidate = arg;
                if (candidate.empty() || candidate.find(' ') != std::string::npos) {
                    std::cout << "\033[1;31m[-] Erreur : Format de cible invalide (ex: set 192.168.0.1).\033[0m\n";
                } else {
                    target = candidate;
                    std::cout << "\033[1;32m[+] Cible définie sur : " << target << "\033[0m\n";
                }
            }
        }
        else if (!command.empty()) {
            std::cout << "\033[1;31m[-] Commande inconnue : " << command << "\033[0m\n";
        }
    }
}
