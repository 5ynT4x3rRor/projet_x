#include "input_validator_ipv4.hpp"
#include <cctype>
#include <cstring>

/**
 * @brief Vérifie les motifs d'injection basiques
 */
static ipv4_status_t check_ipv4_injection(const std::string& input)
{
    const char* dangerous[] = {";", "|", "&", "`", "$", "(", ")", "'", "\"", nullptr};
    for (int i = 0; dangerous[i] != nullptr; i++) {
        if (input.find(dangerous[i]) != std::string::npos) {
            return IPV4_ERR_SUSPICIOUS;
        }
    }
    return IPV4_VALID;
}

/**
 * @brief Valide une adresse IPv4 (format et longueur)
 */
ipv4_status_t validate_ipv4_string(const std::string& ip_str)
{
    if (ip_str.empty()) {
        return IPV4_ERR_EMPTY;
    }

    if (ip_str.length() > MAX_IPV4_LENGTH) {
        return IPV4_ERR_TOO_LONG;
    }

    // Vérification des caractères dangereux / injections
    ipv4_status_t inj = check_ipv4_injection(ip_str);
    if (inj != IPV4_VALID) {
        return inj;
    }

    // Validation du format IPv4 : uniquement chiffres et 3 points
    int dot_count = 0;
    for (char c : ip_str) {
        if (c == '.') {
            dot_count++;
        } else if (!std::isdigit(static_cast<unsigned char>(c))) {
            return IPV4_ERR_INVALID_CHARS;
        }
    }

    if (dot_count != 3) {
        return IPV4_ERR_INVALID_FORMAT;
    }

    return IPV4_VALID;
}
