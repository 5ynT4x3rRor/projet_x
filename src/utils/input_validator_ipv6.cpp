#include "input_validator_ipv6.hpp"
#include <cstring>

ipv6_status_t validate_ipv6_string(const std::string& ip_str)
{
    if (ip_str.empty()) {
        return IPV6_ERR_EMPTY;
    }

    if (ip_str.length() > MAX_IPV6_LENGTH) {
        return IPV6_ERR_TOO_LONG;
    }

    // Validation du format via inet_pton pour l'IPv6
    char buf[sizeof(struct in6_addr)];
    int result = inet_pton(AF_INET6, ip_str.c_str(), buf);
    
    if (result != 1) {
        return IPV6_ERR_INVALID_FORMAT;
    }

    return IPV6_VALID;
}
