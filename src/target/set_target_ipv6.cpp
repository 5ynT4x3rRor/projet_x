#include "set_target_ipv6.hpp"
#include <arpa/inet.h>
#include <iostream>

SetTargetIPv6::SetTargetIPv6() : targetIPv6("::1") {}

SetTargetIPv6::SetTargetIPv6(const std::string& initialTarget) {
    if (!setTarget(initialTarget)) {
        targetIPv6 = "::1";
    }
}

bool SetTargetIPv6::validateIPv6(const std::string& ip) const {
    char buf[sizeof(struct in6_addr)];
    // inet_pton convertit une chaîne IPv6 en structure binaire (renvoie 1 si valide, 0 ou -1 sinon)
    return inet_pton(AF_INET6, ip.c_str(), buf) == 1;
}

bool SetTargetIPv6::setTarget(const std::string& input) {
    if (validateIPv6(input)) {
        targetIPv6 = input;
        return true;
    }
    return false;
}

std::string SetTargetIPv6::getTarget() const {
    return targetIPv6;
}
