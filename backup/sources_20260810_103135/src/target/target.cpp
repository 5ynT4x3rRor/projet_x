#include <iostream>
#include <string>
#include "target.hpp"
#include "../scan/scan.hpp"

// On garde uniquement target ici (port_start/port_end sont gérés dans range.cpp)
std::string target = "";

// On supprime handle_clear_command d'ici puisqu'elle est dans clear/clear.cpp

void handle_commands() {
    std::string command;
    
    while (true) {
        std::cout << "\033[1;32mProjetX>\033[0m ";
        if (!std::getline(std::cin, command)) {
            break;
        }

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
        else if (command.rfind("set target : ", 0) == 0) {
            std::string candidate = command.substr(13);
            if (candidate.empty() || candidate.find(' ') != std::string::npos) {
                std::cout << "\033[1;31m[-] Erreur : Format de cible invalide (ex: set target : 127.0.0.1).\033[0m\n";
            } else {
                target = candidate;
                std::cout << "\033[1;32m[+] Cible définie sur : " << target << "\033[0m\n";
            }
        }
        // Note: Le "set range" est géré dans range.cpp ou ici selon ton architecture, 
        // mais si range.cpp le traite déjà, laisse-le le faire pour éviter les conflits !
        else if (!command.empty()) {
            std::cout << "\033[1;31m[-] Commande inconnue : " << command << "\033[0m\n";
        }
    }
}
