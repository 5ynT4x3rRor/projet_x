#ifndef INPUT_VALIDATOR_HPP
#define INPUT_VALIDATOR_HPP

#include <string>

// Constantes de validation
#define MAX_IP_LENGTH 15
#define MAX_PORT_LENGTH 5
#define MAX_FILENAME_LENGTH 255
#define MIN_PORT 1
#define MAX_PORT 65535
#define DANGEROUS_CHARS ";&|`$<>"


// Codes d'erreur complets
typedef enum {
    INPUT_VALID = 0,
    INPUT_ERR_EMPTY = -1,
    INPUT_ERR_INVALID_FORMAT = -2,
    INPUT_ERR_OUT_OF_RANGE = -3,
    INPUT_ERR_TOO_LONG = -4,
    INPUT_ERR_INVALID_CHARS = -5,
    INPUT_ERR_OVERFLOW = -6,
    INPUT_ERR_RANGE = -7,
    INPUT_ERR_SUSPICIOUS = -8
} input_status_t;

// Déclarations des fonctions
bool safe_getline(std::string &str, size_t max_len);
std::string sanitize_input(const std::string &input);
bool validate_command_whitelist(const std::string &cmd);
input_status_t validate_ipv4_string(const std::string& ip_str);
input_status_t validate_port_string(const std::string& port_str, int &port);
input_status_t validate_filename(const std::string& filename);
input_status_t check_injection_patterns(const std::string& str);
std::string get_validation_error_message(input_status_t status);

#endif // INPUT_VALIDATOR_HPP
