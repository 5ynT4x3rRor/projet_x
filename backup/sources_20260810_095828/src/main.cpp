#include <iostream>
#include <string>
#include "utils/utils.hpp"
#include "target/target.hpp"
#include "clear/clear.hpp"

void print_banner() {
    std::cout << "\033[1;31m";
    std::cout << "\n\n";
    std::cout << "                 +-----------------------------------------+\n";
    std::cout << "                 |                                         |\n";
    std::cout << "                 |                 PROJET X                |\n";
    std::cout << "                 |            by Cr4sh0v3rr1d3             |\n";
    std::cout << "                 |                 v1.0                    |\n";
    std::cout << "                 |                                         |\n";
    std::cout << "                 +-----------------------------------------+\n";
    std::cout << "\033[0m";
}

void main_loop() {
    std::string command;
    while (true) {
        std::cout << "\033[1;32mProjetX> \033[0m";
        std::getline(std::cin, command);

        if (command == "exit" || command == "quit") {
            break;
        }
        else if (command == "clear" || command == "cls") {
            handle_clear_command();
        }
        else {
            // On délègue les autres commandes au module target (ou autres modules futurs)
            // Pour l'instant, target gère ses propres commandes
            // (Tu peux adapter selon la structure de ta fonction handle_commands)
        }
    }
}

int main() {
    clear_console();
    print_banner();
    
    handle_commands(); // Ou ta boucle principale
    
    return 0;
}
