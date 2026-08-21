#include <iostream>
#include <string>
#include "utils/utils.hpp"
#include "utils/input_validator_ipv4.hpp"
#include "utils/input_validator_ipv6.hpp"
#include "clear/clear.hpp"
#include "scan/scan.hpp"
#include "target/set_target_ipv4.hpp"
#include "target/set_target_ipv6.hpp"
#include "target/set_port.hpp"

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

        if (!safe_getline(command, MAX_CMD_LENGTH)) {
            continue;
        }

        command = sanitize_input(command);
        std::cout << "[DEBUG] Commande reçue après sanitize : '" << command << "'\n";

        if (command.empty()) {
            continue;
        }

        if (command == "exit" || command == "quit") {
            std::cout << "[*] Fermeture de Projet X...\n";
            break;
        }
        else if (command == "clear" || command == "cls") {
            handle_clear_command();
        }
        else if (!validate_command_whitelist(command)) {
            std::cout << "\033[1;31m[-] Erreur : Commande inconnue ou module non actif. Niet !\033[0m\n";
        }
        else {
            if (command == "help") {
                std::cout << "[+] Commandes actives : help, exit, clear, scan, set target_ipv4, set_target_ipv6, set port, proxy\n";
            }
            else if (command == "scan") {
                std::cout << "[+] Exécution du module de scan...\n";
                execute_scan();
            }
            // Router proprement vers les fonctions dédiées :
            else if (command.rfind("set target_ipv4", 0) == 0) {
                handle_target_ipv4_command(command);
            }
            else if (command.rfind("set_target_ipv6", 0) == 0) {
                handle_target_ipv6_command(command);
            }
            else if (command.rfind("set port", 0) == 0) {
                std::string port_arg = command.substr(8);
                handle_set_port(port_arg);
            }
            else if (command.rfind("proxy", 0) == 0) {
                std::cout << "[+] Module proxy en cours de traitement...\n";
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
