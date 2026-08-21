#ifndef INPUT_VALIDATOR_IPV4_HPP
#define INPUT_VALIDATOR_IPV4_HPP

#include <string>

// Codes d'état pour la validation IPv4
enum ipv4_status_t {
    IPV4_VALID = 0,
    IPV4_ERR_EMPTY,
    IPV4_ERR_TOO_LONG,
    IPV4_ERR_INVALID_CHARS,
    IPV4_ERR_INVALID_FORMAT,
    IPV4_ERR_SUSPICIOUS
};

// Constante de longueur max pour une IPv4 standard (ex: 255.255.255.255)
const size_t MAX_IPV4_LENGTH = 15;

// Prototypes
ipv4_status_t validate_ipv4_string(const std::string& ip_str);

#endif // INPUT_VALIDATOR_IPV4_HPP
