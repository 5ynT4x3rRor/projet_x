#include "set_target_ipv6.hpp"
#include "../utils/input_validator_ipv6.hpp"
#include <arpa/inet.h>
#include <iostream>

SetTargetIPv6::SetTargetIPv6() : targetIPv6("::1") {}

SetTargetIPv6::SetTargetIPv6(const std::string& initialTarget) {
    if (!setTarget(initialTarget)) {
        targetIPv6 = "::1";
    }
}

bool SetTargetIPv6::validateIPv6(const std::string& ip) const {
    char buf[sizeof(struct in6_addr)];
    // inet_pton convertit une chaîne IPv6 en structure binaire (renvoie 1 si valide, 0 ou -1 sinon)
    return inet_pton(AF_INET6, ip.c_str(), buf) == 1;
}

bool SetTargetIPv6::setTarget(const std::string& input) {
    if (validateIPv6(input)) {
        targetIPv6 = input;
        return true;
    }
    return false;
}

std::string SetTargetIPv6::getTarget() const {
    return targetIPv6;
}

// Fonction globale appelée par le main.cpp pour gérer la commande console
void handle_target_ipv6_command(const std::string& command)
{
    std::string prefix = "set_target_ipv6 ";
    std::string ip_str;

    if (command.rfind(prefix, 0) == 0) {
        ip_str = command.substr(prefix.length());
    } else {
        ip_str = command;
    }

    // Utilisation du validateur IPv6 dédié
    if (validate_ipv6_string(ip_str) == IPV6_VALID) {
        SetTargetIPv6 target(ip_str);
        std::cout << "[+] Cible IPv6 définie avec succès : " << target.getTarget() << std::endl;
    } else {
        std::cout << "[-] Erreur : Adresse IPv6 invalide !" << std::endl;
    }
}
