#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstdint>
#include "set_range_ipv4.hpp"

/* Variable globale pour stocker la plage actuelle */
static ipv4_range_t current_range = {
    .start_octets = {0, 0, 0, 0},
    .end_octets = {0, 0, 0, 0},
    .start_str = {0},
    .end_str = {0},
    .cidr_prefix = -1
};

static bool range_defined = false;

/**
 * @brief Valide qu'une chaîne est un nombre décimal valide (0-255)
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
 * @brief Parse une adresse IPv4 dans un tableau d'octets
 */
static ipv4_range_status_t parse_ipv4(const char *input, uint8_t *octets)
{
    if (!input || !octets) {
        return IPV4_RANGE_ERR_NULL_INPUT;
    }

    size_t input_len = std::strlen(input);
    if (input_len == 0 || input_len > 15) {
        return IPV4_RANGE_ERR_BUFFER_OVERFLOW;
    }

    int parsed_octets[4] = {-1, -1, -1, -1};
    int octet_index = 0;
    size_t current_octet_len = 0;
    const char *current_octet_start = input;

    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];

        if (c == '.' || c == '\0') {
            if (current_octet_len == 0) {
                return IPV4_RANGE_ERR_INVALID_FORMAT;
            }

            int octet_value = validate_octet(current_octet_start, current_octet_len);
            if (octet_value < 0) {
                return IPV4_RANGE_ERR_INVALID_OCTET;
            }

            parsed_octets[octet_index] = octet_value;
            octet_index++;

            if (c == '\0') {
                if (octet_index != 4) {
                    return IPV4_RANGE_ERR_INVALID_FORMAT;
                }
                break;
            }

            if (octet_index > 4) {
                return IPV4_RANGE_ERR_INVALID_FORMAT;
            }

            current_octet_start = &input[i + 1];
            current_octet_len = 0;
        } else if (std::isdigit((unsigned char)c)) {
            current_octet_len++;
        } else {
            return IPV4_RANGE_ERR_INVALID_FORMAT;
        }
    }

    if (octet_index != 4) {
        return IPV4_RANGE_ERR_INVALID_FORMAT;
    }

    for (int i = 0; i < 4; i++) {
        octets[i] = (uint8_t)parsed_octets[i];
    }

    return IPV4_RANGE_OK;
}

/**
 * @brief Calcule l'adresse de fin à partir d'une adresse et d'un préfixe CIDR
 */
static ipv4_range_status_t calculate_cidr_range(const uint8_t *start_ip, int prefix, uint8_t *end_ip)
{
    if (prefix < 0 || prefix > 32) {
        return IPV4_RANGE_ERR_INVALID_CIDR;
    }

    /* Copier l'adresse de départ */
    for (int i = 0; i < 4; i++) {
        end_ip[i] = start_ip[i];
    }

    /* Calculer le masque et l'adresse de fin */
    if (prefix < 32) {
        int bits_to_set = 32 - prefix;
        uint32_t mask = (0xFFFFFFFFU << bits_to_set) & 0xFFFFFFFFU;
        
        uint32_t ip_value = ((uint32_t)start_ip[0] << 24) |
                           ((uint32_t)start_ip[1] << 16) |
                           ((uint32_t)start_ip[2] << 8) |
                           ((uint32_t)start_ip[3]);

        uint32_t end_value = ip_value | (~mask);

        end_ip[0] = (end_value >> 24) & 0xFF;
        end_ip[1] = (end_value >> 16) & 0xFF;
        end_ip[2] = (end_value >> 8) & 0xFF;
        end_ip[3] = end_value & 0xFF;
    }

    return IPV4_RANGE_OK;
}

/**
 * @brief Convertit un tableau d'octets en chaîne IPv4
 */
static ipv4_range_status_t octets_to_string(const uint8_t *octets, char *str, size_t str_len)
{
    if (!octets || !str || str_len < 16) {
        return IPV4_RANGE_ERR_BUFFER_OVERFLOW;
    }

    int result = std::snprintf(str, str_len, "%u.%u.%u.%u",
                               octets[0], octets[1], octets[2], octets[3]);

    if (result < 0 || result >= (int)str_len) {
        return IPV4_RANGE_ERR_BUFFER_OVERFLOW;
    }

    return IPV4_RANGE_OK;
}

/**
 * @brief Parse et valide une plage d'adresses IPv4
 */
static ipv4_range_status_t parse_range(const std::string& range_str, ipv4_range_t *range)
{
    if (!range) {
        return IPV4_RANGE_ERR_NULL_INPUT;
    }

    size_t dash_pos = range_str.find('-');
    size_t slash_pos = range_str.find('/');

    /* Format plage : 192.168.1.0-192.168.1.255 */
    if (dash_pos != std::string::npos && slash_pos == std::string::npos) {
        std::string start_str = range_str.substr(0, dash_pos);
        std::string end_str = range_str.substr(dash_pos + 1);

        ipv4_range_status_t status = parse_ipv4(start_str.c_str(), range->start_octets);
        if (status != IPV4_RANGE_OK) {
            return status;
        }

        status = parse_ipv4(end_str.c_str(), range->end_octets);
        if (status != IPV4_RANGE_OK) {
            return status;
        }

        /* Vérifier que start <= end */
        uint32_t start_val = ((uint32_t)range->start_octets[0] << 24) |
                            ((uint32_t)range->start_octets[1] << 16) |
                            ((uint32_t)range->start_octets[2] << 8) |
                            ((uint32_t)range->start_octets[3]);

        uint32_t end_val = ((uint32_t)range->end_octets[0] << 24) |
                          ((uint32_t)range->end_octets[1] << 16) |
                          ((uint32_t)range->end_octets[2] << 8) |
                          ((uint32_t)range->end_octets[3]);

        if (start_val > end_val) {
            return IPV4_RANGE_ERR_START_GREATER_END;
        }

        range->cidr_prefix = -1;
    }
    /* Format CIDR : 192.168.1.0/24 */
    else if (slash_pos != std::string::npos && dash_pos == std::string::npos) {
        std::string ip_str = range_str.substr(0, slash_pos);
        std::string prefix_str = range_str.substr(slash_pos + 1);

        ipv4_range_status_t status = parse_ipv4(ip_str.c_str(), range->start_octets);
        if (status != IPV4_RANGE_OK) {
            return status;
        }

        /* Valider le préfixe CIDR */
        try {
            int prefix = std::stoi(prefix_str);
            if (prefix < 0 || prefix > 32) {
                return IPV4_RANGE_ERR_INVALID_CIDR;
            }
            range->cidr_prefix = prefix;
        } catch (...) {
            return IPV4_RANGE_ERR_INVALID_CIDR;
        }

        /* Calculer l'adresse de fin */
        status = calculate_cidr_range(range->start_octets, range->cidr_prefix, range->end_octets);
        if (status != IPV4_RANGE_OK) {
            return status;
        }
    } else {
        return IPV4_RANGE_ERR_INVALID_FORMAT;
    }

    /* Convertir les octets en strings */
    ipv4_range_status_t status = octets_to_string(range->start_octets, range->start_str, sizeof(range->start_str));
    if (status != IPV4_RANGE_OK) {
        return status;
    }

    status = octets_to_string(range->end_octets, range->end_str, sizeof(range->end_str));
    if (status != IPV4_RANGE_OK) {
        return status;
    }

    return IPV4_RANGE_OK;
}

/**
 * @brief Gère la commande de plage IPv4
 */
void handle_range_ipv4_command(const std::string& command)
{
    if (command.rfind("set range ipv4 : ", 0) == 0) {
        std::string range_str = command.substr(17);

        ipv4_range_t temp_range = {0};
        ipv4_range_status_t status = parse_range(range_str, &temp_range);

        if (status == IPV4_RANGE_OK) {
            current_range = temp_range;
            range_defined = true;
            std::cout << "\033[1;32m[+] Plage IPv4 définie !\033[0m\n";
            display_range_ipv4(&current_range);
        } else {
            handle_range_ipv4_error(status);
        }
    } else if (command == "show range ipv4") {
        if (!range_defined) {
            std::cout << "\033[1;33m[-] Aucune plage IPv4 définie.\033[0m\n";
        } else {
            display_range_ipv4(&current_range);
        }
    }
}

/**
 * @brief Affiche le message d'erreur approprié
 */
void handle_range_ipv4_error(ipv4_range_status_t status)
{
    std::cout << "\n\033[1;31m❌ ERREUR : ";

    switch (status) {
        case IPV4_RANGE_OK:
            std::cout << "Aucune erreur.\033[0m\n";
            break;
        case IPV4_RANGE_ERR_NULL_INPUT:
            std::cout << "Pointeur NULL fourni.\033[0m\n";
            break;
        case IPV4_RANGE_ERR_EMPTY_INPUT:
            std::cout << "La plage IPv4 ne peut pas être vide.\033[0m\n";
            break;
        case IPV4_RANGE_ERR_INVALID_FORMAT:
            std::cout << "Format invalide.\033[0m\n";
            std::cout << "\033[1;36m📝 Formats acceptés :\033[0m\n";
            std::cout << "   - Plage : set range ipv4 : 192.168.1.0-192.168.1.255\n";
            std::cout << "   - CIDR  : set range ipv4 : 192.168.1.0/24\n";
            break;
        case IPV4_RANGE_ERR_INVALID_OCTET:
            std::cout << "Un ou plusieurs octets sont invalides (doit être 0-255).\033[0m\n";
            break;
        case IPV4_RANGE_ERR_INVALID_CIDR:
            std::cout << "Préfixe CIDR invalide (doit être 0-32).\033[0m\n";
            break;
        case IPV4_RANGE_ERR_START_GREATER_END:
            std::cout << "L'adresse de début ne peut pas être supérieure à celle de fin.\033[0m\n";
            break;
        case IPV4_RANGE_ERR_BUFFER_OVERFLOW:
            std::cout << "Plage IPv4 trop longue.\033[0m\n";
            break;
        default:
            std::cout << "Erreur inconnue (code: " << status << ").\033[0m\n";
    }

    std::cout << "\033[1;36m📝 Réessayez avec une plage IPv4 valide.\033[0m\n\n";
}

/**
 * @brief Affiche la plage IPv4 validée
 */
void display_range_ipv4(const ipv4_range_t *range)
{
    if (!range) {
        std::cout << "Erreur : pointeur NULL\n";
        return;
    }

    std::cout << "\033[1;36m[*] Plage IPv4 : " << range->start_str << " → " << range->end_str;
    
    if (range->cidr_prefix >= 0) {
        std::cout << " (CIDR: /" << range->cidr_prefix << ")";
    }

    std::cout << "\033[0m\n";
}

/**
 * @brief Récupère la plage IPv4 actuelle
 */
ipv4_range_t* get_range_ipv4(void)
{
    if (range_defined) {
        return &current_range;
    }
    return nullptr;
}
