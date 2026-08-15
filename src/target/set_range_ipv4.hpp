#ifndef SET_RANGE_IPV4_HPP
#define SET_RANGE_IPV4_HPP

#include <string>
#include <cstdint>

/*
 * Types et codes d'erreur utilisés par set_range_ipv4.cpp
 * (Doit correspondre aux usages dans le .cpp)
 */

typedef enum {
    IPV4_RANGE_OK = 0,
    IPV4_RANGE_ERR_NULL_INPUT,
    IPV4_RANGE_ERR_EMPTY_INPUT,
    IPV4_RANGE_ERR_INVALID_FORMAT,
    IPV4_RANGE_ERR_INVALID_OCTET,
    IPV4_RANGE_ERR_INVALID_CIDR,
    IPV4_RANGE_ERR_START_GREATER_END,
    IPV4_RANGE_ERR_BUFFER_OVERFLOW,
    IPV4_RANGE_ERR_UNKNOWN
} ipv4_range_status_t;

/* Structure représentant une plage IPv4 */
typedef struct {
    uint8_t start_octets[4];
    uint8_t end_octets[4];
    char start_str[16]; /* "255.255.255.255" + NUL */
    char end_str[16];
    int cidr_prefix;    /* -1 si non applicable */
} ipv4_range_t;

/* Fonctions exportées (définies dans set_range_ipv4.cpp) */
void handle_range_ipv4_command(const std::string& command);
void handle_range_ipv4_error(ipv4_range_status_t status);
void display_range_ipv4(const ipv4_range_t *range);
ipv4_range_t* get_range_ipv4(void);

#endif /* SET_RANGE_IPV4_HPP */
