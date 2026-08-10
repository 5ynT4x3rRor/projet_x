#include <iostream>
#include <string>
#include <algorithm>
#include "target.hpp"
#include "port.hpp"
#include "range.hpp"
#include "../scan/scan.hpp"
#include "../clear/clear.hpp"

// Définitions globales
std::string target = "";

// Fonction utilitaire pour nettoyer les espaces superflus autour d'une chaîne
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

void handle_commands() {
    std::string raw_command;
    
    while (true) {
        std::cout << "\033[1;32mProjetX>\033[0m ";
        if (!std::getline(std::cin, raw_command)) {
            break;
        }

        // Nettoyage strict et sécurisation de la ligne de commande globale
        std::string command = trim(raw_command);

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
            std::string candidate = trim(command.substr(11));
            
            // Sécurisation stricte de l'IP / Cible : pas d'espaces internes, pas vide
            if (candidate.empty() || candidate.find(' ') != std::string::npos) {
                std::cout << "\033[1;31m[-] Erreur : Format de cible invalide (ex: set target 192.168.0.1).\033[0m\n";
            } else {
                target = candidate;
                std::cout << "\033[1;32m[+] Cible définie sur : " << target << "\033[0m\n";
            }
        }
        else if (command.rfind("set port ", 0) == 0) {
            std::string port_str = trim(command.substr(9));
            
            try {
                size_t idx;
                int p = std::stoi(port_str, &idx);
                // Vérifie qu'on a bien converti toute la chaîne (pas de caractères parasites derrière)
                if (idx == port_str.length() && p > 0 && p <= 65535) {
                    port_start = p;
                    port_end = p; 
                    std::cout << "\033[1;32m[+] Port défini sur : " << port_start << "\033[0m\n";
                } else {
                    std::cout << "\033[1;31m[-] Erreur : Port hors limites (1-65535) ou caractères non autorisés.\033[0m\n";
                }
            } catch (...) {
                std::cout << "\033[1;31m[-] Erreur : Format de port non numérique (ex: set port 80).\033[0m\n";
            }
        }
        else if (command.rfind("set range ", 0) == 0) {
            std::string range_str = trim(command.substr(10));
            
            size_t dash = range_str.find('-');
            if (dash != std::string::npos && dash > 0 && dash < range_str.length() - 1) {
                try {
                    std::string start_str = trim(range_str.substr(0, dash));
                    std::string end_str = trim(range_str.substr(dash + 1));
                    
                    size_t idx1, idx2;
                    port_start = std::stoi(start_str, &idx1);
                    port_end = std::stoi(end_str, &idx2);
                    
                    if (idx1 == start_str.length() && idx2 == end_str.length() &&
                        port_start > 0 && port_end <= 65535 && port_start <= port_end) {
                        std::cout << "\033[1;32m[+] Plage de ports définie : " << port_start << " à " << port_end << "\033[0m\n";
                    } else {
                        std::cout << "\033[1;31m[-] Erreur : Ports hors limites (1-65535), début > fin ou caractères invalides.\033[0m\n";
                    }
                } catch (...) {
                    std::cout << "\033[1;31m[-] Erreur : Valeurs de ports non numériques dans la plage.\033[0m\n";
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
