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

     std::stringstream ss(range_str);
     std::string start_str, end_str, extra;

     if (!(ss >> start_str)) {
         return PORT_RANGE_ERR_INVALID_FORMAT;
     }

     if (!(ss >> end_str)) {
         return PORT_RANGE_ERR_INVALID_FORMAT;
     }

     // Vérifier s'il y a des arguments en trop
     if (ss >> extra) {
         return PORT_RANGE_ERR_TOO_MANY_ARGS;
     }

     uint16_t p_start = 0, p_end = 0;
     port_range_status_t status;

     status = validate_port(start_str, &p_start);
     if (status != PORT_RANGE_OK) {
         return status;
     }

     status = validate_port(end_str, &p_end);
     if (status != PORT_RANGE_OK) {
         return status;
     }

     if (p_start > p_end) {
         return PORT_RANGE_ERR_START_GREATER_END;
     }

     range->port_start = p_start;
     range->port_end = p_end;

     return PORT_RANGE_OK;
}

/**
 * @brief Affiche le message d'erreur approprié
 */
void handle_range_port_error(port_range_status_t status)
{
    switch (status) {
        case PORT_RANGE_ERR_NULL_INPUT:
            std::cout << "[-] Erreur : Pointeur nul.\n";
            break;
        case PORT_RANGE_ERR_EMPTY_INPUT:
            std::cout << "[-] Erreur : L'entrée de la plage est vide.\n";
            break;
        case PORT_RANGE_ERR_INVALID_FORMAT:
            std::cout << "[-] Erreur : Format invalide. Attendu : [port_debut] [port_fin]\n";
            break;
        case PORT_RANGE_ERR_INVALID_PORT:
            std::cout << "[-] Erreur : Port invalide (doit être entre 1 et 65535).\n";
            break;
        case PORT_RANGE_ERR_START_GREATER_END:
            std::cout << "[-] Erreur : Le port de début ne peut pas être supérieur au port de fin.\n";
            break;
        case PORT_RANGE_ERR_TOO_MANY_ARGS:
            std::cout << "[-] Erreur : Trop d'arguments fournis pour la plage.\n";
            break;
        default:
            std::cout << "[-] Erreur : Erreur inconnue.\n";
            break;
    }
}

/**
 * @brief Gère la commande de définition de la plage de ports
 */
void handle_range_port_command(const std::string& command)
{
    port_range_t new_range;
    port_range_status_t status = parse_range_ports(command, &new_range);

    if (status != PORT_RANGE_OK) {
        handle_range_port_error(status);
        return;
    }

    current_range = new_range;
    range_defined = true;
    std::cout << "[+] Plage de ports configurée avec succès : " 
              << current_range.port_start << " - " << current_range.port_end << "\n";
}
