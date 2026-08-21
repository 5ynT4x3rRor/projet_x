#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstdint>
#include "set_target_ipv4.hpp"

/* Variables globales partagées (sans static pour être accessibles par les autres modules) */
ipv4_target_t current_target = {
    {0, 0, 0, 0},
    {0}
};

bool target_defined = false;

/**
 * @brief Validates that a string is a valid decimal number (0-255)
 */
static int validate_octet(const char *str, size_t len)
{
    if (!str || len == 0 || len > 3) {
        return -1;
    }

    for (size_t i = 0; i < len; i++) {
        if (!std::isdigit((unsigned char)str[i])) {
            return -1;
        }
    }

    char temp[4] = {0};
    std::strncpy(temp, str, len);
    temp[len] = '\0';

    int value = std::atoi(temp);

    if (value < 0 || value > 255) {
        return -1;
    }

    return value;
}

/**
 * @brief Parses an IPv4 address into an octet array
 */
static ipv4_target_status_t parse_ipv4(const char *input, uint8_t *octets)
{
    if (!input || !octets) {
        return IPV4_TARGET_ERR_NULL_INPUT;
    }

    size_t input_len = std::strlen(input);
    if (input_len == 0 || input_len > 15) {
        return IPV4_TARGET_ERR_BUFFER_OVERFLOW;
    }

    int octet_count = 0;
    const char *start = input;
    const char *ptr = input;

    while (*ptr != '\0') {
        if (*ptr == '.') {
            size_t len = ptr - start;
            int val = validate_octet(start, len);
            if (val < 0) {
                return IPV4_TARGET_ERR_INVALID_OCTET;
            }
            if (octet_count >= 4) {
                return IPV4_TARGET_ERR_INVALID_FORMAT;
            }
            octets[octet_count++] = (uint8_t)val;
            start = ptr + 1;
        }
        ptr++;
    }

    // Dernier octet
    size_t len = ptr - start;
    int val = validate_octet(start, len);
    if (val < 0) {
        return IPV4_TARGET_ERR_INVALID_OCTET;
    }
    if (octet_count != 3) {
        return IPV4_TARGET_ERR_INVALID_FORMAT;
    }
    octets[octet_count] = (uint8_t)val;

    return IPV4_TARGET_OK;
}

// Fonction pour gérer la commande set target
void handle_target_ipv4_command(const std::string& command)
{
    std::string prefix = "set target_ipv4"; // Modification du prefix suite la modification de la sécuritée des entrées (set target ---> set_target_ipv4) 
    std::string ip_str;

    if (command.rfind(prefix, 0) == 0) {
        ip_str = command.substr(prefix.length());
    } else {
        ip_str = command;
    }

    // Nettoyage des espaces superflus
    ip_str.erase(0, ip_str.find_first_not_of(" \t\n\r"));
    ip_str.erase(ip_str.find_last_not_of(" \t\n\r") + 1);

    if (ip_str.empty()) {
        handle_target_ipv4_error(IPV4_TARGET_ERR_EMPTY_INPUT);
        return;
    }

    uint8_t temp_octets[4];
    ipv4_target_status_t status = parse_ipv4(ip_str.c_str(), temp_octets);

    if (status != IPV4_TARGET_OK) {
        handle_target_ipv4_error(status);
        return;
    }

    // Mise à jour de la cible globale
    for (int i = 0; i < 4; i++) {
        current_target.octets[i] = temp_octets[i];
    }
    
    std::strncpy(current_target.ip_str, ip_str.c_str(), sizeof(current_target.ip_str) - 1);
    current_target.ip_str[sizeof(current_target.ip_str) - 1] = '\0';

    target_defined = true;

    std::cout << "[+] Cible IPv4 verrouillée : " << current_target.ip_str << "\n";
}

void handle_target_ipv4_error(ipv4_target_status_t status)
{
    switch (status) {
        case IPV4_TARGET_ERR_NULL_INPUT:
        case IPV4_TARGET_ERR_EMPTY_INPUT:
            std::cout << "[-] Erreur : Entrée vide ou nulle.\n";
            break;
        case IPV4_TARGET_ERR_INVALID_FORMAT:
            std::cout << "[-] Erreur : Format IPv4 invalide (ex attendu : X.X.X.X).\n";
            break;
        case IPV4_TARGET_ERR_INVALID_OCTET:
            std::cout << "[-] Erreur : Octet invalide (doit être entre 0 et 255).\n";
            break;
        case IPV4_TARGET_ERR_BUFFER_OVERFLOW:
            std::cout << "[-] Erreur : Dépassement de tampon (chaîne trop longue).\n";
            break;
        default:
            std::cout << "[-] Erreur IPv4 inconnue.\n";
            break;
    }
}

void display_target_ipv4(const ipv4_target_t *target)
{
    if (target && target_defined) {
        std::cout << "[+] Cible IPv4 actuelle : " << target->ip_str << "\n";
    } else {
        std::cout << "[-] Aucune cible IPv4 définie.\n";
    }
}

/**
 * @brief Getter public pour récupérer la cible IPv4 depuis d'autres modules (ex: scan)
 */
ipv4_target_t* get_target_ipv4(void)
{
    if (!target_defined) {
        return nullptr;
    }
    return &current_target;
}
