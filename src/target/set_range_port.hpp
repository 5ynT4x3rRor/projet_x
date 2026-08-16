#ifndef SET_RANGE_PORT_HPP
#define SET_RANGE_PORT_HPP

#include <string>
#include <cstdint>

/**
 * @brief Structure pour stocker une plage de ports validée
 */
typedef struct {
     uint16_t port_start;  /* Port de début (1-65535) */
     uint16_t port_end;    /* Port de fin (1-65535) */
} port_range_t;

/**
 * @brief Codes de retour pour set_range_port
 */
typedef enum {
     PORT_RANGE_OK = 0,
     PORT_RANGE_ERR_NULL_INPUT = -1,
     PORT_RANGE_ERR_EMPTY_INPUT = -2,
     PORT_RANGE_ERR_INVALID_FORMAT = -3,
     PORT_RANGE_ERR_INVALID_PORT = -4,
     PORT_RANGE_ERR_START_GREATER_END = -5,
     PORT_RANGE_ERR_TOO_MANY_ARGS = -6
} port_range_status_t;

/**
 * @brief Valide et définit une plage de ports
 * 
 * @param command Chaîne de commande (ex: "set range port : 1000 2000")
 */
void handle_range_port_command(const std::string& command);

/**
 * @brief Affiche le message d'erreur approprié
 * 
 * @param status Code d'erreur
 */
void handle_range_port_error(port_range_status_t status);

#endif // SET_RANGE_PORT_HPP
