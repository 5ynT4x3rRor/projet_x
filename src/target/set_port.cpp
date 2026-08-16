#include <iostream>
#include "set_port.hpp"
#include "utils/input_validator.hpp"

bool handle_set_port(const std::string& port_arg) {
    int port = 0;
    input_status_t status = validate_port_string(port_arg, port);

    if (status != INPUT_VALID) {
        std::cout << "[-] Erreur port : " << get_validation_error_message(status) << "\n";
        return false;
    }

    std::cout << "[+] Port configuré avec succès : " << port << "\n";
    return true;
}
