#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

void clear_console();
bool safe_getline(std::string& input, size_t max_length);
std::string sanitize_input(const std::string& input);
bool validate_command_whitelist(const std::string& command);

#endif // UTILS_HPP
