#include <iostream>
#include <string>
#include "utils/utils.hpp"
#include "utils/input_validator.hpp"
#include "clear/clear.hpp"
#include "scan/scan.hpp"


void print_banner() {
    std::cout << "\033[1;31m";
    std::cout << "\n\n";
    std::cout << "                 +-----------------------------------------+\n";
    std::cout << "                 |                                         |\n";
    std::cout << "                 |                PROJET X                 |\n";
    std::cout << "                 |            by Cr4sh0v3rr1d3             |\n";
    std::cout << "                 |                  v1.0                   |\n";
    std::cout << "                 |                                         |\n";
    std::cout << "                 +-----------------------------------------+\n";
    std::cout << "\033[0m";
}

void main_loop() {
    std::string command;
    const size_t MAX_CMD_LENGTH = 256;

    while (true) {
        std::cout << "\033[1;32mProjetX> \033[0m";
        
        // Utilisation de safe_getline pour éviter les dépassements de mémoire (BOF)
        if (!safe_getline(command, MAX_CMD_LENGTH)) {
            continue;
        }

        // Nettoyage des espaces superflus
        // Juste après sanitize_input dans main.cpp :
        command = sanitize_input(command);
        std::cout << "[DEBUG] Commande reçue après sanitize : '" << command << "'\n";


        if (command.empty()) {
            continue;
        }

        // Commandes de sortie
        if (command == "exit" || command == "quit") {
            std::cout << "[*] Fermeture de Projet X...\n";
            break;
        }
        // Commande de nettoyage
        else if (command == "clear" || command == "cls") {
            handle_clear_command();
        }
        // Validation par liste blanche stricte pour tout le reste ("Niet" si non reconnu)
        else if (!validate_command_whitelist(command)) {
            std::cout << "\033[1;31m[-] Erreur : Commande inconnue ou module non actif. Niet !\033[0m\n";
        }
        else {
            // Traitement des commandes autorisées par la whitelist
            if (command == "help") {
                std::cout << "[+] Commandes actives : help, exit, clear, scan, set target, set port, proxy\n";
            }
            else if (command == "scan") {
                std::cout << "[+] Exécution du module de scan...\n";
                execute_scan();
            }
            else if (command.rfind("set target", 0) == 0 || command.rfind("set port", 0) == 0 || command.rfind("proxy", 0) == 0) {
                std::cout << "[+] Module en cours d'exécution : " << command << "\n";
            }
        }
    }
}

int main() {
    clear_console();
    print_banner();
    
    main_loop();
    
    return 0;
}
