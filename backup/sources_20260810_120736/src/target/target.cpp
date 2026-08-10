#include <iostream>
#include <string>
#include "target.hpp"
#include "port.hpp"
#include "range.hpp"
#include "../scan/scan.hpp"
#include "../clear/clear.hpp"

// Définitions globales
std::string target = "";

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
        else if (command.rfind("set target ", 0) == 0) {
            std::string candidate = command.substr(11);
            
            if (candidate.empty() || candidate.find(' ') != std::string::npos) {
                std::cout << "\033[1;31m[-] Erreur : Format de cible invalide (ex: set target 192.168.0.1).\033[0m\n";
            } else {
                target = candidate;
                std::cout << "\033[1;32m[+] Cible définie sur : " << target << "\033[0m\n";
            }
        }
        else if (command.rfind("set port ", 0) == 0) {
            std::string port_str = command.substr(9);
            
            try {
                int p = std::stoi(port_str);
                if (p > 0 && p <= 65535) {
                    port_start = p;
                    port_end = p; // Met aussi port_end au même niveau pour un port unique
                    std::cout << "\033[1;32m[+] Port défini sur : " << port_start << "\033[0m\n";
                } else {
                    std::cout << "\033[1;31m[-] Erreur : Port hors limites (1-65535).\033[0m\n";
                }
            } catch (...) {
                std::cout << "\033[1;31m[-] Erreur : Format de port non numérique (ex: set port 80).\033[0m\n";
            }
        }
        else if (command.rfind("set range ", 0) == 0) {
            std::string range_str = command.substr(10);
            
            size_t dash = range_str.find('-');
            if (dash != std::string::npos && dash > 0 && dash < range_str.length() - 1) {
                try {
                    port_start = std::stoi(range_str.substr(0, dash));
                    port_end = std::stoi(range_str.substr(dash + 1));
                    
                    if (port_start > 0 && port_end <= 65535 && port_start <= port_end) {
                        std::cout << "\033[1;32m[+] Plage de ports définie : " << port_start << " à " << port_end << "\033[0m\n";
                    } else {
                        std::cout << "\033[1;31m[-] Erreur : Ports hors limites (1-65535) ou début > fin.\033[0m\n";
                    }
                } catch (...) {
                    std::cout << "\033[1;31m[-] Erreur : Valeurs de ports non numériques.\033[0m\n";
                }
            } else {
                std::cout << "\033[1;31m[-] Erreur : Format strict requis (ex: set range 1-1024).\033[0m\n";
            }
        }
        else if (!command.empty()) {
            std::cout << "\033[1;31m[-] Commande inconnue : " << command << "\033[0m\n";
        }
    }
}
