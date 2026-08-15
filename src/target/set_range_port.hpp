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
 * 
 * Format accepté :
 * - "set range port : 1000 2000"
 * 
 * Validation :
 * - Ports valides (1-65535)
 * - Plage valide (start <= end)
 * - Format : deux arguments séparés par un espace
 */
void handle_range_port_command(const std::string& command);

/**
 * @brief Affiche le message d'erreur approprié
 * 
 * @param status Code d'erreur
 */
void handle_range_port_error(port_range_status_t status);

/**
 * @brief Affiche la plage de ports validée
 * 
 * @param range Pointeur vers structure port_range_t
 */
void display_range_port(const port_range_t *range);

/**
 * @brief Récupère la plage de ports actuellement définie
 * 
 * @return Pointeur vers la plage de ports (NULL si aucune définie)
 */
port_range_t* get_range_port(void);

#endif /* SET_RANGE_PORT_HPP */
