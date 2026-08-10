#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "target.hpp"
#include "port.hpp"
#include "range.hpp"
#include "../clear/clear.hpp"

std::string target = "";

// Fonction utilitaire pour valider une adresse IPv4
bool is_valid_ipv4(const std::string &ip) {
    std::stringstream ss(ip);
    std::string segment;
    std::vector<std::string> segments;
    
    while (std::getline(ss, segment, '.')) {
        segments.push_back(segment);
    }
    
    if (segments.size() != 4) return false;
    
    for (const auto &seg : segments) {
        if (seg.empty() || (seg.length() > 1 && seg[0] == '0')) return false; // Pas de zéros superflus (ex: 192.168.01.1)
        for (char c : seg) {
            if (!std::isdigit(c)) return false; // Uniquement des chiffres
        }
        int val = std::stoi(seg);
        if (val < 0 || val > 255) return false;
    }
    return true;
}

void handle_commands() {
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
        else if (command.rfind("set target : ", 0) == 0) {
            std::string candidate = command.substr(13);
            
            // Nettoyage basique des espaces de fin si besoin
            while (!candidate.empty() && (candidate.back() == ' ' || candidate.back() == '\r')) {
                candidate.pop_back();
            }

            if (is_valid_ipv4(candidate)) {
                target = candidate;
                std::cout << "\033[1;32m[+] Cible validée et définie : " << target << "\033[0m\n";
            } else {
                std::cout << "\033[1;31m[-] Erreur : Adresse IP invalide.\033[0m\n";
            }
        } 
        else if (command == "show target") {
            if (target.empty()) std::cout << "\033[1;33m[-] Aucune cible.\033[0m\n";
            else std::cout << "\033[1;36m[*] Cible : " << target << "\033[0m\n";
        } 
        else if (command.rfind("set port : ", 0) == 0 || command == "show port") {
            handle_port_command(command);
        } 
        else if (command.rfind("set range start : ", 0) == 0 || 
                   command.rfind("set range end : ", 0) == 0 || 
                   command == "show range") {
            handle_range_command(command);
        } 
        else if (!command.empty()) {
            std::cout << "\033[1;31m[-] Commande inconnue.\033[0m\n";
        }
    }
}
