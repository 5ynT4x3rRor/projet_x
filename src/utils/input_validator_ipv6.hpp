#ifndef INPUT_VALIDATOR_IPV6_HPP
#define INPUT_VALIDATOR_IPV6_HPP

#include <string>
#include <arpa/inet.h>

// Codes d'état spécifiques pour l'IPv6
enum ipv6_status_t {
    IPV6_VALID = 0,
    IPV6_ERR_EMPTY,
    IPV6_ERR_TOO_LONG,
    IPV6_ERR_INVALID_FORMAT,
    IPV6_ERR_SUSPICIOUS
};

// Constante de longueur max pour une IPv6 standard (39 caractères)
const size_t MAX_IPV6_LENGTH = 39;

// Prototype de la fonction de validation IPv6
ipv6_status_t validate_ipv6_string(const std::string& ip_str);

#endif // INPUT_VALIDATOR_IPV6_HPP
