#ifndef INPUT_VALIDATOR_PORT_HPP
#define INPUT_VALIDATOR_PORT_HPP

#include <string>

// Valide qu'un port unique est un entier valide entre 1 et 65535
bool validate_port(const std::string& port_str);

#endif // INPUT_VALIDATOR_PORT_HPP
