#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <cstdint>
#include "set_target_ipv4.hpp"

/* Global variable to store the current target IPv4 */
static ipv4_target_t current_target = {
    .octets = {0, 0, 0, 0},
    .ip_str = {0}
};

static bool target_defined = false;

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

    int parsed_octets[4] = {-1, -1, -1, -1};
    int octet_index = 0;
    size_t current_octet_len = 0;
    const char *current_octet_start = input;

    for (size_t i = 0; i <= input_len; i++) {
        char c = input[i];

        if (c == '.' || c == '\0') {
            if (current_octet_len == 0) {
                return IPV4_TARGET_ERR_INVALID_FORMAT;
            }

            int octet_value = validate_octet(current_octet_start, current_octet_len);
            if (octet_value < 0) {
                return IPV4_TARGET_ERR_INVALID_OCTET;
            }

            parsed_octets[octet_index] = octet_value;
            octet_index++;

            if (c == '\0') {
                if (octet_index != 4) {
                    return IPV4_TARGET_ERR_INVALID_FORMAT;
                }
                break;
            }

            if (octet_index > 4) {
                return IPV4_TARGET_ERR_INVALID_FORMAT;
            }

            current_octet_start = &input[i + 1];
            current_octet_len = 0;
        } else if (std::isdigit((unsigned char)c)) {
            current_octet_len++;
        } else {
            return IPV4_TARGET_ERR_INVALID_FORMAT;
        }
    }

    if (octet_index != 4) {
        return IPV4_TARGET_ERR_INVALID_FORMAT;
    }

    for (int i = 0; i < 4; i++) {
        octets[i] = (uint8_t)parsed_octets[i];
    }

    return IPV4_TARGET_OK;
}

/**
 * @brief Converts an octet array to an IPv4 string
 */
static ipv4_target_status_t octets_to_string(const uint8_t *octets, char *str, size_t str_len)
{
    if (!octets || !str || str_len < 16) {
        return IPV4_TARGET_ERR_BUFFER_OVERFLOW;
    }

    int result = std::snprintf(str, str_len, "%u.%u.%u.%u",
                               octets[0], octets[1], octets[2], octets[3]);

    if (result < 0 || result >= (int)str_len) {
        return IPV4_TARGET_ERR_BUFFER_OVERFLOW;
    }

    return IPV4_TARGET_OK;
}

/**
 * @brief Parses and validates a single IPv4 target address
 */
static ipv4_target_status_t parse_target(const std::string& target_str, ipv4_target_t *target)
{
    if (!target) {
        return IPV4_TARGET_ERR_NULL_INPUT;
    }

    if (target_str.empty()) {
        return IPV4_TARGET_ERR_EMPTY_INPUT;
    }

    /* Parse the IPv4 address */
    ipv4_target_status_t status = parse_ipv4(target_str.c_str(), target->octets);
    if (status != IPV4_TARGET_OK) {
        return status;
    }

    /* Convert octets to string */
    status = octets_to_string(target->octets, target->ip_str, sizeof(target->ip_str));
    if (status != IPV4_TARGET_OK) {
        return status;
    }

    return IPV4_TARGET_OK;
}

/**
 * @brief Handles the IPv4 target command
 */
void handle_target_ipv4_command(const std::string& command)
{
    if (command.rfind("set target ipv4 : ", 0) == 0) {
        std::string target_str = command.substr(18);

        ipv4_target_t temp_target = {0};
        ipv4_target_status_t status = parse_target(target_str, &temp_target);

        if (status == IPV4_TARGET_OK) {
            current_target = temp_target;
            target_defined = true;
            std::cout << "\033[1;32m[+] Cible IPv4 définie !\033[0m\n";
            display_target_ipv4(&current_target);
        } else {
            handle_target_ipv4_error(status);
        }
    } else if (command == "show target ipv4") {
        if (!target_defined) {
            std::cout << "\033[1;33m[-] Aucune cible IPv4 définie.\033[0m\n";
        } else {
            display_target_ipv4(&current_target);
        }
    }
}

/**
 * @brief Displays the appropriate error message
 */
void handle_target_ipv4_error(ipv4_target_status_t status)
{
    std::cout << "\n\033[1;31m❌ ERREUR : ";

    switch (status) {
        case IPV4_TARGET_OK:
            std::cout << "Aucune erreur.\033[0m\n";
            break;
        case IPV4_TARGET_ERR_NULL_INPUT:
            std::cout << "Pointeur NULL fourni.\033[0m\n";
            break;
        case IPV4_TARGET_ERR_EMPTY_INPUT:
            std::cout << "La cible IPv4 ne peut pas être vide.\033[0m\n";
            break;
        case IPV4_TARGET_ERR_INVALID_FORMAT:
            std::cout << "Format invalide.\033[0m\n";
            std::cout << "\033[1;36m📝 Format accepté :\033[0m\n";
            std::cout << "   - set target ipv4 : 192.168.1.1\n";
            break;
        case IPV4_TARGET_ERR_INVALID_OCTET:
            std::cout << "Un ou plusieurs octets sont invalides (doit être 0-255).\033[0m\n";
            break;
        case IPV4_TARGET_ERR_BUFFER_OVERFLOW:
            std::cout << "Adresse IPv4 trop longue.\033[0m\n";
            break;
        default:
            std::cout << "Erreur inconnue (code: " << status << ").\033[0m\n";
    }

    std::cout << "\033[1;36m📝 Réessayez avec une adresse IPv4 valide.\033[0m\n\n";
}

/**
 * @brief Displays the validated IPv4 target
 */
void display_target_ipv4(const ipv4_target_t *target)
{
    if (!target) {
        std::cout << "Erreur : pointeur NULL\n";
        return;
    }

    std::cout << "\033[1;36m[*] Cible IPv4 : " << target->ip_str << "\033[0m\n";
}

/**
 * @brief Gets the current IPv4 target
 */
ipv4_target_t* get_target_ipv4(void)
{
    if (target_defined) {
        return &current_target;
    }
    return nullptr;
}
