#include <iostream>
#include <string>
#include <cstring>
#include <cctype>
#include <algorithm>
#include "input_validator.hpp"

/**
 * @brief Validates an IPv4 address string for format and length
 */
input_status_t validate_ipv4_string(const std::string& ip_str)
{
    if (ip_str.empty()) {
        return INPUT_ERR_EMPTY;
    }

    if (ip_str.length() > MAX_IP_LENGTH) {
        return INPUT_ERR_TOO_LONG;
    }

    /* Check for dangerous characters */
    input_status_t inj = check_injection_patterns(ip_str);
    if (inj != INPUT_VALID) {
        return inj;
    }

    /* Validate IPv4 format: must contain exactly 3 dots */
    int dot_count = 0;
    for (char c : ip_str) {
        if (c == '.') dot_count++;
        else if (!std::isdigit(static_cast<unsigned char>(c))) {
            return INPUT_ERR_INVALID_CHARS;
        }
    }

    if (dot_count != 3) {
        return INPUT_ERR_INVALID_FORMAT;
    }

    return INPUT_VALID;
}

/**
 * @brief Validates a port number within acceptable range
 */
input_status_t validate_port_string(const std::string& port_str, int& port)
{
    if (port_str.empty()) {
        return INPUT_ERR_EMPTY;
    }

    if (port_str.length() > MAX_PORT_LENGTH) {
        return INPUT_ERR_TOO_LONG;
    }

    /* Check for injection patterns */
    input_status_t inj = check_injection_patterns(port_str);
    if (inj != INPUT_VALID) {
        return inj;
    }

    /* Check that all characters are digits */
    for (char c : port_str) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return INPUT_ERR_INVALID_CHARS;
        }
    }

    /* Convert to integer and check range */
    try {
        port = std::stoi(port_str);
        if (port < MIN_PORT || port > MAX_PORT) {
            return INPUT_ERR_RANGE;
        }
    } catch (...) {
        return INPUT_ERR_OVERFLOW;
    }

    return INPUT_VALID;
}

/**
 * @brief Validates a filename for safe filesystem operations
 */
input_status_t validate_filename(const std::string& filename)
{
    if (filename.empty()) {
        return INPUT_ERR_EMPTY;
    }

    if (filename.length() > MAX_FILENAME_LENGTH) {
        return INPUT_ERR_TOO_LONG;
    }

    /* Check for path traversal attempts */
    if (filename.find("..") != std::string::npos ||
        filename.find("/") != std::string::npos ||
        filename.find("\\") != std::string::npos) {
        return INPUT_ERR_SUSPICIOUS;
    }

    /* Check for dangerous shell characters */
    input_status_t inj = check_injection_patterns(filename);
    if (inj != INPUT_VALID) {
        return inj;
    }

    /* Whitelist allowed characters for filenames */
    for (char c : filename) {
        if (!std::isalnum(static_cast<unsigned char>(c)) && 
            c != '.' && c != '_' && c != '-') {
            return INPUT_ERR_INVALID_CHARS;
        }
    }

    return INPUT_VALID;
}

/**
 * @brief Checks for suspicious patterns in input (injection attempts)
 */
input_status_t check_injection_patterns(const std::string& input)
{
    if (input.empty()) {
        return INPUT_VALID;
    }

    /* Check for shell metacharacters and injection patterns */
    const char* dangerous = DANGEROUS_CHARS;
    for (size_t i = 0; i < std::strlen(dangerous); i++) {
        if (input.find(dangerous[i]) != std::string::npos) {
            return INPUT_ERR_SUSPICIOUS;
        }
    }

    /* Check for common SQL injection patterns */
    const char* sql_patterns[] = {"' OR", "\" OR", "--", "/*", "*/", "xp_", "sp_"};
    for (size_t i = 0; i < sizeof(sql_patterns) / sizeof(sql_patterns[0]); i++) {
        if (input.find(sql_patterns[i]) != std::string::npos) {
            return INPUT_ERR_SUSPICIOUS;
        }
    }

    /* Check for command injection patterns */
    const char* cmd_patterns[] = {";", "|", "&", "&&", "||", "`"};
    for (size_t i = 0; i < sizeof(cmd_patterns) / sizeof(cmd_patterns[0]); i++) {
        if (input.find(cmd_patterns[i]) != std::string::npos) {
            return INPUT_ERR_SUSPICIOUS;
        }
    }

    return INPUT_VALID;
}

/**
 * @brief Sanitizes input by removing/escaping dangerous characters
 */
std::string sanitize_input(const std::string& input)
{
    std::string result = input;
    
    /* Remove dangerous characters */
    const char* dangerous = DANGEROUS_CHARS;
    for (size_t i = 0; i < std::strlen(dangerous); i++) {
        result.erase(std::remove(result.begin(), result.end(), dangerous[i]), result.end());
    }

    /* Trim whitespace */
    size_t start = result.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    size_t end = result.find_last_not_of(" \t\r\n");
    return result.substr(start, (end - start + 1));
}

/**
 * @brief Converts error code to human-readable message
 */
std::string get_validation_error_message(input_status_t status)
{
    switch (status) {
        case INPUT_VALID:
            return "Validée avec succès";
        case INPUT_ERR_EMPTY:
            return "L'entrée ne peut pas être vide";
        case INPUT_ERR_TOO_LONG:
            return "L'entrée est trop longue";
        case INPUT_ERR_INVALID_CHARS:
            return "L'entrée contient des caractères invalides";
        case INPUT_ERR_OVERFLOW:
            return "Débordement numériquement";
        case INPUT_ERR_RANGE:
            return "La valeur est hors limites";
        case INPUT_ERR_SUSPICIOUS:
            return "L'entrée contient des caractères suspectes (injection détectée)";
        default:
            return "Erreur inconnue";
    }
}

/**
 * @brief Safe getline wrapper with length protection
 */
bool safe_getline(std::string& input, size_t max_length)
{
    if (!std::getline(std::cin, input)) {
        return false;
    }

    /* Enforce maximum length */
    if (input.length() > max_length) {
        std::cerr << "\033[1;31m[-] Erreur : Entrée trop longue (max " << max_length << " caractères)\033[0m\n";
        input.clear();
        return false;
    }

    return true;
}

/**
 * @brief Validates the main command prompt input against an active whitelist
 */
bool validate_command_whitelist(const std::string& command)
{
    // Liste blanche stricte des commandes actives et autorisées
    const std::string active_commands[] = {
        "help",
        "exit",
        "scan",
        "set target",
        "set port",
        "proxy"
    };

    for (const auto& cmd : active_commands) {
        if (command == cmd || command.rfind(cmd + " ", 0) == 0) {
            return true;
        }
    }

    return false;
}
