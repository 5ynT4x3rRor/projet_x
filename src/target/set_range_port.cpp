#include <iostream>
#include <string>
#include <sstream>
#include <cstdint>
#include <cstdlib>
#include "set_range_port.hpp"

/* Variable globale pour stocker la plage actuelle */
static port_range_t current_range = {
    .port_start = 0,
    .port_end = 0
};

static bool range_defined = false;

/**
 * @brief Valide qu'une chaîne est un nombre décimal valide (port)
 */
static port_range_status_t validate_port(const std::string& str, uint16_t *port)
{
    if (str.empty()) {
        return PORT_RANGE_ERR_EMPTY_INPUT;
    }

    try {
        int value = std::stoi(str);

        /* Vérifier que le port est dans la plage valide */
        if (value < 1 || value > 65535) {
            return PORT_RANGE_ERR_INVALID_PORT;
        }

        *port = (uint16_t)value;
        return PORT_RANGE_OK;
    } catch (...) {
        return PORT_RANGE_ERR_INVALID_PORT;
    }
}

/**
 * @brief Parse et valide une plage de ports
 */
static port_range_status_t parse_range_ports(const std::string& range_str, port_range_t *range)
{
    if (!range) {
        return PORT_RANGE_ERR_NULL_INPUT;
    }

    if (range_str.empty()) {
        return PORT_RANGE_ERR_EMPTY_INPUT;
    }

    /* Utiliser un stringstream pour parser les deux ports */
    std::istringstream iss(range_str);
    std::string start_str, end_str, extra_str;

    /* Lire le premier port */
    if (!(iss >> start_str)) {
        return PORT_RANGE_ERR_INVALID_FORMAT;
    }

    /* Lire le deuxième port */
    if (!(iss >> end_str)) {
        return PORT_RANGE_ERR_INVALID_FORMAT;
    }

    /* Vérifier qu'il n'y a pas d'arguments supplémentaires */
    if (iss >> extra_str) {
        return PORT_RANGE_ERR_TOO_MANY_ARGS;
    }

    /* Valider les deux ports */
    uint16_t start_port = 0;
    uint16_t end_port = 0;

    port_range_status_t status = validate_port(start_str, &start_port);
    if (status != PORT_RANGE_OK) {
        return status;
    }

    status = validate_port(end_str, &end_port);
    if (status != PORT_RANGE_OK) {
        return status;
    }

    /* Vérifier que start <= end */
    if (start_port > end_port) {
        return PORT_RANGE_ERR_START_GREATER_END;
    }

    range->port_start = start_port;
    range->port_end = end_port;

    return PORT_RANGE_OK;
}

/**
 * @brief Gère la commande de plage de ports
 */
void handle_range_port_command(const std::string& command)
{
    if (command.rfind("set range port : ", 0) == 0) {
        std::string range_str = command.substr(17);

        port_range_t temp_range = {0, 0};
        port_range_status_t status = parse_range_ports(range_str, &temp_range);

        if (status == PORT_RANGE_OK) {
            current_range = temp_range;
            range_defined = true;
            std::cout << "\033[1;32m[+] Plage de ports définie !\033[0m\n";
            display_range_port(&current_range);
        } else {
            handle_range_port_error(status);
        }
    } else if (command == "show range port") {
        if (!range_defined) {
            std::cout << "\033[1;33m[-] Aucune plage de ports définie.\033[0m\n";
        } else {
            display_range_port(&current_range);
        }
    }
}

/**
 * @brief Affiche le message d'erreur approprié
 */
void handle_range_port_error(port_range_status_t status)
{
    std::cout << "\n\033[1;31m❌ ERREUR : ";

    switch (status) {
        case PORT_RANGE_OK:
            std::cout << "Aucune erreur.\033[0m\n";
            break;
        case PORT_RANGE_ERR_NULL_INPUT:
            std::cout << "Pointeur NULL fourni.\033[0m\n";
            break;
        case PORT_RANGE_ERR_EMPTY_INPUT:
            std::cout << "La plage de ports ne peut pas être vide.\033[0m\n";
            break;
        case PORT_RANGE_ERR_INVALID_FORMAT:
            std::cout << "Format invalide.\033[0m\n";
            std::cout << "\033[1;36m📝 Format attendu : set range port : 1000 2000\033[0m\n";
            break;
        case PORT_RANGE_ERR_INVALID_PORT:
            std::cout << "Un ou plusieurs ports sont invalides (doit être 1-65535).\033[0m\n";
            break;
        case PORT_RANGE_ERR_START_GREATER_END:
            std::cout << "Le port de début ne peut pas être supérieur au port de fin.\033[0m\n";
            break;
        case PORT_RANGE_ERR_TOO_MANY_ARGS:
            std::cout << "Trop d'arguments fournis.\033[0m\n";
            std::cout << "\033[1;36m📝 Format attendu : set range port : 1000 2000\033[0m\n";
            break;
        default:
            std::cout << "Erreur inconnue (code: " << status << ").\033[0m\n";
    }

    std::cout << "\033[1;36m📝 Réessayez avec une plage de ports valide (1-65535).\033[0m\n\n";
}

/**
 * @brief Affiche la plage de ports validée
 */
void display_range_port(const port_range_t *range)
{
    if (!range) {
        std::cout << "Erreur : pointeur NULL\n";
        return;
    }

    std::cout << "\033[1;36m[*] Plage de ports : " << range->port_start << " → " << range->port_end << "\033[0m\n";
}

/**
 * @brief Récupère la plage de ports actuelle
 */
port_range_t* get_range_port(void)
{
    if (range_defined) {
        return &current_range;
    }
    return nullptr;
}
