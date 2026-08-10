#ifndef TARGET_HPP
#define TARGET_HPP

#include <string>

// Déclarations globales (extern pour éviter les doublons)
extern std::string target;
extern int port_start;
extern int port_end;

void handle_clear_command();
void handle_commands();

#endif
