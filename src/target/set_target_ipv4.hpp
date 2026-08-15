#ifndef SET_TARGET_IPV4_HPP
#define SET_TARGET_IPV4_HPP

#include <cstdint>
#include <string>

/* Status codes for IPv4 target operations */
typedef enum {
    IPV4_TARGET_OK = 0,
    IPV4_TARGET_ERR_NULL_INPUT = 1,
    IPV4_TARGET_ERR_EMPTY_INPUT = 2,
    IPV4_TARGET_ERR_INVALID_FORMAT = 3,
    IPV4_TARGET_ERR_INVALID_OCTET = 4,
    IPV4_TARGET_ERR_BUFFER_OVERFLOW = 5
} ipv4_target_status_t;

/* Structure for storing a single IPv4 target */
typedef struct {
    uint8_t octets[4];              /* IP address octets (0-255) */
    char ip_str[16];                /* String representation (e.g., "192.168.1.1") */
} ipv4_target_t;

/* Function declarations */
void handle_target_ipv4_command(const std::string& command);
void handle_target_ipv4_error(ipv4_target_status_t status);
void display_target_ipv4(const ipv4_target_t *target);
ipv4_target_t* get_target_ipv4(void);

#endif

